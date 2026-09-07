// Reads the enrolled templates the tool already wrote, instead of asking for
// fresh touches.
//
// This is the better sample set, not just the cheaper one. A stage in an
// enrolled print is exactly what the driver kept from one real touch during
// a real enrolment - same normalisation, same quality gate, same SIFT call -
// so scoring stages against each other measures the matcher on the placements
// enrolment actually produced. A separate capture loop would measure a
// slightly different pipeline and a placement distribution I invented.
//
// Format, from libfprint/fp-print.c: the three bytes "FP3", then a GVariant
// of type (issbymsmsia{sv}v) whose trailing variant is (a(ay)) for a SIGFM
// print - one byte array per enrol stage, each the output of
// sigfm_serialize_binary.

#include "upstream/img-info.hpp"
#include "upstream/binary.hpp"

#include <gio/gio.h>
#include <glib.h>

#include <cstdio>
#include <string>
#include <vector>

namespace bin {
template<>
struct deserializer<SigfmImgInfo> : public std::true_type {
    static SigfmImgInfo deserialize(stream &in)
    {
        SigfmImgInfo info;
        in >> info.keypoints >> info.descriptors;
        return info;
    }
};
} // namespace bin

bool load_print_stages(const std::string &path,
                       std::vector<SigfmImgInfo> *out,
                       std::string *error)
{
    gchar *data = nullptr;
    gsize len = 0;
    GError *err = nullptr;
    if (!g_file_get_contents(path.c_str(), &data, &len, &err)) {
        *error = err ? err->message : "cannot read";
        if (err) g_error_free(err);
        return false;
    }

    if (len < 4 || data[0] != 'F' || data[1] != 'P' || data[2] != '3') {
        g_free(data);
        *error = "not an FP3 print";
        return false;
    }

    // g_variant_new_from_data needs the buffer to outlive the variant; the
    // copy also guarantees the alignment GVariant wants.
    GBytes *bytes = g_bytes_new(data + 3, len - 3);
    g_free(data);

    GVariant *v = g_variant_new_from_bytes(
        G_VARIANT_TYPE("(issbymsmsia{sv}v)"), bytes, FALSE);
    if (!v) { g_bytes_unref(bytes); *error = "malformed variant"; return false; }

    gint type = 0;
    g_variant_get_child(v, 0, "i", &type);
    const int FPI_PRINT_SIGFM = 3; // fpi-print.h; NBIS=1, RAW=2, SIGFM=3

    GVariant *inner = g_variant_get_child_value(v, 9);   // the trailing 'v'
    GVariant *payload = g_variant_get_variant(inner);

    if (!g_variant_is_of_type(payload, G_VARIANT_TYPE("(a(ay))"))) {
        *error = std::string("print type ") + std::to_string(type) +
                 " is not SIGFM (payload " +
                 g_variant_get_type_string(payload) + ")";
        g_variant_unref(payload); g_variant_unref(inner);
        g_variant_unref(v); g_bytes_unref(bytes);
        return false;
    }
    (void) FPI_PRINT_SIGFM;

    GVariant *arr = g_variant_get_child_value(payload, 0);
    const gsize n = g_variant_n_children(arr);
    for (gsize i = 0; i < n; i++) {
        GVariant *tup = g_variant_get_child_value(arr, i);
        GVariant *ay = g_variant_get_child_value(tup, 0);
        gsize slen = 0;
        const guchar *ser = (const guchar *)
            g_variant_get_fixed_array(ay, &slen, sizeof(guchar));
        try {
            bin::stream s{ser, ser + slen};
            SigfmImgInfo info;
            s >> info;
            out->push_back(std::move(info));
        }
        catch (const std::exception &e) {
            std::fprintf(stderr, "stage %zu: %s\n", (size_t) i, e.what());
        }
        g_variant_unref(ay);
        g_variant_unref(tup);
    }

    g_variant_unref(arr);
    g_variant_unref(payload);
    g_variant_unref(inner);
    g_variant_unref(v);
    g_bytes_unref(bytes);
    return true;
}
