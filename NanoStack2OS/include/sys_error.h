#ifndef _SYS_ERROR_H_
#define _SYS_ERROR_H_

/**System error types*/
typedef enum sys_error_t
{
	sMEMORY,	/*!< out of dynamic memory */
	sSTACK_OVERFLOW,	/*!< stack overflow */
	sBUSY
}sys_error_t;

/**System error value*/
extern sys_error_t sys_error;

#ifdef HAVE_DEBUG
/**
 * Print out system error in human readable form
 * */
extern void sys_error_print(void);
#else
#define sys_error_print(x)
#endif

#endif /*_SYS_ERROR_H_*/
