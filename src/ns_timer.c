#include "ns_types.h"
#include "sys_error.h"
#include "sys_event.h"
#include "ns_timer.h"
#include "platform/ns_debug.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/arm_hal_timer.h"
#include "nsdynmemLIB.h"

static ns_timer_struct *ns_timer_list=0;


#define NS_TIMER_RUNNING	1
static uint8_t ns_timer_state = 0;

#ifdef ATMEGA256RFR2
#define COMPENSATION 3
#define COMPENSATION_TUNE 1
#else
#define COMPENSATION 0
#define COMPENSATION_TUNE 0
#endif

int8_t ns_timer_init(void)
{
	/*Set interrupt handler in HAL driver*/
	platform_timer_set_cb(ns_timer_interrupt_handler);
	return 0;
}

int8_t eventOS_callback_timer_register(void (*timer_interrupt_handler)(int8_t, uint16_t))
{
	int8_t retval = -1;
	ns_timer_struct *current_timer;

	if(ns_timer_list)
	{
		int8_t i = 0;
		/*Find first free timer ID in timer list*/
		do
		{
			current_timer = ns_timer_list;
			while(current_timer)
			{
				if(current_timer->ns_timer_id == i)
					break;
				else
					current_timer = current_timer->next_timer;
			}
			if(!current_timer)
			{
				retval = i;
				i=0;
			}
			else
				i++;
		}while(i);

		if(retval >= 0)
		{
			/*Find first free next timer -pointer in timer list*/
			current_timer = ns_timer_list;
			while(current_timer)
			{
				if(!current_timer->next_timer)
					break;
				else
					current_timer = current_timer->next_timer;
			}
			/*Allocate memory for timer structure*/
			if(current_timer)
			{
				current_timer->next_timer = ns_dyn_mem_alloc(sizeof(ns_timer_struct));
				if(current_timer->next_timer)
					current_timer = current_timer->next_timer;
			}
		}
	}
	else
	{
		ns_timer_list = ns_dyn_mem_alloc(sizeof(ns_timer_struct));
		current_timer = ns_timer_list;
		retval = 0;
	}
	/*Initialise new timer*/
	if(current_timer)
	{
		current_timer->ns_timer_id = retval;
		current_timer->next_timer = 0;
		current_timer->timer_state = NS_TIMER_STOP;
		current_timer->remaining_slots = 0;
		current_timer->interrupt_handler = timer_interrupt_handler;
	}
	else
		retval = -1;
	/*Return timer ID*/
	return retval;
}

int8_t eventOS_callback_timer_unregister(int8_t ns_timer_id)
{
	int8_t retval = -1;
	ns_timer_struct *current_timer;
	ns_timer_struct *tmp_timer;

	current_timer = ns_timer_get_pointer_to_timer_struct(ns_timer_id);
	if(current_timer)
	{
		tmp_timer = ns_timer_list;
		while(tmp_timer)
		{
			if(tmp_timer->next_timer == current_timer)
			{
				break;
			}
			tmp_timer = tmp_timer->next_timer;
		}
		if(tmp_timer)
		{
			tmp_timer->next_timer = current_timer->next_timer;
		}
		else if(current_timer->next_timer)
		{
			ns_timer_list = current_timer->next_timer;
		}
		ns_dyn_mem_free(current_timer);
		retval = 0;
	}
	return retval;
}


int8_t ns_timer_start_pl_timer(uint16_t pl_timer_start_slots)
{
	/*Don't start timer with 0 slots*/
	if(!pl_timer_start_slots)
		pl_timer_start_slots = 1;

	/*Start HAL timer*/
	platform_timer_start(pl_timer_start_slots);
	/*Set HAL timer state to running*/
	ns_timer_state |= NS_TIMER_RUNNING;
	return 0;
}

int8_t ns_timer_sleep(void)
{
	int8_t ret_val = -1;
	if(ns_timer_state & NS_TIMER_RUNNING)
	{
		/*Start HAL timer*/
		platform_timer_disable();
		/*Set HAL timer state to running*/
		ns_timer_state &= ~NS_TIMER_RUNNING;
		ret_val = 0;
	}
	return ret_val;
}

int8_t ns_timer_wake_up(void)
{
	int8_t ret_val = -1;
	if((ns_timer_state & NS_TIMER_RUNNING) == 0)
	{
		/*Start HAL timer*/
		platform_timer_start(2);
		/*Set HAL timer state to running*/
		ns_timer_state |= NS_TIMER_RUNNING;
		ret_val = 0;
	}
	return ret_val;
}

