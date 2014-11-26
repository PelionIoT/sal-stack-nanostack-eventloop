/*
 * \file eventOS_event_timer.h
 * \brief Add short description about this file!!!
 * Copyrigth Sensinode Ltd 2013
 *
 */

#ifndef EVENTOS_EVENT_TIMER_H_
#define EVENTOS_EVENT_TIMER_H_
 #ifdef __cplusplus
extern "C" {
#endif
#include "ns_types.h"

/**
 * Send an event after time expired (in milliseconds)
 *
 * \param snmessage event to send
 * \param time time to sleep in milliseconds
 *
 * \return none
 *
 * */
extern int8_t eventOS_event_timer_request(uint8_t snmessage, uint8_t event_type, int8_t tasklet_id, uint32_t time);
/**
 * Cancel an event
 *
 * \param event event to cancel
 *
 * \return none
 *
 * */
extern int8_t eventOS_event_timer_cancel(uint8_t snmessage, int8_t tasklet_id);

/**
 * System Timer shortest time in milli seconds
 *
 * \param ticks Time in 10 ms resolution
 *
 * \return none
 *
 * */
extern uint32_t eventOS_event_timer_shortest_active_timer(void);
#ifdef __cplusplus
}
#endif

#endif /* EVENTOS_EVENT_TIMER_H_ */
