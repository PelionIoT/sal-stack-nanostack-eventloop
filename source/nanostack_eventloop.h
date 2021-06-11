/*
 * Copyright (c) 2021, Pelion and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef NANOSTACK_EVENTLOOP_H_
#define NANOSTACK_EVENTLOOP_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* This collection of function exists to tackle the circular dependency between sal-stack-nanostack-eventloop and ns-hal-pal (or other hal implementations)
   Needed hal functions are given as function pointers and if define NANOSTACK_USE_PAL_FUNCTION_POINTERS is defined then function pointers are used. Otherwise
   original platform function are called. See Function documention from original functions. */

typedef void (*platform_enter_critical_fptr)(void);
typedef void (*platform_exit_critical_fptr)(void);

typedef int8_t (*platform_tick_timer_start_fptr)(uint32_t);
typedef int8_t (*platform_tick_timer_stop_fptr)(void);
typedef int8_t (*platform_tick_timer_register_fptr)(void (*tick_timer_cb_handler)(void));

typedef struct ns_func_pointers_t
{
	platform_enter_critical_fptr pl_enter_critical;
    platform_exit_critical_fptr pl_exit_critical;
    platform_tick_timer_start_fptr pl_tick_timer_start;
    platform_tick_timer_stop_fptr pl_tick_timer_stop;
    platform_tick_timer_register_fptr pl_tick_timer_register;
} ns_func_pointers_t;

void nanostack_eventOS_scheduler_init(platform_enter_critical_fptr pl_enter_critical, platform_exit_critical_fptr pl_exit_critical, platform_tick_timer_start_fptr pl_tick_timer_start,
                                      platform_tick_timer_stop_fptr pl_tick_timer_stop, platform_tick_timer_register_fptr pl_tick_timer_register);

void nanostack_enter_critical();

void nanostack_exit_critical();

int8_t nanostack_platform_tick_timer_register(void (*tick_timer_cb_handler)(void));

int8_t nanostack_platform_tick_timer_start(uint32_t period_ms);

int8_t nanostack_platform_tick_timer_stop();

#ifdef __cplusplus
}
#endif

#endif /*NANOSTACK_EVENTLOOP_H_*/
