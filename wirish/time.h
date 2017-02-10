#ifndef _WIRISH_WIRISH_TIME_H_
#define _WIRISH_WIRISH_TIME_H_

#include <libmaple/libmaple_types.h>

#include <xtimer.h>

static inline uint32 millis(void) {
  return xtimer_now_usec()/1000;
}

static inline uint32 micros(void) {
  return xtimer_now_usec();
}

static inline void delay(unsigned long ms) {
  xtimer_usleep(ms*1000);
}

static inline void delayMicroseconds(uint32 us) {
  xtimer_usleep(us);
}

#endif
