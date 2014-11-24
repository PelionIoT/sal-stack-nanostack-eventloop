#ifndef _SYSTEM_EVENT_H_
#define _SYSTEM_EVENT_H_
/*
 * Copyright ARM Ltd 2014
 */

 #ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"

 /**
  * \enum error_t
  * \brief System generic error.
  */
typedef enum error_t
{
	eOK = 0,	/*!< no error */
	eFALSE = 1, /*!< no result */
	eBUSY = 2,	/*!< resource busy */
	eSYSTEM		/*!< error code readable in sys_error */
}error_t;

typedef enum arm_library_event_priority_e
{
	ARM_LIB_HIGH_PRIORITY_EVENT = 0, /**< High Priority Event (Function CB) */
	ARM_LIB_MED_PRIORITY_EVENT = 1,	/**< Medium Priority (Timer) */
	ARM_LIB_LOW_PRIORITY_EVENT = 2, /*!*< Normal Event and ECC / Security */
} arm_library_event_priority_e;

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

typedef void (*idle_cb_t)(void *);

/**
 * \brief This function will be called when stack enter idle state and start waiting signal.
 */
extern void platform_event_os_wait(void);

/**
 * \brief This function will be called when stack receive event and could wake from idle.
 */
extern void platform_event_os_signal(void);

/**
 * \brief This function will be called when stack can enter deep sleep state in detected time.
 *
 * \param sleep_time_ms Time in milliseconds to sleep
 *
 * \return sleeped time in milliseconds
 */
extern uint32_t platform_event_os_sleep(uint32_t sleep_time_ms);

extern int8_t ns_timer_start(int8_t ns_timer_id, uint16_t slots);
extern int8_t ns_timer_stop(int8_t ns_timer_id);
extern int8_t ns_timer_sleep(void);
extern int8_t ns_timer_register(void (*timer_interrupt_handler)(int8_t, uint16_t));
extern int8_t ns_timer_unregister(int8_t ns_timer_id);

extern void set_core_timer_period(uint8_t snperiod);

extern int8_t timer_sys_wakeup(void);
extern void timer_sys_disable(void);
extern void timer_runtime_ticks_sleep_update(uint32_t sleep_ticks);
extern int8_t timer_sys_event(uint8_t snmessage, uint32_t time);
extern int8_t timer_sys_event_cancel(uint8_t snmessage);

extern void event_init(void);
extern  int8_t event_get_active_tasklet(void);
extern  void event_set_active_tasklet(int8_t tasklet);
extern int8_t arm_ns_event_send(arm_event_s *event);
extern noreturn void event_dispatch(void);
extern void event_register_idle_cb(idle_cb_t idle_cb, void *idle_cb_param);
extern int8_t arm_ns_tasklet_create(void (*tasklet_func_ptr)(arm_event_s*));


#ifdef __cplusplus
}
#endif

#endif /*_SYSTEM_EVENT_H_*/
