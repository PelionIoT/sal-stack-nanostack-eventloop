#ifndef _SYS_EVENT_H_
#define _SYS_EVENT_H_
#include "system_event.h"

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
	void (* cb_fptr)(uint8_t);
	uint32_t event_data;
	arm_library_event_priority_e priority;
	struct arm_core_event_s * next;
}arm_core_event_s;

extern int8_t arm_ns_event_send(arm_event_s *event);
/**
 * Event loop
 */
extern noreturn void event_dispatch(void);

extern int8_t arm_ns_tasklet_create(void (*tasklet_func_ptr)(arm_event_s*));
/**
 * \brief Event dispatch loop idle Callback
 *
 *
 */
extern void arm_event_os_idle(void);
 #ifdef __cplusplus
}
#endif
#endif /*_SYS_EVENT_H_*/
