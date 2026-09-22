#!/bin/bash
# Build FPStudio with profile-guided optimisation.
#
# Three stages: compile with counters, run something representative, compile
# again against what was counted. The middle stage is the whole point - a
# profile from an unrepresentative run makes the result worse than no profile
# at all, because the compiler will confidently lay out the wrong branches.
#
# The training here is the test suite plus a synthetic matcher sweep. That is
# chosen so it needs no fingerprint data: the matcher's hot paths are the same
# whether the ridges came from a sensor or from a sine function, and a training
# run that required biometric samples could not run in CI and would not be run
# often enough to stay current.
#
#   tools/pgo-build.sh [output-build-dir]
#
# Leaves the profile in src/fpstudio/pgo-data so a later rebuild can reuse it
# without retraining.
set -uo pipefail

REPO="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")/.." && pwd)"
SOURCE="$REPO/src/fpstudio"
GEN="$SOURCE/build-pgo-generate"
OUT="${1:-$SOURCE/build-pgo}"
PROFILE="$SOURCE/pgo-data"
JOBS="$(nproc 2>/dev/null || echo 4)"

step() { printf '\n== %s ==\n' "$1"; }
die()  { printf '!! %s\n' "$1" >&2; exit 1; }

step "1/4 계측 빌드 구성"
rm -rf "$GEN" "$PROFILE"
cmake -S "$SOURCE" -B "$GEN" -DCMAKE_BUILD_TYPE=Release \
      -DFPSTUDIO_PGO=GENERATE -DFPSTUDIO_PGO_DIR="$PROFILE" >/dev/null \
      || die "구성 실패"
cmake --build "$GEN" -j"$JOBS" >/dev/null || die "계측 빌드 실패"

step "2/4 훈련 실행"
# The suite exercises the PAM conversation, the frame selector, the placement
# search and the Vulkan matcher end to end. Failures here are not fatal to the
# profile - a test that fails still records the paths it took - but they are
# worth seeing, because training against broken code trains the wrong thing.
if ! ctest --test-dir "$GEN" --output-on-failure >/dev/null 2>&1; then
    printf '   주의: 훈련 중 일부 테스트가 실패했습니다 (프로파일은 계속 수집됩니다)\n'
fi
# A bounded matcher sweep over synthetic ridges, which is where the release
# actually spends its time.
TRAIN="$(mktemp -d)"
trap 'rm -rf "$TRAIN"' EXIT
if python3 - "$TRAIN" <<'PY'
import math, struct, sys, pathlib
out = pathlib.Path(sys.argv[1])
def ridges(shift, angle):
    c, s = math.cos(angle), math.sin(angle)
    rows = []
    for y in range(88):
        row = bytearray()
        for x in range(108):
            u = (x - 54) * c - (y - 44) * s + shift
            v = (x - 54) * s + (y - 44) * c
            value = 128 + 90 * math.sin(u * 0.62 + 0.004 * v * v) + 20 * math.sin(v * 0.21)
            row.append(max(0, min(255, int(value))))
        rows.append(bytes(row))
    return b"P5\n108 88\n255\n" + b"".join(rows)
for i in range(6):
    (out / f"ref-{i}.pgm").write_bytes(ridges(i * 3.0, i * 0.04))
(out / "probe.pgm").write_bytes(ridges(2.0, 0.02))
PY
then
    VK="$GEN/fpstudio-vkmatch"
    SPV="$GEN/vkmatch/fpstudio-match.spv"
    if [ -x "$VK" ] && [ -f "$SPV" ]; then
        args=(--shader "$SPV" --auth-search)
        for r in "$TRAIN"/ref-*.pgm; do args+=(--reference "$r"); done
        for _ in 1 2 3; do
            "$VK" "${args[@]}" --probe "$TRAIN/probe.pgm" >/dev/null 2>&1
        done
        printf '   합성 매처 학습 3회 완료\n'
    else
        printf '   주의: 매처 실행 파일이 없어 합성 학습을 건너뜁니다\n'
    fi
fi

count=$(find "$PROFILE" -name '*.gcda' 2>/dev/null | wc -l)
[ "$count" -gt 0 ] || die "프로파일이 하나도 생성되지 않았습니다"
printf '   프로파일 %s개 수집: %s\n' "$count" "$PROFILE"

step "3/4 프로파일 적용 빌드"
rm -rf "$OUT"
cmake -S "$SOURCE" -B "$OUT" -DCMAKE_BUILD_TYPE=Release \
      -DFPSTUDIO_PGO=USE -DFPSTUDIO_PGO_DIR="$PROFILE" >/dev/null \
      || die "구성 실패"
cmake --build "$OUT" -j"$JOBS" >/dev/null || die "최적화 빌드 실패"

step "4/4 검증"
ctest --test-dir "$OUT" >/dev/null 2>&1 \
    && printf '   테스트 통과\n' \
    || die "PGO 빌드가 테스트를 통과하지 못했습니다 - 배포하지 마세요"

printf '\n완료: %s\n프로파일: %s (재훈련 없이 재사용 가능)\n' "$OUT" "$PROFILE"
