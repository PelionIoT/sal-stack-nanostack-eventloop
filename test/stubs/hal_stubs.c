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
#include "arm_hal_timer.h"
#include "hal_stubs.h"

hal_stub_def hal_stub;

void platform_enter_critical() {}

void platform_exit_critical() {}

void platform_timer_set_cb(platform_timer_cb new_fp)
{
    hal_stub.cb = new_fp;
}

void platform_timer_enable(void) {}

void platform_timer_disable(void) {}

void platform_timer_start(uint16_t slots) {}

uint16_t platform_timer_get_remaining_slots(void)
{
    return hal_stub.uint16_value;
}


