/*
 * Copyright (c) 2014-2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Include before mbed.h to properly get UINT*_C()

#include "ns_types.h"

#include "platform/arm_hal_timer.h"
#include "platform/arm_hal_interrupt.h"

#if defined(NS_EVENTLOOP_USE_TICK_TIMER) && defined(YOTTA_CFG_MINAR)

#include "minar/minar.h"
#include "mbed-drivers/mbed.h"
#include "core-util/FunctionPointer.h"
#include "core-util/Event.h"

#define TICK_TIMER_ID   1

using minar::Scheduler;
using minar::milliseconds;
using minar::callback_handle_t;
using namespace mbed::util;

static callback_handle_t sys_timer_handle;
static void (*tick_timer_callback)(void);

void timer_callback(void const *funcArgument)
{
    (void)funcArgument;
    if (NULL != tick_timer_callback) {
        tick_timer_callback();
    }
}

// Low precision platform tick timer
int8_t platform_tick_timer_register(void (*tick_timer_cb_handler)(void))
{
    tick_timer_callback = tick_timer_cb_handler;
    return TICK_TIMER_ID;
}

int8_t platform_tick_timer_start(uint32_t period_ms)
{
    int8_t retval = -1;
    if (sys_timer_handle != NULL) {
        return 0; // Timer already started already so return success
    }
    Event e = FunctionPointer1<void, void const *>(timer_callback).bind(NULL);
    if (e != NULL) {
        sys_timer_handle = Scheduler::postCallback(e).period(milliseconds(period_ms)).getHandle();
        if (sys_timer_handle != NULL) {
            retval = 0;
        }
    }
    return retval;
}

int8_t platform_tick_timer_stop(void)
{
    int8_t retval = -1;
    if (sys_timer_handle != NULL) {
        Scheduler::cancelCallback(sys_timer_handle);
        sys_timer_handle = NULL;
        retval = 0;
    }
    return retval;
}

#endif // defined(NS_EVENTLOOP_USE_TICK_TIMER) && defined(YOTTA_CFG)