int8_t ns_timer_get_next_running_to(void)
{
	uint8_t i = 0;

	ns_timer_struct *timer_tmp = 0;
	ns_timer_struct *current_timer;

	/*Find hold-labelled timer with the less remaining slots*/
	current_timer = ns_timer_list;
	while(current_timer)
	{
		if(current_timer->timer_state == NS_TIMER_HOLD)
		{
			if(timer_tmp)
			{
				if(current_timer->remaining_slots < timer_tmp->remaining_slots)
				{
					timer_tmp = current_timer;
				}
			}
			else
			{
				timer_tmp = current_timer;
			}
			/*For optimisation, count the found timers*/
			i++;
		}
		current_timer = current_timer->next_timer;
	}
	/*If hold-labelled timer found, set it active and start the HAL driver*/
	if(timer_tmp)
	{
		i--;
		timer_tmp->timer_state = NS_TIMER_ACTIVE;
		/*Compensate time spent in timer function*/
		if(timer_tmp->remaining_slots > COMPENSATION)
			timer_tmp->remaining_slots -= COMPENSATION;
		/*Start HAL timer*/
		ns_timer_start_pl_timer(timer_tmp->remaining_slots);

		/*Update hold-labelled timers*/
		current_timer = ns_timer_list;
		while(current_timer && i)
		{
			if(current_timer->timer_state == NS_TIMER_HOLD)
			{
				if(current_timer->remaining_slots == timer_tmp->remaining_slots)
				{
					current_timer->timer_state = NS_TIMER_ACTIVE;
				}
				else
				{
					current_timer->remaining_slots -= timer_tmp->remaining_slots;
					/*Compensate time spent in timer function*/
					if(current_timer->remaining_slots > COMPENSATION)
						current_timer->remaining_slots -= COMPENSATION;
				}
				i--;
			}
			current_timer = current_timer->next_timer;
		}
	}

	return 0;
}


ns_timer_struct *ns_timer_get_pointer_to_timer_struct(int8_t timer_id)
{
	ns_timer_struct *current_timer;
	/*Find timer with the given ID*/
	current_timer = ns_timer_list;
	while(current_timer)
	{
		if(current_timer->ns_timer_id == timer_id)
			break;
		current_timer = current_timer->next_timer;
	}
	return current_timer;
}

int8_t eventOS_callback_timer_start(int8_t ns_timer_id, uint16_t slots)
{
	int8_t ret_val = 0;
	uint16_t pl_timer_remaining_slots;
	ns_timer_struct *current_timer = 0;
	platform_enter_critical();

	/*If none of timers is active*/
	if(ns_timer_state & NS_TIMER_RUNNING)
	{
		/*Get remaining slots of the currently activated timeout*/
		pl_timer_remaining_slots = platform_timer_get_remaining_slots();

		/*New timeout is shorter than currently enabled timeout*/
		if(pl_timer_remaining_slots > slots)
		{
			/*Start HAL timer*/
			ns_timer_start_pl_timer(slots - 0);

			current_timer = ns_timer_list;
			while(current_timer)
			{
				/*Switch active timers to hold*/
				if(current_timer->timer_state == NS_TIMER_ACTIVE)
				{
					current_timer->timer_state = NS_TIMER_HOLD;
					current_timer->remaining_slots = 0;
				}
				/*Update hold-labelled timers*/
				if(current_timer->timer_state == NS_TIMER_HOLD)
				{
					current_timer->remaining_slots += (pl_timer_remaining_slots - slots);
					/*Compensate time spent in timer function*/
					if(current_timer->remaining_slots > (COMPENSATION - COMPENSATION_TUNE))
						current_timer->remaining_slots -= (COMPENSATION - COMPENSATION_TUNE);
				}
				current_timer = current_timer->next_timer;
			}
			/*Find timer to be activated*/
			current_timer = ns_timer_get_pointer_to_timer_struct(ns_timer_id);
			if(current_timer)
			{
				/*Mark active and start the timer*/
				current_timer->timer_state = NS_TIMER_ACTIVE;
				current_timer->slots = slots;
				current_timer->remaining_slots = slots;
			}
			else
			{
				ret_val = -3;
			}
		}

		/*New timeout is longer than currently enabled timeout*/
		else if(pl_timer_remaining_slots < slots)
		{
			current_timer = ns_timer_get_pointer_to_timer_struct(ns_timer_id);
			if(current_timer)
			{
				/*Mark hold and update remaining slots*/
				current_timer->timer_state = NS_TIMER_HOLD;
				current_timer->slots = slots;
				current_timer->remaining_slots = (slots - pl_timer_remaining_slots);
			}
			else
			{
				ret_val = -4;
			}
		}
		/*New timeout is equal to currently enabled timeout*/
		else
		{
			current_timer = ns_timer_get_pointer_to_timer_struct(ns_timer_id);
			if(current_timer)
			{
				/*Mark it active and it will be handled in next interrupt*/
				current_timer->timer_state = NS_TIMER_ACTIVE;
				current_timer->slots = slots;
				current_timer->remaining_slots = slots;
			}
			else
			{
				ret_val = -2;
			}
		}
	}
	else
	{
		/*No timers running*/
		current_timer = ns_timer_get_pointer_to_timer_struct(ns_timer_id);
		if(current_timer)
		{
			current_timer->timer_state = NS_TIMER_HOLD;
			current_timer->slots = slots;
			current_timer->remaining_slots = slots;
			/*Start next timeout*/
			ns_timer_get_next_running_to();
		}
		else
		{
			ret_val = -1;
		}
	}
	platform_exit_critical();
	return ret_val;
}

