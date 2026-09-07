#!/usr/bin/env python3
"""Guided capture collection for matchlab.

Writes into captures/real/, which .gitignore already excludes - every file
this produces is biometric data.

Why it dictates a placement for each capture instead of saying "vary it":
the set is used to predict how the sensor behaves at unlock time, and unlock
time is mostly *off-centre* touches. A stack of well-centred captures would
make every candidate change score near-perfectly and predict nothing. The
plan below walks the finger across the window deliberately, so the resulting
set contains the marginal placements that actually decide the pass rate.

Usage:
    collect.py idx 20      the finger under test
    collect.py mid 12      a different finger, to measure false accepts
"""
import os
import subprocess
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROOT = os.path.dirname(ROOT)
OUT = os.path.join(ROOT, "captures", "real")
FPSTUDIO = os.path.join(ROOT, "src", "fpstudio", "build", "fpstudio")

BOLD, DIM, CYAN, GREEN, RED, RESET = (
    "\x1b[1m", "\x1b[2m", "\x1b[1;36m", "\x1b[1;32m", "\x1b[1;31m", "\x1b[0m")

# (row, col) of the finger centre in a 3x3 grid over the sensor window,
# plus a tilt in degrees. Rows: 0 top .. 2 bottom. Cols: 0 left .. 2 right.
PLAN = [
    (1, 1,   0, "중앙 - 평소 하던 대로"),
    (1, 1,   0, "중앙 - 한 번 더"),
    (0, 1,   0, "위쪽 - 손끝 방향으로 밀어서"),
    (0, 1,   0, "위쪽 - 한 번 더"),
    (2, 1,   0, "아래쪽 - 첫 마디 쪽이 닿게"),
    (2, 1,   0, "아래쪽 - 한 번 더"),
    (1, 0,   0, "왼쪽 - 손가락을 왼쪽으로"),
    (1, 0,   0, "왼쪽 - 한 번 더"),
    (1, 2,   0, "오른쪽 - 손가락을 오른쪽으로"),
    (1, 2,   0, "오른쪽 - 한 번 더"),
    (0, 0,   0, "왼쪽 위 모서리"),
    (2, 2,   0, "오른쪽 아래 모서리"),
    (0, 2,   0, "오른쪽 위 모서리"),
    (2, 0,   0, "왼쪽 아래 모서리"),
    (1, 1, -20, "중앙 - 왼쪽으로 비스듬히"),
    (1, 1, -20, "중앙 - 왼쪽으로 비스듬히, 한 번 더"),
    (1, 1,  20, "중앙 - 오른쪽으로 비스듬히"),
    (1, 1,  20, "중앙 - 오른쪽으로 비스듬히, 한 번 더"),
    (1, 1,   0, "중앙 - 평소보다 살짝 세게"),
    (1, 1,   0, "중앙 - 평소보다 살짝 가볍게"),
]


def draw(row, col, tilt):
    """The sensor window, with the spot to aim the centre of the fingertip at.

    Drawn 3 cells wide and 3 tall in the same proportions as the real window
    (about 5.5 x 4.5 mm), so 'left' on screen is 'left' on the sensor."""
    lines = []
    lines.append(f"    {DIM}지문 센서 (약 5.5 x 4.5 mm){RESET}")
    lines.append(f"    {DIM}┌───────┬───────┬───────┐{RESET}")
    for r in range(3):
        cells = []
        for c in range(3):
            if (r, c) == (row, col):
                cells.append(f"{GREEN}  ●●●  {RESET}")
            else:
                cells.append(f"{DIM}   ·   {RESET}")
        lines.append(f"    {DIM}│{RESET}" + f"{DIM}│{RESET}".join(cells) + f"{DIM}│{RESET}")
        if r < 2:
            lines.append(f"    {DIM}├───────┼───────┼───────┤{RESET}")
    lines.append(f"    {DIM}└───────┴───────┴───────┘{RESET}")

    if tilt < 0:
        lines.append(f"    {CYAN}기울기: 손가락 끝을 왼쪽으로 약 20도  ↖{RESET}")
    elif tilt > 0:
        lines.append(f"    {CYAN}기울기: 손가락 끝을 오른쪽으로 약 20도  ↗{RESET}")
    else:
        lines.append(f"    {DIM}기울기: 없음 (똑바로){RESET}")
    return "\n".join(lines)


def capture(path):
    r = subprocess.run([FPSTUDIO, "--cli", "capture", "--out", path,
                        "--timeout", "25"],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return r.returncode == 0 and os.path.exists(path) and os.path.getsize(path) > 0


def main(argv):
    if len(argv) < 2:
        sys.exit("usage: collect.py <label> <count>")
    label, count = argv[0], int(argv[1])
    os.makedirs(OUT, exist_ok=True)

    print(f"\n{BOLD}{label} - {count}회 수집{RESET}")
    print(f"{DIM}각 단계마다 '어디에 올릴지' 그림으로 나옵니다. 그대로 올렸다가 떼세요.{RESET}")
    print(f"{DIM}중단하려면 Ctrl+C.{RESET}\n")

    i = 1
    while i <= count:
        row, col, tilt, note = PLAN[(i - 1) % len(PLAN)]
        print(f"{BOLD}[{i:2d}/{count}]  {CYAN}{note}{RESET}")
        print(draw(row, col, tilt))
        print(f"\n    {BOLD}지금 올려주세요{RESET} {DIM}(25초 대기){RESET}", flush=True)

        path = os.path.join(OUT, f"{label}-{i:02d}.png")
        if capture(path):
            print(f"    {GREEN}저장됨{RESET}  ->  {os.path.basename(path)}\n")
            i += 1
        else:
            if os.path.exists(path):
                os.remove(path)
            print(f"    {RED}손가락을 못 봤습니다 - 같은 위치로 다시{RESET}\n")
        print(f"    {DIM}손가락을 떼주세요{RESET}", flush=True)
        time.sleep(1.5)

    n = len([f for f in os.listdir(OUT) if f.startswith(label + "-")])
    print(f"{GREEN}{BOLD}완료: {label} {n}장{RESET}  ({OUT})")


if __name__ == "__main__":
    main(sys.argv[1:])
