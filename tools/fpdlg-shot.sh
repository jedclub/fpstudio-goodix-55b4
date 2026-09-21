#!/bin/bash
# Capture the authentication dialog exactly as it is on screen right now.
#
# The KDE authentication window is sometimes drawn with its lower section
# clipped - the buttons below the password field are not reachable. It has not
# reproduced on demand: dialogs captured deliberately came out whole, and the
# leading explanation, that a long fingerprint status line wraps and forces the
# window to grow after it was laid out, is ruled out by measurement. The
# longest status line renders at 314px in the desktop's UI font against about
# 424px of room.
#
# So what is missing is the broken frame itself rather than another theory
# about it. Leave the dialog open, run this, and keep the two files: a
# full-screen capture, and the PAM and polkit records from either side of it.
#
# Nothing here is privileged and nothing is sent anywhere. The capture is a
# picture of your screen, so treat it as you would any screenshot.
set -uo pipefail

out="${1:-$HOME/fpdlg-$(date +%Y%m%d-%H%M%S)}"

if ! command -v spectacle >/dev/null 2>&1; then
    echo "spectacle 이 필요합니다 (pacman -S spectacle)" >&2
    exit 1
fi
if ! spectacle -b -n -f -o "$out.png" >/dev/null 2>&1; then
    echo "화면 캡처에 실패했습니다" >&2
    exit 1
fi

{
    echo "시각: $(date -Is)"
    echo "세션: ${XDG_SESSION_TYPE:-unknown}  스케일: ${QT_SCALE_FACTOR:-1}"
    echo
    echo "-- 인증 에이전트 / PAM 기록 (최근 2분) --"
    journalctl --since "2 min ago" --no-pager 2>/dev/null |
        grep -iE "polkit-kde|pam_fpstudio|fpstudio_auth" | tail -40
    echo
    echo "-- 지문 매처 판정 (최근 2분) --"
    journalctl --since "2 min ago" --no-pager 2>/dev/null |
        grep -o "gpu_metrics .*" | tail -10
} > "$out.txt" 2>&1

chmod 600 "$out.png" "$out.txt"
echo "저장: $out.png"
echo "      $out.txt"
