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
#ifndef HAL_STUBS_H
#define HAL_STUBS_H

#include <inttypes.h>
#include "arm_hal_timer.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef NS_EXCLUDE_HIGHRES_TIMER
typedef void (*platform_timer_cb)(void);
#endif

typedef struct {
    int16_t uint16_value;
    platform_timer_cb cb;
} hal_stub_def;

extern hal_stub_def hal_stub;

#ifdef __cplusplus
}
#endif

#endif // HAL_STUBS_H
