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
#include "ns_list.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "timer_sys.h"
#include "nsdynmemLIB.h"
#include "ns_timer.h"
#include "event.h"
#include "nanostack_eventloop.h"

#include "nsdynmemLIB_stub.h"
#include "ns_timer_stub.h"
#include "system_timer_stub.h"

void *ptr;

class event_test : public testing::Test {
protected:
    void SetUp()
    {
        eventOS_scheduler_init();
        ptr = malloc(64);
        nsdynmemlib_stub.expectedPointer = NULL;
        nsdynmemlib_stub.returnCounter = 0;
        ns_timer_stub.int8_value = 0;
        system_timer_stub.int8_value = 0;
    }

    void TearDown()
    {
        free(ptr);
    }
};

static void event_handler(arm_event_s *event)
{
}

static void event_handler1(arm_event_s *event) {}
static void event_handler2(arm_event_s *event) {}
static void event_handler3(arm_event_s *event) {}
static void event_handler4(arm_event_s *event) {}
static void event_handler5(arm_event_s *event) {}
static void event_handler6(arm_event_s *event) {}
static void event_handler7(arm_event_s *event) {}
static void event_handler8(arm_event_s *event) {}
static void event_handler9(arm_event_s *event) {}
static void event_handler10(arm_event_s *event) {}

TEST_F(event_test, event_tasklet_handler_id_valid)
{
    ASSERT_EQ(false, event_tasklet_handler_id_valid(1));
    nsdynmemlib_stub.expectedPointer = ptr;
    int8_t id = eventOS_event_handler_create(event_handler, 0);
    ASSERT_EQ(true, event_tasklet_handler_id_valid(id));
}

TEST_F(event_test, eventOS_event_handler_create)
{
    int8_t id = eventOS_event_handler_create(event_handler, 0);
    ASSERT_EQ(-2, id);

    nsdynmemlib_stub.expectedPointer = ptr;
    id = eventOS_event_handler_create(event_handler, 0);

    //Cannot use same cb function again
    id = eventOS_event_handler_create(event_handler, 0);
    ASSERT_EQ(-1, id);

    void *ptr1 = malloc(64);
    void *ptr2 = malloc(64);
    void *ptr3 = malloc(64);
    void *ptr4 = malloc(64);
    void *ptr5 = malloc(64);
    void *ptr6 = malloc(64);
    void *ptr7 = malloc(64);
    void *ptr8 = malloc(64);
    void *ptr9 = malloc(64);
    void *ptr10 = malloc(64);
    nsdynmemlib_stub.expectedPointer = ptr1;
    id = eventOS_event_handler_create(event_handler1, 0);
    nsdynmemlib_stub.expectedPointer = ptr2;
    id = eventOS_event_handler_create(event_handler2, 0);
    nsdynmemlib_stub.expectedPointer = ptr3;
    id = eventOS_event_handler_create(event_handler3, 0);
    nsdynmemlib_stub.expectedPointer = ptr4;
    id = eventOS_event_handler_create(event_handler4, 0);
    nsdynmemlib_stub.expectedPointer = ptr5;
    id = eventOS_event_handler_create(event_handler5, 0);
    nsdynmemlib_stub.expectedPointer = ptr6;
    id = eventOS_event_handler_create(event_handler6, 0);
    nsdynmemlib_stub.expectedPointer = ptr7;
    id = eventOS_event_handler_create(event_handler7, 0);
    nsdynmemlib_stub.expectedPointer = ptr8;
    id = eventOS_event_handler_create(event_handler8, 0);
    nsdynmemlib_stub.expectedPointer = ptr9;
    id = eventOS_event_handler_create(event_handler9, 0);
    //there are no empty events left in list

    //make malloc fail
    nsdynmemlib_stub.expectedPointer = NULL;
    nsdynmemlib_stub.returnCounter = 1;
    id = eventOS_event_handler_create(event_handler10, 0);
    ASSERT_EQ(-2, id);

    nsdynmemlib_stub.expectedPointer = ptr10;
    id = eventOS_event_handler_create(event_handler10, 0);
    ASSERT_NE(-2, id);

    //Let's do this here
    arm_event_storage_t *evi = eventOS_event_find_by_id_critical(id, 0);
    eventOS_cancel(evi);

    free(ptr1);
    free(ptr2);
    free(ptr3);
    free(ptr4);
    free(ptr5);
    free(ptr6);
    free(ptr7);
    free(ptr8);
    free(ptr9);
}

