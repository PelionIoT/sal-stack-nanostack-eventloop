/*
 * Copyright (c) 2014 ARM. All rights reserved.
 */
#ifndef _SYS_EVENT_H_
#define _SYS_EVENT_H_
#include "eventOS_event.h"
 #ifdef __cplusplus
extern "C" {
#endif
#define RPL_TIMER_ID 0xfd
/**
 * Event callback structure
 */
typedef struct event_cb_t
{
	uint8_t event;
	void (*fptr)(uint8_t);
}event_cb_t;


typedef struct arm_core_event_s
{
	int8_t receiver; /**< Event handler Tasklet ID */
	int8_t sender; /**< Event sender Tasklet ID */
	uint8_t event_type; /**< This will be typecast arm_library_event_type_e, arm_internal_event_type_e or application specific define */
	uint8_t event_id; /**< Timer ID or NWK interface ID */
	void *data_ptr; /**< Library could share data inside or application tasklet together.*/
	uint32_t event_data;
	arm_library_event_priority_e priority;
	struct arm_core_event_s * next;
}arm_core_event_s;

 #ifdef __cplusplus
}
#endif
#endif /*_SYS_EVENT_H_*/
