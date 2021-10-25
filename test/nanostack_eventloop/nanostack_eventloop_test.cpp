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
#include "gtest/gtest.h"
#include "ns_types.h"
#include "nanostack_eventloop.h"

void platform_enter_critical_f() {}
void platform_exit_critical_f() {}

int8_t platform_tick_timer_start_f(uint32_t a)
{
    return 0;
}
int8_t platform_tick_timer_stop_f()
{
    return 0;
}
int8_t platform_tick_timer_register_f(void (*tick_timer_cb_handler)(void))
{
    return 0;
}


class nanostack_eventloop_test : public testing::Test {
protected:
    void SetUp()
    {
        nanostack_eventOS_scheduler_init(platform_enter_critical_f, platform_exit_critical_f, platform_tick_timer_start_f,
                                         platform_tick_timer_stop_f, platform_tick_timer_register_f);
    }

    void TearDown()
    {
    }
};

TEST_F(nanostack_eventloop_test, nanostack_enter_critical)
{
    nanostack_enter_critical();
}

TEST_F(nanostack_eventloop_test, nanostack_exit_critical)
{
    nanostack_exit_critical();
}

static void timer_cb() {}

TEST_F(nanostack_eventloop_test, nanostack_platform_tick_timer_register)
{
    nanostack_platform_tick_timer_register(timer_cb);
}

TEST_F(nanostack_eventloop_test, nanostack_platform_tick_timer_start)
{
    nanostack_platform_tick_timer_start(5);
}

TEST_F(nanostack_eventloop_test, nanostack_platform_tick_timer_stop)
{
    nanostack_platform_tick_timer_stop();
}
