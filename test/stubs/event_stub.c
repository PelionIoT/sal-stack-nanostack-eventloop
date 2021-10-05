/*
 * Copyright 2020-2021 Pelion.
 *
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
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "timer_sys.h"
#include "nsdynmemLIB.h"
#include "ns_timer.h"
#include "event.h"
#include "nanostack_eventloop.h"
#include "event_stub.h"

event_stub_def event_stub;

bool event_tasklet_handler_id_valid(uint8_t tasklet_id)
{
    return event_stub.bool_value;
}

int8_t eventOS_event_handler_create(void (*handler_func_ptr)(arm_event_s *), uint8_t init_event_type)
{
    event_stub.event_handler_cb = handler_func_ptr;
    return event_stub.int8_value;
}

int8_t eventOS_event_send(const arm_event_t *event)
{
    return event_stub.int8_value;
}

void eventOS_event_send_user_allocated(arm_event_storage_t *event)
{
}

void eventOS_event_send_timer_allocated(arm_event_storage_t *event)
{
}

void eventOS_event_cancel_critical(arm_event_storage_t *event)
{
}

arm_event_storage_t *event_core_get(void)
{
    return NULL;
}

void event_core_free_push(arm_event_storage_t *free)
{
}

void event_core_write(arm_event_storage_t *event)
{
}

arm_event_storage_t *eventOS_event_find_by_id_critical(uint8_t tasklet_id, uint8_t event_id)
{
    return NULL;
}

void eventOS_scheduler_init(void)
{
}

int8_t eventOS_scheduler_get_active_tasklet(void)
{
    return event_stub.int8_value;
}

void eventOS_scheduler_set_active_tasklet(int8_t tasklet)
{
}

int eventOS_scheduler_timer_stop(void)
{
    return event_stub.int_value;
}

int eventOS_scheduler_timer_synch_after_sleep(uint32_t sleep_ticks)
{
    return event_stub.int_value;
}

bool eventOS_scheduler_dispatch_event(void)
{
    return event_stub.bool_value;
}

void eventOS_scheduler_run_until_idle(void)
{
}

void eventOS_scheduler_run(void)
{
    while(1);
}

void eventOS_cancel(arm_event_storage_t *event)
{
}
