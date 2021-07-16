#ifndef SYSTEM_TIMER_STUB_H
#define SYSTEM_TIMER_STUB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "eventOS_event.h"

typedef struct {
    int8_t int8_value;
    uint32_t uint32_value;
    arm_event_storage_t *storage;
} system_timer_stub_def;

extern system_timer_stub_def system_timer_stub;

#ifdef __cplusplus
}
#endif

#endif // SYSTEM_TIMER_STUB_H
