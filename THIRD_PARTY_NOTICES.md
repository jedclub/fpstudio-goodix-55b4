# Third-party notices

## libfprint goodixtls 55x4 fork

The driver package fetches the `55b4-experimental` revision
`c1937b99ec3db5abca05f619a95d2e37496d8810` from
[`TheWeirdDev/libfprint`](https://github.com/TheWeirdDev/libfprint).  The
driver modifications in `src/driver/patches/` and the imported SIGFM headers
are licensed under the GNU Lesser General Public License, version 2.1 or (at
your option) any later version.  The complete licence is in [LICENSE](LICENSE).

This project does not redistribute the upstream driver source tree, firmware
blobs, device keys, fingerprint images, templates, or enrolment data.  The
PKGBUILD retrieves the reviewed upstream source revision and applies the
patches locally.

## Runtime dependencies

FPStudio is built against Qt 6, Vulkan, GLib and libfprint.  The Arch package
recipe also depends on libgusb, OpenSSL, pixman, NSS, libgudev and OpenCV.
They remain distribution-provided shared dependencies; a binary package that
bundles any of them must include that dependency's own licence notices.
