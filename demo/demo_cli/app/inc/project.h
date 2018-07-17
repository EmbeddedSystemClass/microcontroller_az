
#ifndef __PROJECT_H__
#define __PROJECT_H__

#include "driver.h"
#include "hw_platform.h"
#include "type.h"



/* Led LD3 - PC9 */
#define LD3                     0
#define LD3_PORT                PORT_C
#define LD3_PIN                 9
/* Led LD4 - PC8 */
#define LD4                     1
#define LD4_PORT                PORT_C
#define LD4_PIN                 8

/* user button (B1) - PA0 */
#define USER_BUTTON             0

extern void delay(unsigned int timeout);
extern void led_on(u8_t led_id);
extern void led_off(u8_t led_id);
extern void led_func(u8_t argc, u8_t **argv);

#endif
