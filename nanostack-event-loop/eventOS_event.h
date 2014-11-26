/*
 * \file eventOS_event.h
 * \brief Add short description about this file!!!
 * Copyrigth Sensinode Ltd 2013
 *
 */

#ifndef EVENTOS_EVENT_H_
#define EVENTOS_EVENT_H_
 #ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"
/**
 * \enum arm_library_event_priority_e
 * \brief Event Priority level.
 */
typedef enum arm_library_event_priority_e
{
	ARM_LIB_HIGH_PRIORITY_EVENT = 0, /**< High Priority Event (Function CB) */
	ARM_LIB_MED_PRIORITY_EVENT = 1,	/**< Medium Priority (Timer) */
	ARM_LIB_LOW_PRIORITY_EVENT = 2, /*!*< Normal Event and ECC / Security */
} arm_library_event_priority_e;

/**
 * \enum arm_event_s
 * \brief Event structure.
 */
typedef struct arm_event_s
{
	int8_t receiver; /**< Event handler Tasklet ID */
	int8_t sender; /**< Event sender Tasklet ID */
	uint8_t event_type; /**< This will be typecast arm_library_event_type_e */
	uint8_t event_id; /**< Timer ID, NWK interface ID or application specific ID */
	void *data_ptr; /**< Application could share data pointer tasklet to tasklet */
	void (*cb_fptr)(uint8_t); /**< Application could share data pointer tasklet to tasklet */
	arm_library_event_priority_e priority;
	uint32_t event_data;
} arm_event_s;

/**
 * \brief Send event to  event scheduler.
 *
 * \param event pointer to pushed event.
 *
 * \return 0 Event push OK
 * \return -1 Memory allocation Fail
 *
 */
extern int8_t eventOS_event_send(arm_event_s *event);

/**
 * \brief Event handler callback register
 *
 * Function will register and allocate unique event id handler
 *
 * \param handler_func_ptr function pointer for event handler
 * \param init_event_type generated evevnt type for init purpose
 *
 * \return >= 0 Unique event ID for this handler
 * \return < 0 Register fail
 *
 * */
extern int8_t eventOS_event_handler_create(void (*handler_func_ptr)(arm_event_s*), uint8_t init_event_type);
#ifdef __cplusplus
}
#endif
#endif /* EVENTOS_EVENT_H_ */
