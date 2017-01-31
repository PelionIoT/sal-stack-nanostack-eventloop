/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef EVENTOS_EVENT_TIMER_H_
#define EVENTOS_EVENT_TIMER_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "ns_types.h"

struct arm_event_s;

/* 100 Hz ticker, so 10 milliseconds per tick */
#define EVENTOS_EVENT_TIMER_HZ 100

static inline uint32_t eventOS_event_timer_ticks_to_ms(uint32_t ticks)
{
    NS_STATIC_ASSERT(1000 % EVENTOS_EVENT_TIMER_HZ == 0, "Assuming whole number of ms per tick")
    return ticks * (1000 / EVENTOS_EVENT_TIMER_HZ);
}

static inline uint32_t eventOS_event_timer_ms_to_ticks(uint32_t ms)
{
    NS_STATIC_ASSERT(1000 % EVENTOS_EVENT_TIMER_HZ == 0, "Assuming whole number of ms per tick")
    return (ms + (1000 / EVENTOS_EVENT_TIMER_HZ) - 1) / (1000 / EVENTOS_EVENT_TIMER_HZ);
}

/**
 * Read current timer tick count.
 *
 * Can be used as a monotonic time source, and to schedule events with
 * eventOS_event_timer_send.
 *
 * Note that the value will wrap, so take care on comparisons.
 *
 * \return tick count.
 */
extern uint32_t eventOS_event_timer_ticks(void);

/* Comparison macros handling wrap efficiently (assuming a conventional compiler
 * which converts 0x80000000 to 0xFFFFFFFF to negative when casting to int32_t).
 */
#define TICKS_AFTER(a, b) ((int32_t) ((a)-(b)) > 0)
#define TICKS_BEFORE(a, b) ((int32_t) ((a)-(b)) < 0)
#define TICKS_AFTER_OR_AT(a, b) ((int32_t) ((a)-(b)) >= 0)
#define TICKS_BEFORE_OR_AT(a, b) ((int32_t) ((a)-(b)) <= 0)

/**
 * Send an event after time expired (in milliseconds)
 *
 * Note that the current implementation has the "feature" that rounding
 * varies depending on the precise timing requested:
 *     0-20 ms => 2 x 10ms tick
 *    21-29 ms => 3 x 10ms tick
 *    30-39 ms => 4 x 10ms tick
 *    40-49 ms => 5 x 10ms tick
 *    ... etc
 *
 * For improved flexibility on the event, and for more control of time,
 * you should use eventOS_event_timer_request_at().
 *
 * \param event_id event_id for event
 * \param event_type event_type for event
 * \param tasklet_id receiver for event
 * \param time time to sleep in milliseconds
 *
 * \return 0 on success
 * \return -1 on error (invalid tasklet_id or allocation failure)
 *
 * */
extern int8_t eventOS_event_timer_request(uint8_t event_id, uint8_t event_type, int8_t tasklet_id, uint32_t time);

/**
 * Send an event at specified time
 *
 * The event will be sent when eventOS_event_timer_ticks() reaches the
 * specified value.
 *
 * Can also be invoked using the eventOS_event_send_at() macro in eventOS_event.h
 *
 * \param event event to send
 * \param at absolute tick time to run event at
 *
 * \return 0 on success
 * \return -1 on error (invalid tasklet_id or allocation failure)
 *
 */
extern int8_t eventOS_event_timer_send(const struct arm_event_s *event, uint32_t at);

/**
 * Cancel an event timer
 *
 * This cancels a pending timed event, matched by event_id and tasklet_id.
 *
 * Note that the current implementation can only cancel the *timer* - if the
 * timer has expired and the event is already queued, the pending event is not
 * cancelled.
 *
 * \param event_id event_id for event
 * \param tasklet_id receiver for event
 *
 * \return 0 on success
 * \return -1 on error (event not found)
 *
 * */
extern int8_t eventOS_event_timer_cancel(uint8_t event_id, int8_t tasklet_id);

/**
 * System Timer shortest time in milli seconds
 *
 * \param ticks Time in 10 ms resolution
 *
 * \return none
 *
 * */
extern uint32_t eventOS_event_timer_shortest_active_timer(void);


/** Timeout structure. Not to be modified by user */
typedef struct timeout_entry_t timeout_t;

/** Request timeout callback.
 *
 * Create timeout request for specific callback. Maximum 255 timeouts can be requested.
 * Not thread safe. Should not be called from interrupt context.
 * \param ms timeout in milliseconds. Maximum range is same as for eventOS_event_timer_request().
 * \param callback function to call after timeout
 * \param arg arquement to pass to callback
 * \return pointer to timeout structure or NULL on errors
 */
timeout_t *eventOS_timeout_ms(void (*callback)(void *), uint32_t ms, void *arg);

/** Cancell timeout request.
 * Not thread safe. Should not be called from interrupt context.
 * \param t timeout request id.
 */
void eventOS_timeout_cancel(timeout_t *t);


#ifdef __cplusplus
}
#endif

#endif /* EVENTOS_EVENT_TIMER_H_ */
