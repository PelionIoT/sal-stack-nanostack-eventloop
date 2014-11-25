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


typedef enum ns_timer_state_e
{
	NS_TIMER_ACTIVE = 0,
	NS_TIMER_HOLD,
	NS_TIMER_RUN_INTERRUPT,
	NS_TIMER_STOP
}ns_timer_state_e;

typedef struct ns_timer_struct
{
	int8_t ns_timer_id;
	ns_timer_state_e timer_state;
	uint16_t slots;
	uint16_t remaining_slots;
	void (*interrupt_handler)(int8_t, uint16_t);
	struct ns_timer_struct *next_timer;
} ns_timer_struct;

extern int8_t ns_timer_init(void);
extern void ns_timer_interrupt_handler(void);
extern ns_timer_struct *ns_timer_get_pointer_to_timer_struct(int8_t timer_id);
extern int8_t ns_timer_sleep(void);

#endif /*NS_TIMER_H_*/
