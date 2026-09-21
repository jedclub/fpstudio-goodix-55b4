#pragma once

// Named timing scopes for the matcher's CPU paths.
//
// The optimisation work that produced this found its targets by profiling
// rather than by reading: the function that looked expensive (the connected
// component search) was not, and the one that did not (an 11x11 window written
// as two nested loops) was most of the cost. Keeping the scopes in the source
// means the next person does not have to rebuild that scaffolding to ask the
// same question, and it means a regression can be attributed to a function
// rather than to a release.
//
// Off unless the build asks for it. FPSTUDIO_PROFILE is not defined in a normal
// build, and then every macro below expands to nothing at all - no branch, no
// counter, no atomic, nothing for the optimiser to work around in the inner
// loops these scopes sit in. That is deliberate: a profiler that costs
// something in production is a profiler that changes the thing it measures, and
// the release path must stay exactly as fast as it measures.
//
//   cmake -S src/fpstudio -B build -DFPSTUDIO_PROFILE=ON
//   ./build/fpstudio-vkmatch ...        # prints a table at exit
//
// Counting is per thread, with no locks and no atomics in the hot path;
// each thread's totals are merged into the report when it ends.

#ifdef FPSTUDIO_PROFILE

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <mutex>
#include <string>
#include <vector>

namespace vkmatch {
namespace profile {

struct Total {
    const char *name;
    unsigned long long calls;
    unsigned long long nanoseconds;
};

// Per-thread counters as a plain array with no destructor.
//
// This was a thread_local std::vector first, and it printed an empty table: on
// the main thread the vector had already been destroyed by the time the static
// reporter ran, so there was nothing left to report and reading it at all was
// undefined. A trivially-destructible array has no such ordering to get wrong -
// it is simply still there during static destruction - and it also keeps a
// heap container out of the inner loops these scopes wrap.
//
// 64 names is far more than this matcher has; the counter below records any
// overflow so a silent undercount is not possible.
constexpr int kMaxScopes = 64;

struct ThreadTotals {
    Total entries[kMaxScopes];
    int used;
    int overflowed;
};

inline ThreadTotals &slots()
{
    static thread_local ThreadTotals t{};
    return t;
}

// The report's storage, deliberately never destroyed.
//
// These have to outlive every static and thread_local that writes to them, and
// as ordinary function-local statics they did not: the reporter is constructed
// on the first scope, the storage on the first publish, so the storage was
// destroyed first and the reporter then read freed memory. The symptom was a
// table that printed its header and no rows - reading a destroyed std::vector
// happened to report itself as non-empty and iterate as empty.
//
// Allocating once and never freeing removes the ordering question rather than
// answering it. It is a single fixed allocation per process, reachable from a
// static for the whole run, so a leak checker sees "still reachable" rather
// than a leak; and it only exists in a build that opted into profiling.
inline std::mutex &reportLock()
{
    static std::mutex *m = new std::mutex();
    return *m;
}

inline std::vector<Total> &merged()
{
    static std::vector<Total> *m = new std::vector<Total>();
    return *m;
}

inline int &overflowTotal()
{
    static int *n = new int(0);
    return *n;
}

// Folds a thread's counters into the report and zeroes them, so publishing
// twice for the same thread cannot double-count. Both the per-thread sink and
// the process-wide reporter call this, because which of them runs first is not
// something this code should have to depend on.
inline void publish(ThreadTotals &totals)
{
    std::lock_guard<std::mutex> guard(reportLock());
    for (int i = 0; i < totals.used; ++i) {
        const Total &t = totals.entries[i];
        auto it = std::find_if(merged().begin(), merged().end(),
                               [&](const Total &m) { return m.name == t.name; });
        if (it == merged().end())
            merged().push_back(t);
        else {
            it->calls += t.calls;
            it->nanoseconds += t.nanoseconds;
        }
    }
    overflowTotal() += totals.overflowed;
    totals.used = 0;
    totals.overflowed = 0;
}

inline void report()
{
    publish(slots());              // anything this thread has not handed over
    std::lock_guard<std::mutex> guard(reportLock());
    std::vector<Total> rows = merged();
    if (rows.empty())
        return;
    std::sort(rows.begin(), rows.end(),
              [](const Total &a, const Total &b) { return a.nanoseconds > b.nanoseconds; });
    unsigned long long total = 0;
    for (const Total &r : rows)
        total += r.nanoseconds;
    std::fprintf(stderr, "\n%-34s %10s %12s %10s %7s\n",
                 "scope", "calls", "total ms", "us/call", "share");
    for (const Total &r : rows)
        std::fprintf(stderr, "%-34s %10llu %12.2f %10.2f %6.1f%%\n",
                     r.name, r.calls, r.nanoseconds / 1e6,
                     r.calls ? r.nanoseconds / 1e3 / r.calls : 0.,
                     total ? 100. * r.nanoseconds / total : 0.);
    if (overflowTotal())
        std::fprintf(stderr, "%d measurement(s) dropped: more than %d distinct "
                             "scope names\n", overflowTotal(), kMaxScopes);
}

// Hands this thread's counters over when the thread ends, which is what covers
// worker threads; the main thread's are collected by the reporter instead.
struct Sink {
    ~Sink() { publish(slots()); }
};

struct Reporter {
    ~Reporter() { report(); }
};

inline void arm()
{
    // Touch the storage first so it is created before the reporter that reads
    // it. Belt and braces alongside the never-destroyed allocation above.
    (void)reportLock();
    (void)merged();
    (void)overflowTotal();
    static Reporter reporter;
    static thread_local Sink sink;
    (void)&reporter;
    (void)&sink;
}

class Scope {
public:
    explicit Scope(const char *name) : m_name(name),
        m_start(std::chrono::steady_clock::now())
    {
        arm();
    }

    ~Scope()
    {
        const auto elapsed = std::chrono::steady_clock::now() - m_start;
        const auto ns =
            (unsigned long long)std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
        ThreadTotals &t = slots();
        // Compared by pointer: every name is a string literal, so equal names
        // are the same address and this needs no hashing or comparison.
        for (int i = 0; i < t.used; ++i) {
            if (t.entries[i].name == m_name) {
                ++t.entries[i].calls;
                t.entries[i].nanoseconds += ns;
                return;
            }
        }
        if (t.used == kMaxScopes) {
            ++t.overflowed;
            return;
        }
        t.entries[t.used++] = Total{m_name, 1, ns};
    }

    Scope(const Scope &) = delete;
    Scope &operator=(const Scope &) = delete;

private:
    const char *m_name;
    std::chrono::steady_clock::time_point m_start;
};

} // namespace profile
} // namespace vkmatch

#define FPSTUDIO_PROFILE_CONCAT_(a, b) a##b
#define FPSTUDIO_PROFILE_CONCAT(a, b) FPSTUDIO_PROFILE_CONCAT_(a, b)
#define FPSTUDIO_PROFILE_SCOPE(name) \
    ::vkmatch::profile::Scope FPSTUDIO_PROFILE_CONCAT(fpstudioScope_, __LINE__)(name)

#else // FPSTUDIO_PROFILE

// Expands to nothing. Not to an empty object, not to a no-op function call -
// nothing, so that a release build is byte-for-byte what it would have been
// had these lines never been written.
#define FPSTUDIO_PROFILE_SCOPE(name) do { } while (0)

#endif // FPSTUDIO_PROFILE
