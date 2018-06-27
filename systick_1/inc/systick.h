
#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "hw_stm32f051r8.h"

extern void systick_init(void);
extern void start_systick(unsigned int value);
extern unsigned int get_state_systick(void);
extern void delay_systick(unsigned int value);

/* end file */
#endif /* __SYSTICK_H__ */
