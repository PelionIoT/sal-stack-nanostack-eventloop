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
#include "ns_types.h"


//EventOS functions
/*
void eventOS_scheduler_init(void) {}

bool eventOS_scheduler_dispatch_event(void)
{
    return false;
}

void eventOS_scheduler_run_until_idle(void) {}

void eventOS_scheduler_run(void) {}

int eventOS_scheduler_timer_stop(void)
{
    return 0;
}

int eventOS_scheduler_timer_synch_after_sleep(uint32_t sleep_ticks)
{
    return 0;
}

int8_t eventOS_scheduler_get_active_tasklet(void)
{
    return 0;
}

void eventOS_scheduler_set_active_tasklet(int8_t tasklet){}

void eventOS_scheduler_wait(void){}*/

void eventOS_scheduler_idle(void){}

void eventOS_scheduler_signal(void){}
/*
uint32_t eventOS_scheduler_sleep(uint32_t sleep_time_ms)
{
    return 0;
}

void eventOS_scheduler_mutex_wait(void){}

void eventOS_scheduler_mutex_release(void){}

bool eventOS_scheduler_mutex_am_owner(void)
{
    return false;
}*/
