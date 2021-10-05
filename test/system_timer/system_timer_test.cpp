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
#include "timer_sys.h"
#include "event_stub.h"
#include "eventOS_event_timer.h"

#define STARTUP_EVENT 0
#define TIMER_EVENT 1

// Timeout structure, already typedefed to timeout_t
struct timeout_entry_t {
    void (*callback)(void *);
    void *arg;
    arm_event_storage_t *event;
};

uint32_t ticks = 0;

uint32_t timer_get_runtime_ticks(void)
{
    return ticks;
}

void cb(void *a){}

class system_timer_test : public testing::Test {
protected:
    void SetUp()
    {
        ticks = 0;
        event_stub.bool_value = false;
        timer_sys_init();
    }

    void TearDown()
    {
    }
};

TEST_F(system_timer_test, timer_sys_wakeup)
{
    timer_sys_wakeup();
    //Nothing to test here
}

TEST_F(system_timer_test, timer_sys_disable)
{
    timer_sys_disable();
    //Nothing to test here
}

TEST_F(system_timer_test, timer_sys_event_free)
{
//    timer_sys_event_free(NULL); //SEG_FAULTs at the moment!

    sys_timer_struct_s ss;
    memset(&ss, 0, sizeof(sys_timer_struct_s));

    ss.period = 1;
    timer_sys_event_free(&ss.event);

    system_timer_tick_update(8);

    ss.period = 1;
    timer_sys_event_free(&ss.event);
}

TEST_F(system_timer_test, eventOS_event_timer_request)
{
    ASSERT_EQ(-1, eventOS_event_timer_request(1, 1, 1, 6));

    ASSERT_EQ(-1, eventOS_event_timer_request(1, 1, 1, 3000));
}

TEST_F(system_timer_test, eventOS_event_timer_cancel)
{
    ASSERT_EQ(-1, eventOS_event_timer_cancel(1, 1));
}



TEST_F(system_timer_test, eventOS_event_timer_request_every)
{
    timeout_t timeout;
    timeout.callback = cb;
    timeout.arg = NULL;
    arm_event_t event = {
        .receiver   = 1,
        .sender     = 1,
        .event_type = TIMER_EVENT,
        .event_id   = TIMER_EVENT,
        .data_ptr   = &timeout
    };
    ASSERT_TRUE(NULL == eventOS_event_timer_request_every(&event, 0));
    ASSERT_TRUE(NULL == eventOS_event_timer_request_every(&event, 5));
    event_stub.bool_value = true;
    arm_event_storage_t *storage = eventOS_event_timer_request_every(&event, 5);

    eventOS_event_timer_cancel(1, 1);
    timer_sys_event_cancel_critical(storage);
    timer_sys_event_free(storage);
}

TEST_F(system_timer_test, eventOS_event_timer_request_in)
{
    timeout_t timeout;
    timeout.callback = cb;
    timeout.arg = NULL;
    arm_event_t event = {
        .receiver   = 1,
        .sender     = 1,
        .event_type = TIMER_EVENT,
        .event_id   = TIMER_EVENT,
        .data_ptr   = &timeout
    };
    ASSERT_TRUE(NULL == eventOS_event_timer_request_in(&event, 0));
    ASSERT_TRUE(NULL == eventOS_event_timer_request_in(&event, 5));
    event_stub.bool_value = true;
    arm_event_storage_t *storage = eventOS_event_timer_request_in(&event, 5);

    eventOS_event_timer_cancel(1, 1);
    timer_sys_event_cancel_critical(storage);
    timer_sys_event_free(storage);
}

TEST_F(system_timer_test, eventOS_event_timer_request_at)
{
    timeout_t timeout;
    timeout.callback = cb;
    timeout.arg = NULL;
    arm_event_t event = {
        .receiver   = 1,
        .sender     = 1,
        .event_type = TIMER_EVENT,
        .event_id   = TIMER_EVENT,
        .data_ptr   = &timeout
    };
    ASSERT_TRUE(NULL == eventOS_event_timer_request_at(&event, 0));
    ASSERT_TRUE(NULL == eventOS_event_timer_request_at(&event, 5));
    event_stub.bool_value = true;
    arm_event_storage_t *storage = eventOS_event_timer_request_at(&event, 5);

    eventOS_event_timer_cancel(1, 1);
    timer_sys_event_free(storage);
}

TEST_F(system_timer_test, system_timer_tick_update)
{
    system_timer_tick_update(0);
}


TEST_F(system_timer_test, eventOS_event_timer_shortest_active_timer)
{
    ASSERT_EQ(0, eventOS_event_timer_shortest_active_timer());

    //TODO: test rest
}




