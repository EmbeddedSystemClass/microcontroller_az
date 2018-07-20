/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Anh Vo Tuan <votuananhs@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include "project.h"
#include "driver.h"
#include "clock.h"
#include "gpio.h"
#include "interrupt.h"
#include "systick.h"
#include "usart.h"
#include "cli.h"

extern volatile unsigned char rx_data;


/*************************************************************************************************/
#define NUM_LIST_PINS       4
const gpio_pin_t init_list_pins[NUM_LIST_PINS] =
{
    /* led 3 - PC9 */
    {
        LD3_PORT,
        LD3_PIN,
        GPIO_MODE_OPTION_OUTPUT,
    },
    /* led 4 - PC8 */
    {
        LD4_PORT,
        LD4_PIN,
        GPIO_MODE_OPTION_OUTPUT,
    },
    /* Tx - PA9 */
    {
        PORT_A,
        9,
        GPIO_MODE_OPTION_ALT | GPIO_MODE_OPTION_AF1,
    },
    /* Rx - PA10 */
    {
        PORT_A,
        10,
        GPIO_MODE_OPTION_ALT | GPIO_MODE_OPTION_AF1,
    },
};

cli_t list_commands[] =
{
        /* control led on/off */
        {
            "led",
            led_func,
            2,
            "Control led on or off. \n\r\tSyntax: led <on/off> <led_id>",
            NULL,
        },
        /* print message in Terminal app */
        {
            "test",
            test_func,
            0,
            "Print call test_func() in terminal",
            NULL,
        },
};

/*************************************************************************************************/

void delay(unsigned int timeout)
{
    unsigned int t1, t2;
    for (t1 = 0; t1 < timeout; t1++)
    {
        for (t2 = 0; t2 < 0xFFF; t2++)
        {
          asm(" nop");
        }
    }
}

void led_on(u8_t led_id)
{
    if (LD3 == led_id)
    {
        write_pin(LD3_PORT, LD3_PIN, HIGH);
        usart_send_string("--> Turn on LED3 successfully\n\r");
    }
    else if (LD4 == led_id)
    {
        write_pin(LD4_PORT, LD4_PIN, HIGH);
        usart_send_string("--> Turn on LED4 successfully\n\r");
    }
    else
    {
        usart_send_string("--> Don't support the led_id\n\r");
    }
}

void led_off(u8_t led_id)
{
    if (LD3 == led_id)
    {
        write_pin(LD3_PORT, LD3_PIN, LOW);
        usart_send_string("--> Turn off LED3 successfully\n\r");
    }
    else if (LD4 == led_id)
    {
        write_pin(LD4_PORT, LD4_PIN, LOW);
        usart_send_string("--> Turn off LED4 successfully\n\r");
    }
    else
    {
        usart_send_string("--> Don't support the led_id\n\r");
    }
}

void led_func(u8_t argc, u8_t **argv)
{
    u8_t temp = argv[1][0] - '0';

    if(!strcmp("on", argv[0]))
    {
        led_on(temp);
    }
    else if(!strcmp("off", argv[0]))
    {
        led_off(temp);
    }
    else
    {
        /* nothing to be run here */
        /* printf("\n\rDon't support the input parameter !"); */
    }
}

void test_func(u8_t argc, u8_t **argv)
{
    usart_send_string("Call test_func\n\r");
}

void main(void)
{
    system_init(PREDIV_1, PLL_MUL_12, PLL_SCR_HSI, PREDIV_1, SYSCLKSOURCE_PLLCLK, APB1_HCLK_1);
    enabled_clock();
    init_pin(init_list_pins, NUM_LIST_PINS);
    usart_init();
    systick_init();
    interrupt_init();

    delay(0xff);
    usart_send_string("---> Command Line Interface - CLI <---\n\r");

    init_cli();
    add_cli(list_commands, sizeof(list_commands)/sizeof(cli_t));

    clear_buffer_cli();
    while(1)
    {
        if(0 != rx_data)
        {
            usart_send_string("\n\r");
            prepare_command_cli(rx_data);
            rx_data = 0;
        }
    }
}

