#!/bin/sh
# OpenCV 5 on this system installs its headers under /usr/include/opencv5 but
# still uses the opencv2/ include prefix; -isystem keeps its own warnings out
# of ours.
set -e
CV=$(pkg-config --cflags --libs opencv5)
CV=$(printf '%s' "$CV" | sed 's|-I/|-isystem /|')
# shellcheck disable=SC2086
GL=$(pkg-config --cflags --libs gio-2.0 glib-2.0)
g++ -O2 -std=c++17 -Wall -I. matchlab.cpp printload.cpp -o matchlab $CV $GL
# shellcheck disable=SC2086
g++ -O2 -std=c++17 -Wall -DMATCHLAB_NO_MAIN -I. selftest.cpp matchlab.cpp printload.cpp upstream/sigfm.cpp -o selftest $CV $GL
g++ -O2 -std=c++17 -Wall -I. dumpprint.cpp printload.cpp -o dumpprint $CV $GL
echo "built matchlab, selftest, dumpprint"
# Optional exact patched driver source, to validate the fixed A/B variant.
if [ "$#" -eq 1 ]; then
    g++ -O2 -std=c++17 -Wall -DMATCHLAB_NO_MAIN -DMATCHLAB_TEST_FIXED -I. selftest.cpp matchlab.cpp printload.cpp "$1" -o selftest-fixed $CV $GL
    echo "built selftest-fixed against supplied driver source"
fi
