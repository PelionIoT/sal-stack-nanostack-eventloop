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