TEST_F(event_test, eventOS_event_send)
{
    nsdynmemlib_stub.expectedPointer = ptr;
    int8_t id = eventOS_event_handler_create(event_handler, 0);
    arm_event_t ev;
    ev.receiver = id;
    ev.priority = ARM_LIB_LOW_PRIORITY_EVENT;
    int8_t ret = eventOS_event_send(&ev);
    ASSERT_EQ(0, ret);

    ev.receiver = -1;
    ret = eventOS_event_send(&ev);
    ASSERT_EQ(-1, ret);

    ev.receiver = id;
    for (int i = 0; i < 9; i++) {
        ret = eventOS_event_send(&ev);
    }
    nsdynmemlib_stub.expectedPointer = NULL;

    ret = eventOS_event_send(&ev);
    ASSERT_EQ(-1, ret);
}

TEST_F(event_test, eventOS_event_send_user_allocated)
{
    //This can only fail if OOM happens
    arm_event_storage_t ev;
    eventOS_event_send_user_allocated(&ev);
}

TEST_F(event_test, eventOS_event_send_timer_allocated)
{
    //This can only fail if OOM happens
    arm_event_storage_t ev;
    eventOS_event_send_timer_allocated(&ev);
}

//TEST_F(event_test, event_core_free_push)
//{
// Tested in eventOS_cancel
//}

TEST_F(event_test, eventOS_event_find_by_id_critical)
{
    arm_event_storage_t *ret = eventOS_event_find_by_id_critical(0, 0);
    ASSERT_TRUE(NULL == ret);

    nsdynmemlib_stub.expectedPointer = ptr;
    int8_t id = eventOS_event_handler_create(event_handler, 0);
    arm_event_storage_t ev;
    ev.data.receiver = id;
    ev.data.event_id = 0;
    eventOS_event_send_user_allocated(&ev);
    ret = eventOS_event_find_by_id_critical(id, 0);
    ASSERT_TRUE(ret);
}

TEST_F(event_test, eventOS_scheduler_get_active_tasklet)
{
    ASSERT_TRUE(0 == eventOS_scheduler_get_active_tasklet());
}

TEST_F(event_test, eventOS_scheduler_set_active_tasklet)
{
    eventOS_scheduler_set_active_tasklet(1);
    ASSERT_TRUE(1 == eventOS_scheduler_get_active_tasklet());
}

TEST_F(event_test, eventOS_scheduler_timer_stop)
{
    ASSERT_TRUE(0 == eventOS_scheduler_timer_stop());
    ns_timer_stub.int8_value = 5;
#ifndef NS_EXCLUDE_HIGHRES_TIMER
    ASSERT_TRUE(1 == eventOS_scheduler_timer_stop());
#else
    ASSERT_TRUE(0 == eventOS_scheduler_timer_stop());
#endif
}

TEST_F(event_test, eventOS_scheduler_timer_synch_after_sleep)
{
    ASSERT_TRUE(0 == eventOS_scheduler_timer_synch_after_sleep(1));
    system_timer_stub.int8_value = 5;
    ASSERT_TRUE(-1 == eventOS_scheduler_timer_synch_after_sleep(1));
}

TEST_F(event_test, eventOS_scheduler_dispatch_event)
{
    ASSERT_FALSE(eventOS_scheduler_dispatch_event());
    nsdynmemlib_stub.expectedPointer = ptr;
    int8_t id = eventOS_event_handler_create(event_handler, 0);
    arm_event_storage_t ev;
    ev.data.receiver = id;
    ev.data.event_id = 0;
    ev.data.priority = ARM_LIB_HIGH_PRIORITY_EVENT;
    eventOS_event_send_user_allocated(&ev);

    ASSERT_TRUE(eventOS_scheduler_dispatch_event());
}

TEST_F(event_test, eventOS_scheduler_run_until_idle)
{
    //This will be an endless loop, so cannot test this
}

TEST_F(event_test, eventOS_scheduler_run)
{
    //This will be an endless loop, so cannot test this
}

TEST_F(event_test, eventOS_cancel)
{
    eventOS_cancel(NULL);
    arm_event_storage_t event;
    event.allocator = event.ARM_LIB_EVENT_TIMER;
    event.state = event.ARM_LIB_EVENT_QUEUED;

    eventOS_event_send_timer_allocated(&event);
    eventOS_cancel(&event);
}
