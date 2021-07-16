/*
 * Copyright (c) 2014-2021, Pelion and affiliates.
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
#include "ns_types.h"
#include "ns_list.h"
#include "timer_sys.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/arm_hal_timer.h"
#include "nsdynmemLIB.h"
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "event.h"
#include "eventOS_callback_timer.h"
#include "nanostack_eventloop.h"

#include "ns_timer.h"
#include "system_timer_stub.h"

#ifndef ST_MAX
#define ST_MAX 6
#endif

system_timer_stub_def system_timer_stub;

#ifndef NS_EVENTLOOP_USE_TICK_TIMER
int8_t platform_tick_timer_register(void (*tick_timer_cb)(void))
{
    return system_timer_stub.int8_value;
}

int8_t platform_tick_timer_start(uint32_t period_ms)
{
    return system_timer_stub.int8_value;
}

int8_t platform_tick_timer_stop(void)
{
    return system_timer_stub.int8_value;
}
#endif // !NS_EVENTLOOP_USE_TICK_TIMER

void timer_sys_init(void)
{
}

void timer_sys_disable(void)
{
}

int8_t timer_sys_wakeup(void)
{
    return system_timer_stub.int8_value;
}

void timer_sys_event_free(arm_event_storage_t *event)
{
}

void timer_sys_event_cancel_critical(struct arm_event_storage *event)
{
}

uint32_t eventOS_event_timer_ticks(void)
{
    return system_timer_stub.uint32_value;
}

arm_event_storage_t *eventOS_event_timer_request_at(const arm_event_t *event, uint32_t at)
{
    return system_timer_stub.storage;
}

arm_event_storage_t *eventOS_event_timer_request_in(const arm_event_t *event, int32_t in)
{
    return system_timer_stub.storage;
}

arm_event_storage_t *eventOS_event_timer_request_every(const arm_event_t *event, int32_t period)
{
    return system_timer_stub.storage;
}

int8_t eventOS_event_timer_request(uint8_t event_id, uint8_t event_type, int8_t tasklet_id, uint32_t time)
{
    return system_timer_stub.int8_value;
}

int8_t eventOS_event_timer_cancel(uint8_t event_id, int8_t tasklet_id)
{
    return system_timer_stub.int8_value;
}

uint32_t eventOS_event_timer_shortest_active_timer(void)
{
    return system_timer_stub.uint32_value;
}

void system_timer_tick_update(uint32_t ticks)
{
}

