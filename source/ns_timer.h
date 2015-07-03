/*
 * Copyright (c) 2014 ARM. All rights reserved.
 */
#ifndef NS_TIMER_H_
#define NS_TIMER_H_

typedef struct sys_timer_struct_s
{
	uint32_t timer_sys_launch_time;
	int8_t timer_sys_launch_receiver;
	uint8_t timer_sys_launch_message;
	uint8_t timer_event_type;
	struct sys_timer_struct_s *next;
} sys_timer_struct_s;

extern int8_t ns_timer_init(void);
extern int8_t ns_timer_sleep(void);

#endif /*NS_TIMER_H_*/
