#include "project.h"
#include "driver.h"
#include "clock.h"
#include "gpio.h"
#include "interrupt.h"
#include "systick.h"
#include "usart.h"
#include "cli.h"


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


#define NUM_COMMAND     1
#define NUM_BUFFER      100

cli_t list_commands[NUM_COMMAND] =
{
        /* led on */
        {
            "led",
            led_func,
            2,
            "Control led on or off. \n\rSyntax: led <on/off> <led_id>",
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
    }
    else if (LD4 == led_id)
    {
        write_pin(LD4_PORT, LD4_PIN, HIGH);
    }
    else
    {
        /* nothing to be run here */
    }
}

void led_off(u8_t led_id)
{
    if (LD3 == led_id)
    {
        write_pin(LD3_PORT, LD3_PIN, LOW);
    }
    else if (LD4 == led_id)
    {
        write_pin(LD4_PORT, LD4_PIN, LOW);
    }
    else
    {
        /* nothing to be run here */
    }
}

void led_func(u8_t argc, u8_t **argv)
{
    if(!strcmp("on", argv[0]))
    {
        led_on(argv[1] - '0');
    }
    else if(!strcmp("off", argv[0]))
    {
        led_off(argv[1] - '0');
    }
    else
    {
        /* nothing to be run here */
        /* printf("\n\rDon't support the input parameter !"); */
    }
}

void main(void)
{
    u8_t i;
    u8_t t_command_led_on[] = "led on 0";
    u8_t t_command_led_off[] = "led off 0";

    system_init(PREDIV_1, PLL_MUL_12, PLL_SCR_HSI, PREDIV_1, SYSCLKSOURCE_PLLCLK, APB1_HCLK_1);
    enabled_clock();
    init_pin(init_list_pins, NUM_LIST_PINS);
    usart_init();
    systick_init();

    interrupt_init();
    
    led_on(LD3);
    delay_ms_systick(200);
    led_off(LD3);
    delay_ms_systick(200);

    usart_send_string("---> Command Line Interface - CLI <---");

    init_cli();

    for(i = 0; i < NUM_COMMAND; i++)
    {
        add_cli(&list_commands[i]);
    }

    #if 1
    while(1)
    {
        parse_cli(t_command_led_on);
        delay_ms_systick(500);
        parse_cli(t_command_led_off);
        delay_ms_systick(500);
    }
    #else
        start_systick_test(100);
    #endif
}

