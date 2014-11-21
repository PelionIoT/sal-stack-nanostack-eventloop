/*
 * core_timer.c
 *
 *  Created on: Apr 7, 2010
 *      Author: juha
 */
#include "ns_types.h"
#include "tasklet_api.h"
#include "sys_error.h"
#include "sys_event.h"
#include "timer_sys.h"
#include "platform/ns_debug.h"
#include "platform/arm_hal_interrupt.h"
#include "nsdynmemLIB.h"

#ifndef ST_MAX
#define ST_MAX 6
#endif


typedef struct sys_timer_struct_s
{
	uint32_t timer_sys_launch_time;
	int8_t timer_sys_launch_receiver;
	uint8_t timer_sys_launch_message;
	struct sys_timer_struct_s *next;
} sys_timer_struct_s;



NS_LARGE uint32_t run_time_tick_ticks=0;

static sys_timer_struct_s * sytem_timer_list_ptr = 0;
static sys_timer_struct_s * sytem_timer_free_ptr = 0;

static uint16_t sytem_timer_tick_period = 100;



static sys_timer_struct_s * sys_timer_dynamically_allocate(void)
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
	if(sytem_timer_free_ptr)
	{

		timer = sytem_timer_free_ptr;
		sytem_timer_free_ptr = sytem_timer_free_ptr->next;
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
	free->next = sytem_timer_free_ptr;
	sytem_timer_free_ptr = free;
	platform_exit_critical();
}


void core_timer_init(void)
{
	uint8_t i;
	sys_timer_struct_s *temp = 0;
	run_time_tick_ticks=0;
	//Clean Old timers
	while(sytem_timer_list_ptr)
	{
		temp = sytem_timer_list_ptr;
		sytem_timer_list_ptr = sytem_timer_list_ptr->next;
		ns_dyn_mem_free(temp);
	}
	//Clean Old free timer entrys
	while(sytem_timer_free_ptr)
	{
		temp = sytem_timer_free_ptr;
		sytem_timer_free_ptr = sytem_timer_free_ptr->next;
		ns_dyn_mem_free(temp);
	}

	for(i=0; i< ST_MAX; i++)
	{
		temp = sys_timer_dynamically_allocate();
		if(temp)
		{
			temp->next = sytem_timer_free_ptr;
			sytem_timer_free_ptr = temp;
		}
	}
}

uint32_t timer_get_runtime_ticks(void)
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
		time /= (1000 / sytem_timer_tick_period);
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
		timer->next = sytem_timer_list_ptr;
		sytem_timer_list_ptr = timer;
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
	if(sytem_timer_list_ptr)
	{
		cur = sytem_timer_list_ptr;
		while(cur)
		{
			if(cur->timer_sys_launch_receiver == temp && cur->timer_sys_launch_message == snmessage)
			{
				if(prev == 0)
				{
					sytem_timer_list_ptr = cur->next;
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

extern int8_t protocol_read_tasklet_id(void);
void core_timer_tick_update(uint32_t ticks)
{
	sys_timer_struct_s * cur, *temp, *prev = 0;
	platform_enter_critical();
	if(sytem_timer_list_ptr)
	{
		cur = sytem_timer_list_ptr;
		while(cur)
		{
			if(cur->timer_sys_launch_time <= ticks)
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
					sytem_timer_list_ptr = cur->next;
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
				cur->timer_sys_launch_time -= ticks;
				prev = cur;
				cur = cur->next;
			}
		}
	}
	platform_exit_critical();
}

uint32_t core_timer_shortest_tick(void)
{
	uint32_t ret_val = 0;
	sys_timer_struct_s * cur;
	platform_enter_critical();
	if(sytem_timer_list_ptr)
	{
		cur = sytem_timer_list_ptr;
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

void core_timer_callback(void)
{
	core_timer_tick_update(1);
	//Keep runtime time
	run_time_tick_ticks ++;
}
