#!/usr/bin/env bash
# Put fprintd's enrolment on screen, using fpstudio's banner.
#
# fprintd-enroll is a separate program: it talks to the fprintd daemon over
# D-Bus and knows nothing about fpstudio, so the window stays blank while it
# waits for a finger. That is the wrong way round - the person at the sensor is
# the one who needs to be told what to do, and fprintd tells only the terminal.
#
# The banner is driven by a JSON file (see beacon.h), so anything can publish
# to it. This wrapper runs fprintd-enroll, translates each `Enroll result:` line
# into an instruction, and clears the beacon when it exits.
#
# The pid written is this script's own. The window drops a banner whose pid has
# gone (see MainWindow::onBeaconChanged), which is what makes an interrupted
# run - Ctrl-C, a killed terminal - clean up after itself even though the trap
# below cannot run in every case.
#
# Messages come from fpstudio, not from here. Duplicating the eleven
# translations in shell would guarantee they drift from the ones the window
# already ships, so this asks the binary for each string: `fpstudio --tr KEY`
# resolves it through the same catalogue, in the same language, chosen the same
# way. One source of truth, and a new language needs nothing changed here.
#
# Usage: fprintd-beacon.sh [finger]        default: right-index-finger
#        fprintd-beacon.sh --verify        verify instead of enrol
#        FPSTUDIO_LANG=de fprintd-beacon.sh   force a language

set -uo pipefail
BEACON=/tmp/fpstudio-status.json
FINGER=${1:-right-index-finger}

# The binary beside this script, so a build tree and an install both work.
FPSTUDIO=${FPSTUDIO:-$(dirname "$(readlink -f "$0")")/build/fpstudio}
[ -x "$FPSTUDIO" ] || FPSTUDIO=$(command -v fpstudio || echo "")
LANGOPT=()
[ -n "${FPSTUDIO_LANG:-}" ] && LANGOPT=(--lang "$FPSTUDIO_LANG")

# Falls back to the English key itself if the binary is missing, so the script
# still says something useful rather than printing empty banners.
t() {
    if [ -n "$FPSTUDIO" ]; then
        "$FPSTUDIO" "${LANGOPT[@]}" --tr "$1" 2>/dev/null || printf '%s' "$1"
    else
        printf '%s' "$1"
    fi
}

emit() {  # stage, prompt, status
    local tmp="${BEACON}.$$"
    printf '{"pid":%d,"source":"fprintd","stage":"%s","prompt":"%s","status":"%s","started_at":%d,"ts":%d,"coverage":-1,"sharpness":-1,"quality_ok":true}\n' \
        "$$" "$1" "$2" "$3" "$START" "$(( $(date +%s) * 1000 ))" > "$tmp"
    # Rename rather than write in place: a reader must never see half a file.
    mv -f "$tmp" "$BEACON" 2>/dev/null
    chmod 0644 "$BEACON" 2>/dev/null
}

clear_beacon() {
    printf '{"pid":0,"source":"fprintd","stage":"idle","prompt":"","status":"","started_at":0,"ts":%d,"coverage":-1,"sharpness":-1,"quality_ok":true}\n' \
        "$(( $(date +%s) * 1000 ))" > "$BEACON" 2>/dev/null
}
trap clear_beacon EXIT INT TERM

START=$(( $(date +%s) * 1000 ))

if [ "$FINGER" = "--verify" ]; then
    PROMPT="$(t "Put the enrolled finger on the sensor")"
    emit activated "$PROMPT" "$(t "Sensor ready")"
    CMD=(fprintd-verify)
else
    PROMPT="$(t "Enrolling — press and lift your finger repeatedly")"
    emit activated "$PROMPT" "$(t "Sensor ready")"
    CMD=(fprintd-enroll -f "$FINGER")
fi

# The driver decides how many stages there are, so ask rather than assume -
# this repo has already moved it from 10 to 15 once.
STAGES=$([ -n "$FPSTUDIO" ] && "$FPSTUDIO" --cli devices 2>/dev/null \
         | sed -n 's/.*"enroll_stages":\([0-9]*\).*/\1/p' | head -1)
STAGES=${STAGES:-?}

passed=0
# stdbuf so each line reaches the loop as fprintd prints it; without it the
# pipe buffers and the banner updates only after the run is over, which is
# exactly when nobody needs it.
stdbuf -oL -eL "${CMD[@]}" 2>&1 | while IFS= read -r line; do
    printf '%s\n' "$line"
    case "$line" in
        *enroll-stage-passed*)
            passed=$((passed + 1))
            emit progress "$PROMPT" "$(t "Enrol %1/%2 — lift your finger and press again" | sed "s/%1/${passed}/; s|%2|${STAGES}|")" ;;
        *enroll-finger-not-centered*)
            emit rejected "$PROMPT" "$(t "Only part of the sensor was touched — cover more of it, centred")" ;;
        *enroll-retry-scan*|*enroll-swipe-too-short*)
            emit rejected "$PROMPT" "$(t "The ridges are faint — press a little harder")" ;;
        *enroll-remove-and-retry*)
            emit rejected "$PROMPT" "$(t "Finger lifted")" ;;
        *enroll-completed*)
            emit done "" "$(t "Enrolment complete")" ;;
        *enroll-failed*|*enroll-disconnected*)
            emit failed "" "$(t "Failed")" ;;
        *verify-match*)
            emit done "" "$(t "Match   %1 / %2" | sed "s| *%1 */ *%2||")" ;;
        *verify-no-match*)
            emit failed "" "$(t "No match   %1 / %2" | sed "s| *%1 */ *%2||")" ;;
        *verify-retry-scan*|*verify-swipe-too-short*|*verify-finger-not-centered*)
            emit rejected "$PROMPT" "$(t "Put the enrolled finger on the sensor")" ;;
    esac
done
