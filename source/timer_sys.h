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

extern uint32_t timer_get_runtime_ticks(void);
int8_t timer_sys_wakeup(void);
void timer_sys_disable(void);

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
