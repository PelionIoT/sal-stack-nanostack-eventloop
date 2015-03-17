/*
 * Copyright (c) 2014-2015 ARM. All rights reserved.
 */
/**
 *
 * \file event.c
 * \brief Core event handler.
 *
 *  Event dispatching functions.
 *
 */
#include <string.h>
#include "ns_types.h"
#include "sys_error.h"
#include "sys_event.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "timer_sys.h"
#ifdef NS_DEBUG
#include "platform/ns_debug.h"
#else
#include "platform/ns_debug.h"
#endif
#include "nsdynmemLIB.h"
#include "ns_timer.h"

#include "platform/arm_hal_interrupt.h"


typedef struct arm_core_tasklet_list_s
{
	int8_t id; /**< Event handler Tasklet ID */
	void (*func_ptr)(arm_event_s *);
	struct arm_core_tasklet_list_s * next;
} arm_core_tasklet_list_s;

static arm_core_tasklet_list_s * arm_core_tasklet_list = 0;
static arm_core_event_s * event_queue_active = 0;
static arm_core_event_s * free_event_entry = 0;
static int8_t tasklet_dyn_counter = 0;

/** Curr_tasklet tell to core and platform which task_let is active, Core Update this automatic when switch Tasklet. */
int8_t curr_tasklet = 0;


static arm_core_tasklet_list_s * tasklet_dynamically_allocate(void);
static arm_core_event_s * event_dynamically_allocate(void);
arm_core_event_s * event_core_get(void);
void event_core_write(arm_core_event_s *event);

 static  arm_core_tasklet_list_s * event_tasklet_handler_get(uint8_t tasklet_id)
 {
	 arm_core_tasklet_list_s *new = 0;

	 new = arm_core_tasklet_list;
	 while(new)
	 {
		 if(new->id == tasklet_id)
		 {
			 break;
		 }
		 else
		 {
			 new = new->next;
		 }
	 }
	 return new;
 }

 static int8_t tasklet_get_free_id(void)
 {
	 int8_t ret_val = 0;
	 if(tasklet_dyn_counter)
	 {
		 int8_t base_start = 0;
		 int8_t i,free_iD;

		 arm_core_tasklet_list_s *new = 0;

		 for(i=0;i< tasklet_dyn_counter; i++)
		 {
			 new = arm_core_tasklet_list;
			 free_iD = 0;
			 while(new)
			 {
				 if(new->id == base_start)
				 {
					 free_iD = -1;
					 break;
				 }
				 else
				 {
					 new = new->next;
				 }
			 }
			 if(free_iD == 0)
			 {
				 break;
			 }
			 else
			 {
				 base_start++;
			 }

		 }
		 ret_val = base_start;
		 tasklet_dyn_counter++;
	 }
	 else
	 {
		 ret_val = 0;
		 tasklet_dyn_counter++;
	 }
	 return ret_val;
 }


 int8_t eventOS_event_handler_create(void (*handler_func_ptr)(arm_event_s*), uint8_t init_event_type)
 {
	 int8_t ret_val = 0;
	 arm_core_tasklet_list_s *new = 0;
	 arm_core_tasklet_list_s *prev = 0;
	 arm_core_event_s *event_tmp;

	 new = arm_core_tasklet_list;
	 while(new)
	 {
		 if(new->func_ptr == handler_func_ptr)
		 {
			 return -1;
		 }
		 else
		 {
			 prev = new;
			 new = new->next;
		 }
	 }

	 if(ret_val == 0)
	 {
		 //Allocate new
		 new = tasklet_dynamically_allocate();
		 if(new)
		 {
			 new->id = tasklet_get_free_id();
			 new->func_ptr = handler_func_ptr;
			 if(prev)
			 {
				 prev->next = new;
			 }
			 else
			 {
				 arm_core_tasklet_list =new;
			 }
			event_tmp = event_core_get();
			if(event_tmp)
			{
				event_tmp->receiver = new->id;
				event_tmp->sender = 0;
				event_tmp->event_type = init_event_type; //Function Pointer
				event_tmp->event_data = 0;
				event_core_write(event_tmp);
				ret_val = new->id;
			}
		 }
		 else
		 {
			 ret_val = -2;
		 }
	 }
	 return ret_val;
 }

/**
* \brief Send event to  event scheduler.
*
* \param event pointer to pushed event.
*
* \return 0 Event push OK
* \return -1 Memory allocation Fail
*
*/
int8_t eventOS_event_send(arm_event_s *event)
{
	int8_t retval = -1;
	if(event_tasklet_handler_get(event->receiver))
	{
		arm_core_event_s *event_tmp = event_core_get();
		if(event_tmp)
		{
			event_tmp->receiver = event->receiver;
			event_tmp->sender = event->sender;
			event_tmp->event_type = event->event_type; //Function Pointer
			event_tmp->event_data = event->event_data;
			event_tmp->event_id = event->event_id; //Function Pointer
			event_tmp->data_ptr = event->data_ptr;
			event_tmp->priority = event->priority;
			event_core_write(event_tmp);
			retval = 0;
		}
	}
	return retval;
}


static arm_core_event_s * event_dynamically_allocate(void)
{
	arm_core_event_s *event = 0;
	event = (arm_core_event_s *) ns_dyn_mem_alloc(sizeof(arm_core_event_s));
	if(event)
	{
		event->data_ptr = NULL;
		event->priority = ARM_LIB_LOW_PRIORITY_EVENT;
		event->next = NULL;
	}
	return event;
}

