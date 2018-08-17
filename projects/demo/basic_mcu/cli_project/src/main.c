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

extern volatile uint8_t rx_data;

volatile unsigned int led_state = 0;
volatile unsigned char rx_data = 0;

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

void EXTI0_1_IRQHandler(void)
{
    volatile unsigned int temp;
    /* Ngat cua PA0 */
    temp = read_reg(EXTI_PR, (1 << 0));
    if (1 == temp)
    {
        if(0 == led_state)
        {
            led_on(LD3);
            led_state = 1;
        }
        else
        {
            led_off(LD3);
            led_state = 0;
        }
    }
    /* xoa co ngat */
    temp = 1 << 0;
    write_reg(EXTI_PR, temp);
    write_reg(NVIC_ICPR, (1 << 5));
}

void EXTI2_3_IRQHandler(void)
{
    volatile unsigned int temp;
    /* xoa co ngat */
    temp = 1 << 2;
    write_reg(EXTI_PR, temp);
    write_reg(NVIC_ICPR, (1 << 6));
}

void USART1_IRQHandler(void)
{
    volatile unsigned int temp;

    temp = read_reg(USART_ISR, 1 << 6);
    if (0 != temp) /* Tx - TC flag */
    {
        write_reg(USART_ICR, (1<<6));
    }
    temp = read_reg(USART_ISR, 1 << 5);
    if (0 != temp)  /* Rx - RXNE flag */
    {
        rx_data = read_reg(USART_RDR, 0x000000FF);
        write_reg(USART_RQR, (1<<3)); /* ghi giá trị 1 tới USART_RQR[RXFRQ] để xóa cờ USART_ISR[RXNE] */
        usart_send_byte(rx_data);
    }
}

/*************************************************************************************************/

void led_on(uint8_t led_id)
{
    if (LD3 == led_id)
    {
        write_pin(LD3_PORT, LD3_PIN, HIGH);
        usart_send_string("\n\r--> Turn on LED3 successfully");
    }
    else if (LD4 == led_id)
    {
        write_pin(LD4_PORT, LD4_PIN, HIGH);
        usart_send_string("\n\r--> Turn on LED4 successfully");
    }
    else
    {
        usart_send_string("\n\r--> Don't support the led_id");
    }
}

void led_off(uint8_t led_id)
{
    if (LD3 == led_id)
    {
        write_pin(LD3_PORT, LD3_PIN, LOW);
        usart_send_string("\n\r--> Turn off LED3 successfully");
    }
    else if (LD4 == led_id)
    {
        write_pin(LD4_PORT, LD4_PIN, LOW);
        usart_send_string("\n\r--> Turn off LED4 successfully");
    }
    else
    {
        usart_send_string("\n\r--> Don't support the led_id");
    }
}

void led_func(uint8_t argc, uint8_t **argv)
{
    uint8_t temp = argv[1][0] - '0';

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

void test_func(uint8_t argc, uint8_t **argv)
{
    usart_send_string("\n\rCall test_func");
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
    usart_send_string("\n\r---> Command Line Interface - CLI <---\n\r");

    init_cli();
    add_cli(list_commands, sizeof(list_commands)/sizeof(cli_t));

    clear_buffer_cli();
    while(1)
    {
        if(0 != rx_data)
        {
            if (prepare_command_cli(rx_data))
            {
                usart_send_string("\n\r");
            }
            rx_data = 0;
        }
    }
}

