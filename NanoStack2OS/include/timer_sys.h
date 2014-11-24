#ifndef _PL_NANO_TIMER_SYS_H_
#define _PL_NANO_TIMER_SYS_H_



typedef struct
{
	void (*fptr)(void);
	uint32_t ticks;
} core_timer_32_bit_t;


/**
 * Initialize system timer
 * */
extern void timer_sys_init(void);

/**
 * Force system sleep until time (in milliseconds)
 *
 * \param time time to sleep in milliseconds
 *
 * \return none
 *
 * */
//extern void timer_sys_sleep(uint32_t time);

/**
 * Send an event after time expired (in milliseconds)
 *
 * \param event event to send
 * \param time time to sleep in milliseconds
 *
 * \return none
 *
 * */
extern int8_t timer_sys_event(uint8_t snmessage, uint32_t time);

/**
 * Cancel an event
 *
 * \param event event to cancel
 *
 * \return none
 *
 * */
extern int8_t timer_sys_event_cancel(uint8_t snmessage);

extern uint32_t timer_get_runtime_ticks(void);

/**
 * System Timer update and synch after sleep
 *
 * \param ticks Time in 10 ms resolution
 *
 * \return none
 *
 * */
void system_timer_tick_update(uint32_t ticks);

#endif /*_PL_NANO_TIMER_SYS_H_*/
