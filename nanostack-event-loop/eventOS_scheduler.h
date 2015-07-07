/*
 * Copyright (c) 2014 ARM. All rights reserved.
 */
#ifndef EVENTOS_SCHEDULER_H_
#define EVENTOS_SCHEDULER_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "ns_types.h"
/**
 * \brief Initialise event scheduler.
 *
 */
extern void eventOS_scheduler_init(void);
/**
 * \brief Start Event scheduler.
 *
 */
extern noreturn void eventOS_scheduler_run(void);
/**
 * \brief Disable Event scheduler Timers
 *
 *
 * \return 0 Timer Stop OK
 * \return -1 Timer Stop Fail
 *
 * */
int eventOS_scheduler_timer_stop(void);
/**
 * \brief Synch Event scheduler timer after sleep
 *
 * \param sleep_ticks time in milli seconds
 *
 * \return 0 Timer Synch OK
 * \return -1 Timer Synch & Start Fail
 *
 * */
int eventOS_scheduler_timer_synch_after_sleep(uint32_t sleep_ticks);

/**
 * \brief Read current active Tasklet ID
 *
 * This function not return valid information called inside interrupt
 *
 * \return curret active tasklet id
 *
 * */
extern int8_t eventOS_scheduler_get_active_tasklet(void);
/**
 * \brief Set manually Active Tasklet ID
 *
 *
 * \param tasklet requested tasklet ID
 *
 * */
extern  void eventOS_scheduler_set_active_tasklet(int8_t tasklet);

/**
 * \brief Event schdeuler loop idle Callback which need to be port Every Application which use nanostack event scheduler
 *
 *
 */
extern void eventOS_scheduler_idle(void);

/**
 * \brief This function will be called when stack enter idle state and start waiting signal.
 */
extern void eventOS_scheduler_wait(void);

/**
 * \brief This function will be called when stack receive event and could wake from idle.
 */
extern void eventOS_scheduler_signal(void);

/**
 * \brief This function will be called when stack can enter deep sleep state in detected time.
 *
 * \param sleep_time_ms Time in milliseconds to sleep
 *
 * \return sleeped time in milliseconds
 */
extern uint32_t eventOS_scheduler_sleep(uint32_t sleep_time_ms);

#ifdef __cplusplus
}
#endif

#endif /* EVENTOS_SCHEDULER_H_ */