void ns_timer_interrupt_handler(void)
{
	ns_timer_struct *current_timer;

	uint8_t i = 0;

	platform_enter_critical();
	/*Clear timer running state*/
	ns_timer_state &= ~NS_TIMER_RUNNING;
	/*Mark active timers as NS_TIMER_RUN_INTERRUPT, interrupt functions are called at the end of this function*/
	current_timer = ns_timer_list;
	while(current_timer)
	{
		if(current_timer->timer_state == NS_TIMER_ACTIVE)
		{
			current_timer->timer_state = NS_TIMER_RUN_INTERRUPT;
			/*For optimisation, count the found timers*/
			i++;
		}
		current_timer = current_timer->next_timer;
	}

	/*Start next timeout*/
	ns_timer_get_next_running_to();

	/*Call interrupt functions*/
	current_timer = ns_timer_list;
	while(current_timer && i)
	{
		if(current_timer->timer_state == NS_TIMER_RUN_INTERRUPT)
		{
			current_timer->timer_state = NS_TIMER_STOP;
			current_timer->interrupt_handler(current_timer->ns_timer_id, current_timer->slots);
			i--;
		}
		current_timer = current_timer->next_timer;
	}

	platform_exit_critical();
}

int8_t eventOS_callback_timer_stop(int8_t ns_timer_id)
{
	uint16_t pl_timer_remaining_slots;
	uint8_t active_timer_found = 0;
	ns_timer_struct *current_timer;
	ns_timer_struct *timer_tmp = 0;
	int8_t retval = -1;


	platform_enter_critical();
	/*Find timer with given timer ID*/
	current_timer = ns_timer_get_pointer_to_timer_struct(ns_timer_id);
	if(current_timer)
	{
		/*If not already stopped*/
		if(current_timer->timer_state != NS_TIMER_STOP)
		{
			current_timer->timer_state = NS_TIMER_STOP;
			current_timer->remaining_slots = 0;
			/*Check if some timer is already active*/
			current_timer = ns_timer_list;
			while(current_timer)
			{
				if(current_timer->timer_state == NS_TIMER_ACTIVE)
				{
					active_timer_found = 1;
					break;
				}
				current_timer = current_timer->next_timer;
			}
			/*If no active timers found, start one*/
			if(!active_timer_found)
			{
				current_timer = ns_timer_list;
				pl_timer_remaining_slots = platform_timer_get_remaining_slots();
				/*Find hold-labelled timer with the less remaining slots*/
				while(current_timer)
				{
					if(current_timer->timer_state == NS_TIMER_HOLD)
					{
						current_timer->remaining_slots += pl_timer_remaining_slots;

						if(timer_tmp)
						{
							if(current_timer->remaining_slots < timer_tmp->remaining_slots)
							{
								timer_tmp = current_timer;
							}
						}
						else
						{
							timer_tmp = current_timer;
						}
					}
					current_timer = current_timer->next_timer;
				}
				/*If hold-labelled timer found, set it active and start the HAL driver*/
				if(timer_tmp)
				{
					timer_tmp->timer_state = NS_TIMER_ACTIVE;
					/*Start HAL timer*/
					ns_timer_start_pl_timer(timer_tmp->remaining_slots);
					/*If some of the hold-labelled timers has the same remaining slots as the timer_tmp, mark it active*/
					current_timer = ns_timer_list;
					while(current_timer)
					{
						if(current_timer->timer_state == NS_TIMER_HOLD)
						{
							if(current_timer->remaining_slots == timer_tmp->remaining_slots)
							{
								current_timer->timer_state = NS_TIMER_ACTIVE;
							}
						}
						current_timer = current_timer->next_timer;
					}
				}
			}
		}
		retval = 0;
	}
	platform_exit_critical();

	return retval;
}
