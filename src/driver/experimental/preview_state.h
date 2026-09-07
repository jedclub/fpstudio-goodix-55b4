/* Research preview only. Not a hardware finger-presence or identity proof. */
#ifndef GOODIX_PREVIEW_STATE_H
#define GOODIX_PREVIEW_STATE_H
#include <stdint.h>
#include <math.h>
#include <string.h>
#define GP_PIXELS (108 * 88)
typedef struct {
  double background[GP_PIXELS], noise, signal, raw_diff, offset;
  uint16_t previous[GP_PIXELS];
  unsigned frames, stable, on_count, off_count;
  int ready, touching;
} GoodixPreviewState;
static inline void gp_update(GoodixPreviewState *p, const uint16_t *raw) {
  double mean=0, motion=0;
  for(int i=0;i<GP_PIXELS;++i)mean+=(double)raw[i]-p->previous[i];
  mean/=GP_PIXELS;
  if(p->frames)for(int i=0;i<GP_PIXELS;++i)
    motion+=fabs((double)raw[i]-p->previous[i]-mean)/GP_PIXELS;
  memcpy(p->previous,raw,sizeof(p->previous));
  ++p->frames;
  if(!p->ready) {
    /* Let the continuous acquisition settle, then average 40 stable frames.
     * The UI must explicitly instruct the user to keep the sensor empty.
     * Stability alone cannot establish that a finger is absent. */
    if(p->frames<=20 || motion>20) {
      p->stable=0;p->noise=0;
      memset(p->background,0,sizeof(p->background));
      return;
    }
    ++p->stable;
    for(int i=0;i<GP_PIXELS;++i)p->background[i]+=(raw[i]-p->background[i])/p->stable;
    p->noise+=(motion-p->noise)/p->stable;
    if(p->stable<40)return;
    p->ready=1;
  }
  p->offset=0;p->raw_diff=0;p->signal=0;
  for(int i=0;i<GP_PIXELS;++i) {
    const double d=raw[i]-p->background[i];
    p->offset+=d/GP_PIXELS;p->raw_diff+=fabs(d)/GP_PIXELS;
  }
  /* Remove common-mode brightness drift only for presence estimation.
   * Never fold it into, or automatically replace, the calibrated background. */
  for(int i=0;i<GP_PIXELS;++i)
    p->signal+=fabs(raw[i]-p->background[i]-p->offset)/GP_PIXELS;
  const double high=fmax(40,8*p->noise),low=fmax(20,4*p->noise);
  if(!p->touching) {
    p->off_count=0;
    p->on_count=p->signal>=high?p->on_count+1:0;
    if(p->on_count>=3){p->touching=1;p->on_count=0;}
  } else {
    p->on_count=0;
    p->off_count=p->signal<=low?p->off_count+1:0;
    if(p->off_count>=3){p->touching=0;p->off_count=0;}
  }
}
#endif
