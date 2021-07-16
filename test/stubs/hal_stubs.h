#ifndef HAL_STUBS_H
#define HAL_STUBS_H

#include <inttypes.h>
#include "arm_hal_timer.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef NS_EXCLUDE_HIGHRES_TIMER
typedef void (*platform_timer_cb)(void);
#endif

typedef struct {
    int16_t uint16_value;
    platform_timer_cb cb;
} hal_stub_def;

extern hal_stub_def hal_stub;

#ifdef __cplusplus
}
#endif

#endif // HAL_STUBS_H
