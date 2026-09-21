#pragma once

// Timer slack is how long the kernel may sit on an expiring timer so it can
// batch wakeups and let the CPU stay asleep. It is inherited across fork and
// exec, so a process is handed whatever its session was given and has no way to
// notice it happened.
//
// This desktop hands out 100ms against a kernel default of 50us. Measured with
// a bare 30ms Qt timer and nothing else running: 60.2ms between ticks, worst
// case 130.1ms. After asking for the kernel's own default back: 30.2ms median,
// 30.6ms worst. The sensor produces frames faster than 60ms, and the preview is
// a single file the driver overwrites in place, so frames were being replaced
// before they had ever been read - the capture was not slow, it was blind every
// other frame.
//
// Both ends of that pipe need it. A reader that wakes on time still sees half
// the frames if the writer does not, so this is called by the application, by
// the test harness that stands in for it, and by the fake capture that stands
// in for the driver.
//
// Only the calling process is affected, and only while it runs. Nothing
// system-wide is touched and nothing needs undoing at exit. A kernel that does
// not support the call reports failure, which is not worth acting on: capture
// still works, it just runs at the rate it ran at before.

#if defined(__linux__)
#include <sys/prctl.h>
#endif

namespace fpstudio {

inline void tightenTimerSlack()
{
#if defined(__linux__) && defined(PR_SET_TIMERSLACK)
    prctl(PR_SET_TIMERSLACK, 50000UL, 0UL, 0UL, 0UL);
#endif
}

}   // namespace fpstudio
