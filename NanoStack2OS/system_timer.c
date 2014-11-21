/*
 * system_timer.c
 *
 *  Created on: 14 Jan 2014
 *      Author: jarpas01
 */
#include "ns_types.h"
#include "tasklet_api.h"
#include "sys_error.h"
#include "sys_event.h"
#include "timer_sys.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/ns_debug.h"
#include "ns_timer.h"

void timer_sys_interrupt(int8_t timer_id, uint16_t slots);

int8_t sys_timer_id = -1;

/*
 * Initializes core timer, sleep timer and starts system timer
 */
void timer_sys_init(void)
{
	sys_timer_id = ns_timer_register(timer_sys_interrupt);
	core_timer_init();

	if(sys_timer_id >= 0)
		ns_timer_start(sys_timer_id, 200);
}



/*-------------------SYSTEM TIMER FUNCTIONS--------------------------*/
void timer_sys_disable(void)
{
	ns_timer_stop(sys_timer_id);
}

/*
 * Starts ticking system timer interrupts every 10ms
 */
int8_t timer_sys_wakeup(void)
{
	int8_t ret_val = -1;
	if(sys_timer_id >= 0)
	{
		ret_val = ns_timer_start(sys_timer_id, 200);
	}
	return ret_val;
}


void timer_sys_interrupt(int8_t timer_id, uint16_t slots)
{
	/*200 * 50us = 10ms*/
	ns_timer_start(sys_timer_id, 200);
	core_timer_callback();
}
