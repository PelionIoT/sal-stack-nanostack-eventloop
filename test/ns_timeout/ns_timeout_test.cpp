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
#include "eventOS_event_timer.h"
#include "eventOS_event.h"
#include "nsdynmemLIB.h"
#include "nsdynmemLIB_stub.h"
#include "event_stub.h"
#include "system_timer_stub.h"
#include "timer_sys.h"

struct timeout_entry_t {
    void (*callback)(void *);
    void *arg;
    arm_event_storage_t *event;
};

class ns_timeout_test : public testing::Test {
protected:
    void SetUp()
    {
        nsdynmemlib_stub.returnCounter = 0;
        event_stub.int8_value = 0;
        event_stub.int_value = 0;
        event_stub.bool_value = false;
        event_stub.event_handler_cb = NULL;

        system_timer_stub.storage = NULL;
    }

    void TearDown()
    {
    }
};

void cb(void *a) {}

TEST_F(ns_timeout_test, eventOS_timeout_ms)
{
    eventOS_timeout_ms(cb, 2, NULL);

    nsdynmemlib_stub.returnCounter = 1;
    event_stub.int8_value = -1;
    eventOS_timeout_ms(cb, 2, NULL);

    nsdynmemlib_stub.returnCounter = 1;
    event_stub.int8_value = 1;
    eventOS_timeout_ms(cb, 2, NULL);

    arm_event_storage_t st;
    system_timer_stub.storage = &st;
    nsdynmemlib_stub.returnCounter = 1;
    event_stub.int8_value = 1;
    timeout_t *ret = eventOS_timeout_ms(cb, 2, NULL);
    ASSERT_TRUE(ret);
    ns_dyn_mem_free(ret);

    if (event_stub.event_handler_cb) {
        arm_event_s ev;
        ev.event_type = 0;
        event_stub.event_handler_cb(&ev);

        memset(&ev, 0, sizeof(arm_event_s));
        ev.event_type = 1;
        timeout_t *t = (timeout_t *)malloc(sizeof(timeout_t));
        t->callback = cb;
        t->arg = NULL;
        sys_timer_struct_s ss;
        memset(&ss, 0, sizeof(sys_timer_struct_s));
        t->event = &ss.event;
        ev.data_ptr = t;
        event_stub.event_handler_cb(&ev);
    }
}

TEST_F(ns_timeout_test, eventOS_timeout_every_ms)
{
    eventOS_timeout_every_ms(cb, 2, NULL);

    arm_event_storage_t st;
    system_timer_stub.storage = &st;
    nsdynmemlib_stub.returnCounter = 1;
    event_stub.int8_value = 1;
    timeout_t *ret = eventOS_timeout_every_ms(cb, 2, NULL);
    ASSERT_TRUE(ret);
    ns_dyn_mem_free(ret);
}

TEST_F(ns_timeout_test, eventOS_timeout_cancel)
{
    eventOS_timeout_cancel(NULL);

    nsdynmemlib_stub.returnCounter = 1;
    struct timeout_entry_t *to = (struct timeout_entry_t *)ns_dyn_mem_alloc(sizeof(struct timeout_entry_t));
    arm_event_storage_t ev;
    to->event = &ev;
    ev.state = ev.ARM_LIB_EVENT_QUEUED;
    eventOS_timeout_cancel(to);
}
