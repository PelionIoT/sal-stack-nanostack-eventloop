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

#include "nanostack_eventloop.h"
#include "eventOS_scheduler.h"
#ifndef NANOSTACK_USE_PAL_FUNCTION_POINTERS
#include "platform/arm_hal_timer.h"
#include "platform/arm_hal_interrupt.h"
#endif
#include <assert.h>


void nanostack_eventOS_scheduler_init(platform_enter_critical_fptr pl_enter_critical, platform_exit_critical_fptr pl_exit_critical, platform_tick_timer_start_fptr pl_tick_timer_start,
                                      platform_tick_timer_stop_fptr pl_tick_timer_stop, platform_tick_timer_register_fptr pl_tick_timer_register)
{
}

void nanostack_enter_critical()
{
}

void nanostack_exit_critical()
{
}

int8_t nanostack_platform_tick_timer_register(void (*tick_timer_cb_handler)(void))
{
    return 0;
}

int8_t nanostack_platform_tick_timer_start(uint32_t period_ms)
{
    return 0;
}

int8_t nanostack_platform_tick_timer_stop()
{
    return 0;
}