static arm_core_tasklet_list_s * tasklet_dynamically_allocate(void)
{
	arm_core_tasklet_list_s *event = 0;
	event = (arm_core_tasklet_list_s *) ns_dyn_mem_alloc(sizeof(arm_core_tasklet_list_s));
	if(event)
	{
		event->next = NULL;
	}
	return event;
}


arm_core_event_s * event_core_get(void)
{
	arm_core_event_s *event = 0;
	platform_enter_critical();
	if(free_event_entry)
	{

		event = free_event_entry;
		free_event_entry = free_event_entry->next;
		event->data_ptr = NULL;
		event->priority = ARM_LIB_LOW_PRIORITY_EVENT;
		event->next = NULL;
	}
	else
	{
		debug("DYN Allocate\n");
		event = event_dynamically_allocate();
	}
	platform_exit_critical();
	return event;
}

static void event_core_free_push(arm_core_event_s *free)
{
	platform_enter_critical();
	free->next = free_event_entry;
	free_event_entry = free;
	platform_exit_critical();
}


static arm_core_event_s * event_core_read(void)
{
	arm_core_event_s *event = 0;
	platform_enter_critical();
	if(event_queue_active)
	{
		event = event_queue_active;
		event_queue_active = event_queue_active->next;
	}
	platform_exit_critical();
	return event;
}

void event_core_write(arm_core_event_s *event)
{
	arm_core_event_s *event_tmp = 0;
	arm_core_event_s *event_prev = 0;
	platform_enter_critical();
	if(event_queue_active)
	{
		event_tmp = event_queue_active;
		while(event_tmp)
		{
			if(event_tmp->priority > event->priority)
			{
				if(event_prev)
				{
					//SUb
					event_prev->next = event;
					event->next = event_tmp;
				}
				else
				{
					//New First
					event->next = event_tmp;
					event_queue_active = event;
				}
				event_tmp = NULL;
			}
			else if(event_tmp->next == 0)
			{
				//New Last
				event_tmp->next = event;
				event_tmp = NULL;
			}
			else
			{
				event_prev = event_tmp;
				event_tmp = event_tmp->next;
			}
		}
		//event = event_queue_active;
		//event_queue_active = event_queue_active->next;
	}
	else
	{
		event_queue_active = event;
	}
	/* Wake From Idle */
	eventOS_scheduler_signal();
	platform_exit_critical();
}

/**
 *
 * \brief Initialize Nanostack Core.
 *
 * Function Initialize Nanostack Core, Socket Interface,Buffer memory and Send Init event to all Tasklett which are Defined.
 *
 */
void eventOS_scheduler_init(void)
{
	uint8_t i;
	arm_core_event_s *event = NULL;

	/* Reset Event Ringbuffer variables */
	i=10;
	//Allocate 10 entry
	free_event_entry = NULL;
	event_queue_active = NULL;
	arm_core_tasklet_list = NULL;
	while(i--)
	{
		event = event_dynamically_allocate();
		if(event)
		{
			if(free_event_entry)
			{
				event->next = free_event_entry;

			}
			free_event_entry = event;
		}
	}

	/* Init Generic timer module */
	ns_timer_init();
	timer_sys_init();				//initialize timer
	/* Set Tasklett switcher to Idle */
	curr_tasklet = 0;

}


int8_t eventOS_scheduler_get_active_tasklet(void)
{
	return  curr_tasklet;
}

void eventOS_scheduler_set_active_tasklet(int8_t tasklet)
{
	curr_tasklet = tasklet;
}

int eventOS_scheduler_timer_stop(void)
{
	timer_sys_disable();
	if(ns_timer_sleep() != 0)
	{
		return 1;
	}
	return 0;
}

int eventOS_scheduler_timer_synch_after_sleep(uint32_t sleep_ticks)
{
	//Update MS to 10ms ticks
	sleep_ticks /= 10;
	sleep_ticks++;
	system_timer_tick_update(sleep_ticks);
	if(timer_sys_wakeup() == 0)
	{
		return 0;
	}
	return -1;
}

/**
 *
 * \brief Infinite Event Read Loop.
 *
 * Function Read and handle Cores Event and switch/enable tasklet which are event receiver. WhenEvent queue is empty it goes to sleep
 *
 */
void event_dispatch_cycle(void)
{
	arm_core_tasklet_list_s *tasklet = 0;
	arm_core_event_s * cur_event = 0;
	arm_event_s event;
	curr_tasklet = 0;

	cur_event =  event_core_read();
	if(cur_event)
	{
		event.receiver = cur_event->receiver;
		event.sender = cur_event->sender;
		event.data_ptr = cur_event->data_ptr;
		event.event_data = cur_event->event_data;
		event.event_id = cur_event->event_id;
		event.event_type = cur_event->event_type;
		event_core_free_push(cur_event);
		tasklet = event_tasklet_handler_get(event.receiver);
		if(tasklet)
		{
			curr_tasklet = event.receiver;
			/* Tasklet Sceduler Call */
			tasklet->func_ptr(&event);
			/* Set Current Tasklet to Idle state */
			curr_tasklet = 0;
		}
		else
		{
			debug("NO Task1\n");
		}
	}
	else
	{
		eventOS_scheduler_idle();
	}
}

/**
 *
 * \brief Infinite Event Read Loop.
 *
 * Function Read and handle Cores Event and switch/enable tasklet which are event receiver. WhenEvent queue is empty it goes to sleep
 *
 */
noreturn void eventOS_scheduler_run(void)
{
	while (1)
	{
		event_dispatch_cycle();
	}
}
