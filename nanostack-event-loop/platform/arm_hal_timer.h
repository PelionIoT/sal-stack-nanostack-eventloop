/*
 * Copyright (c) 2014 ARM. All rights reserved.
 */
#ifndef ARM_HAL_TIMER_H_
#define ARM_HAL_TIMER_H_
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \brief This function perform timer init.
 */
extern void platform_timer_enable(void);
/**
 * \brief This function is API for set Timer interrupt handler for stack
 *
 * \param new_fp Function pointer for stack giving timer handler
 *
 */
extern void platform_timer_set_cb(void (*new_fp)(void));
/**
 * \brief This function is API for stack timer start
 *
 * \param slots define how many 50us slot time period will be started
 *
 */
extern void platform_timer_start(uint16_t slots);
/**
 * \brief This function is API for stack timer stop
 *
 */
extern void platform_timer_disable(void);
/**
 * \brief This function is API for stack timer to read active timer remaining slot count
 *
 * \return 50us time slot remaining
 */
extern uint16_t platform_timer_get_remaining_slots(void);
#ifdef __cplusplus
}
#endif

#endif /* ARM_HAL_TIMER_H_ */
