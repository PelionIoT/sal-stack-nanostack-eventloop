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
#include "nsdynmemLIB.h"

#ifndef ST_MAX
#define ST_MAX 6
#endif

#define TIMER_SYS_TICK_PERIOD 100

uint32_t run_time_tick_ticks=0;
sys_timer_struct_s * system_timer_free_ptr = 0;
sys_timer_struct_s * system_timer_list_ptr = 0;
int8_t sys_timer_id = -1;


static void timer_struct_free_push(sys_timer_struct_s *free);
static sys_timer_struct_s * sys_timer_dynamically_allocate(void);
static void timer_sys_interrupt(int8_t timer_id, uint16_t slots);



/*
 * Initializes timers and starts system timer
 */
void timer_sys_init(void)
{
	uint8_t i;
	sys_timer_struct_s *temp = 0;
	run_time_tick_ticks=0;

	sys_timer_id = ns_timer_register(timer_sys_interrupt);

	// Clear old timers
	while(system_timer_list_ptr)
	{
		temp = system_timer_list_ptr;
		system_timer_list_ptr = system_timer_list_ptr->next;
		ns_dyn_mem_free(temp);
	}
	// Clear old free timer entrys
	while(system_timer_free_ptr)
	{
		temp = system_timer_free_ptr;
		system_timer_free_ptr = system_timer_free_ptr->next;
		ns_dyn_mem_free(temp);
	}

	for(i=0; i< ST_MAX; i++)
	{
		temp = sys_timer_dynamically_allocate();
		if(temp)
		{
			temp->next = system_timer_free_ptr;
			system_timer_free_ptr = temp;
		}
	}

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


static void timer_sys_interrupt(int8_t timer_id, uint16_t slots)
{
	sys_timer_struct_s * cur, *temp, *prev = 0;

	/*200 * 50us = 10ms*/
	ns_timer_start(sys_timer_id, 200);

	//Keep runtime time
	run_time_tick_ticks ++;

	platform_enter_critical();
	if(system_timer_list_ptr)
	{
		cur = system_timer_list_ptr;
		while(cur)
		{
			if(cur->timer_sys_launch_time <= 1)
			{
				arm_event_s event;
				event.receiver = cur->timer_sys_launch_receiver;
				event.sender = protocol_read_tasklet_id(); /**< Event sender Tasklet ID */
				event.data_ptr = 0;
				event.event_type = ARM_LIB_SYSTEM_TIMER_EVENT;
				event.event_id = cur->timer_sys_launch_message;
				event.event_data = 0;
				event.cb_fptr = NULL;
				arm_ns_event_send(&event);
				if(prev == 0)
				{
					system_timer_list_ptr = cur->next;
				}
				else
				{
					prev->next = cur->next;
				}
				temp = cur;
				cur = cur->next;
				temp->next = 0;
				timer_struct_free_push(temp);
			}
			else
			{
				cur->timer_sys_launch_time--;
				prev = cur;
				cur = cur->next;
			}
		}
	}
	platform_exit_critical();
}



/* * * * * * * * * */

static sys_timer_struct_s *sys_timer_dynamically_allocate(void)
{
	sys_timer_struct_s *timer = 0;
	timer = (sys_timer_struct_s *) ns_dyn_mem_alloc(sizeof(sys_timer_struct_s));
	if(timer)
	{
		timer->next = 0;
	}
	return timer;
}

static sys_timer_struct_s * timer_struct_get(void)
{
	sys_timer_struct_s *timer = 0;
	platform_enter_critical();
	if(system_timer_free_ptr)
	{
		timer = system_timer_free_ptr;
		system_timer_free_ptr = system_timer_free_ptr->next;
		timer->next = 0;
	}
	else
	{
		debug("Timer DYN Allocate\n");
		timer = sys_timer_dynamically_allocate();
	}
	platform_exit_critical();
	return timer;
}

static void timer_struct_free_push(sys_timer_struct_s *free)
{
	platform_enter_critical();
	free->next = system_timer_free_ptr;
	system_timer_free_ptr = free;
	platform_exit_critical();
}

uint32_t timer_get_runtime_ticks(void)  // only used in dev_stats_internal.c
{
	uint32_t ret_val;
	platform_enter_critical();
	ret_val = run_time_tick_ticks;
	platform_exit_critical();
	return ret_val;
}

void timer_runtime_ticks_sleep_update(uint32_t sleep_ticks)
{
	platform_enter_critical();
	run_time_tick_ticks += sleep_ticks;
	platform_exit_critical();
}

int8_t timer_sys_event(uint8_t snmessage, uint32_t time)
{
	int8_t res=-1;
	sys_timer_struct_s * timer = 0;

	platform_enter_critical();
	if(time > 20)
	{
		time /= (1000 / TIMER_SYS_TICK_PERIOD);
		time++;
	}
	else
	{
		time = 2;
	}
	timer = timer_struct_get();
	if(timer)
	{
		timer->timer_sys_launch_message = snmessage;
		timer->timer_sys_launch_receiver = event_get_active_tasklet();
		timer->timer_sys_launch_time = time;
		timer->next = system_timer_list_ptr;
		system_timer_list_ptr = timer;
		res=0;
	}
	platform_exit_critical();
	return res;
}

int8_t timer_sys_event_cancel(uint8_t snmessage)
{
	uint8_t temp;
	sys_timer_struct_s * cur, *prev = 0;
	int8_t res=-1;
	temp = event_get_active_tasklet();
	platform_enter_critical();
	if(system_timer_list_ptr)
	{
		cur = system_timer_list_ptr;
		while(cur)
		{
			if(cur->timer_sys_launch_receiver == temp && cur->timer_sys_launch_message == snmessage)
			{
				if(prev == 0)
				{
					system_timer_list_ptr = cur->next;
				}
				else
				{
					prev->next = cur->next;
				}
				cur->next = 0;
				timer_struct_free_push(cur);
				res = 0;
				cur = 0;
			}
			else
			{
				prev = cur;
				cur = cur->next;
			}
		}
	}
	platform_exit_critical();
	return res;
}


uint32_t core_timer_shortest_tick(void)
{
	uint32_t ret_val = 0;
	sys_timer_struct_s * cur;
	platform_enter_critical();
	if(system_timer_list_ptr)
	{
		cur = system_timer_list_ptr;
		while(cur)
		{
			if(ret_val == 0)
			{
				ret_val = cur->timer_sys_launch_time;
			}
			else if( cur->timer_sys_launch_time < ret_val)
			{
				ret_val = cur->timer_sys_launch_time;
			}
			cur = cur->next;
		}
	}
	platform_exit_critical();
	return ret_val;
}

