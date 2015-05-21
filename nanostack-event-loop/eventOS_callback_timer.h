/*
 * Copyright (c) 2014 ARM. All rights reserved.
 */
#ifndef EVENTOS_CALLBACK_TIMER_H_
#define EVENTOS_CALLBACK_TIMER_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "ns_types.h"

extern int8_t eventOS_callback_divide_resolution(uint16_t divider);
extern int8_t eventOS_callback_timer_register(void (*timer_interrupt_handler)(int8_t, uint16_t));
extern int8_t eventOS_callback_timer_unregister(int8_t ns_timer_id);

extern int8_t eventOS_callback_timer_stop(int8_t ns_timer_id);
extern int8_t eventOS_callback_timer_start(int8_t ns_timer_id, uint16_t slots);
#ifdef __cplusplus
}
#endif

#endif /* EVENTOS_CALLBACK_TIMER_H_ */
