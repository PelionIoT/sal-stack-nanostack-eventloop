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

#include "eventOS_callback_timer.h"
#include "hal_stubs.h"
#include "nsdynmemLIB_stub.h"
#include "ns_timer.h"

class ns_timer_test : public testing::Test {
protected:
    void SetUp()
    {
        nsdynmemlib_stub.returnCounter = 0;
        hal_stub.uint16_value = 0;
    }

    void TearDown()
    {
    }
};

void timer_interrupt_handler(int8_t a, uint16_t b){}

#ifndef NS_EXCLUDE_HIGHRES_TIMER
TEST_F(ns_timer_test, eventOS_callback_timer_register_unregister)
{
    ASSERT_EQ(-1, eventOS_callback_timer_register(NULL));

    nsdynmemlib_stub.returnCounter = 0;
    ASSERT_EQ(-1, eventOS_callback_timer_register(timer_interrupt_handler));

    nsdynmemlib_stub.returnCounter = 1;
    ASSERT_EQ(0, eventOS_callback_timer_register(timer_interrupt_handler));

    for (uint8_t i = 1; i <= INT8_MAX; i++) {
        nsdynmemlib_stub.returnCounter = 1;
        ASSERT_EQ(i, eventOS_callback_timer_register(timer_interrupt_handler));
    }
    nsdynmemlib_stub.returnCounter = 0; //no need to allocate as case returns before that
    ASSERT_EQ(-1, eventOS_callback_timer_register(timer_interrupt_handler));

    //Clean up everything
    for (int8_t i = INT8_MAX; i >= 0; i--) {
        ASSERT_EQ(0, eventOS_callback_timer_unregister(i));
    }
    ASSERT_EQ(-1, eventOS_callback_timer_unregister(0));
}

TEST_F(ns_timer_test, ns_timer_sleep)
{
    ASSERT_EQ(-1, ns_timer_sleep());

    //We need to create a timer and start it
    nsdynmemlib_stub.returnCounter = 1;
    ASSERT_EQ(0, eventOS_callback_timer_register(timer_interrupt_handler));
    ASSERT_EQ(0, eventOS_callback_timer_start(0, 0));

    ASSERT_EQ(0, ns_timer_sleep());

    ASSERT_EQ(0, eventOS_callback_timer_unregister(0));
}

TEST_F(ns_timer_test, eventOS_callback_timer_start_and_stop)
{
    ASSERT_EQ(-1, eventOS_callback_timer_start(0, 0));

    //Let's create 3 timers
    nsdynmemlib_stub.returnCounter = 3;
    ASSERT_EQ(0, eventOS_callback_timer_register(timer_interrupt_handler));
    ASSERT_EQ(1, eventOS_callback_timer_register(timer_interrupt_handler));
    ASSERT_EQ(2, eventOS_callback_timer_register(timer_interrupt_handler));

    ASSERT_EQ(0, eventOS_callback_timer_start(0, 0));
    hal_stub.uint16_value = 1;
    ASSERT_EQ(0, eventOS_callback_timer_start(0, 0));
    //start another
    ASSERT_EQ(0, eventOS_callback_timer_start(1, 1));

    ASSERT_EQ(0, eventOS_callback_timer_start(2, 0));

    ASSERT_EQ(0, eventOS_callback_timer_start(2, 4));
    if (hal_stub.cb) {
        hal_stub.cb();
    }

    ASSERT_EQ(0, eventOS_callback_timer_start(1, 2));

    if (hal_stub.cb) {
        hal_stub.cb();
    }
    //Let's stop the timers
    ASSERT_EQ(0, eventOS_callback_timer_stop(0));
    ASSERT_EQ(0, eventOS_callback_timer_stop(0)); //already stopped

    ASSERT_EQ(0, eventOS_callback_timer_start(0, 4));

    ASSERT_EQ(0, eventOS_callback_timer_stop(1));
    ASSERT_EQ(0, eventOS_callback_timer_stop(2));
    ASSERT_EQ(0, eventOS_callback_timer_stop(0));

    ASSERT_EQ(0, eventOS_callback_timer_start(0, 0));
    ASSERT_EQ(0, eventOS_callback_timer_start(1, 1));

    ASSERT_EQ(0, eventOS_callback_timer_start(2, 0));

    ASSERT_EQ(0, eventOS_callback_timer_start(2, 4));

    ASSERT_EQ(0, eventOS_callback_timer_start(1, 2));

    ASSERT_EQ(0, eventOS_callback_timer_stop(0));
    ASSERT_EQ(0, eventOS_callback_timer_start(0, 0));
    ASSERT_EQ(0, eventOS_callback_timer_start(0, 4));

    ASSERT_EQ(0, eventOS_callback_timer_stop(1));
    ASSERT_EQ(0, eventOS_callback_timer_stop(2));
    ASSERT_EQ(0, eventOS_callback_timer_stop(0));

    ASSERT_EQ(0, eventOS_callback_timer_unregister(0));
    ASSERT_EQ(0, eventOS_callback_timer_unregister(1));
    ASSERT_EQ(0, eventOS_callback_timer_unregister(2));

    ASSERT_EQ(-1, eventOS_callback_timer_stop(0)); //not found
}

#endif
