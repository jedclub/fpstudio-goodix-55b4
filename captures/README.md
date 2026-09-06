# captures

**These are synthetic.** No real fingerprint is committed to this repository,
and none should be — a captured frame is biometric data, and a template is
derived directly from it. `.gitignore` keeps this directory empty of anything
the sensor actually produced.

What is here is a generated ridge field, put through the same two failures the
driver had, so the effects can be shown without showing anyone's finger. The
generator is thirty lines of Python in the commit that added these files.

To make your own from the real sensor:

```
fpstudio --cli capture --out frame.pgm
```

## What the two pairs show

### Row length

| | |
|---|---|
| `ridges-correct-geometry.png` | cut at 108 — the length the sensor actually emits |
| `ridges-wrong-geometry.png` | cut at 88 — this repo's patch `0006`, since reverted |

The pixel count is identical either way (108 × 88 = 88 × 108 = 9504). What
changes is only where each row of the linear stream is cut, and cutting at the
wrong length shears the image a little further on every row. The result is the
horizontal banding in the second image — which is exactly what the sensor
appeared to be producing, and why verification scored 0 against every enrolled
sample.

Two sources said 88: `goodix-fp-dump` and the vendor's own `Wbdi.dll` table.
Both are right about the die and wrong about this driver, because the sensor
scans the window the uploaded MCU config asks for, and that config is the
GF3268's. See `docs/05-geometry-regression.md`.

### Normalisation

| | |
|---|---|
| `normalisation-minmax.png` | `squash_frame_linear` as it shipped |
| `normalisation-percentile.png` | patch `0009` |

A touch never covers the whole window, so the corners stay near zero. Min/max
therefore pins the black point to untouched area and the ridges end up
compressed into the top of the range. Measured over five real captures the
output spanned 0–255 nominally, but the 1st percentile was 0 in every one while
the median sat at 122–189: roughly half the range spent on dead area.

That costs matches. SIFT's detector drops low-contrast extrema, so squeezing
the ridges halves the keypoints worth having. Re-scoring two overlapping
captures with the percentile stretch instead took the score from 5 to 63
against a threshold of 72 — same images, same matcher, only the normalisation
changed.
