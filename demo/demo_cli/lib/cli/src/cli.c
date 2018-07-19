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

/*
#include <stddef.h>
#include <stdio.h>
*/
#include "stdlib.h"
#include "string.h"

#include "cli.h"

/* ============================== Global Variable ================== */
static cli_t *data_list_command = NULL;
static cli_t *start_point_command = NULL;

/* ============================== API ============================== */

/*
 * Name function    : help
 * Brief            :
 * Parameter        :
 * Return           : None
*/
void help(u8_t argc, u8_t **argv)
{
    cli_t *temp_command = start_point_command;

    while(temp_command != NULL)
    {
        if (0 == argc)
        {
            #ifdef CLI_STAND_ALONE
            usart_send_string("\n\r");
            usart_send_string(temp_command->command);
            usart_send_string("\n\r\t");
            usart_send_string(temp_command->description);
            #else
            printf("\n\r%s\t: %s", temp_command->command, temp_command->description);
            #endif /* CLI_STAND_ALONE */

        }
        else
        {
            if (!(strcmp (argv, temp_command->command)))
            {
                #ifdef CLI_STAND_ALONE
                usart_send_string("\n\r");
                usart_send_string(temp_command->command);
                usart_send_string("\n\r\t");
                usart_send_string(temp_command->description);
                #else
                printf("\n\r%s\t: %s", temp_command->command, temp_command->description);
                #endif /* CLI_STAND_ALONE */
            }
        }
        temp_command = temp_command->next_command;
    }
}


/*
 * Name function    :
 * Brief            :
 * Parameter        :
 * Return           : None
*/
void init_cli(void)
{
    const u8_t *help_command = "help";
    const u8_t *help_desciption = "Show all commands on the terminal";
    cli_t *temp_command = (cli_t *)malloc(sizeof(cli_t));

    if(NULL == temp_command)
    {
        /* lock program here, because it can't allocate new
            area memory for this variable */
        while(1);
    }
    temp_command->command = help_command;
    temp_command->entry_function = help;
    temp_command->num_input_par = 0;
    temp_command->description = help_desciption;
    temp_command->next_command = NULL;

    data_list_command = temp_command;
    start_point_command = temp_command;
}


/*
 * Name function    : add_cli
 * Brief            :
 * Parameter        :
 * Return           : None
*/
void add_cli(cli_t *new_command)
{
    data_list_command->next_command = new_command;
    data_list_command = data_list_command->next_command;
    data_list_command->next_command = NULL;
}


/*
 * Name function    : parse_cli
 * Brief            :
 * Parameter        :
 * Return           : None
*/
void parse_cli(const u8_t *str_command, const u8_t len_command)
{
    u8_t *temp_str = NULL;
    u8_t num_of_input = 0;
    cli_t *temp_command = start_point_command;
    u8_t **input_parameter;
    u8_t *t_str_command = NULL;

    /* copy string command from input parameter into local variable */
    t_str_command = malloc(sizeof(u8_t) * len_command);
    memcpy(t_str_command, str_command, len_command);

    temp_str = strtok(t_str_command, DELIMITER_CHARACTERS);
    while(temp_command != NULL)
    {
        if (!(strcmp (temp_str, temp_command->command)))
        {
            input_parameter = malloc(sizeof(u32_t) * temp_command->num_input_par);
            num_of_input = 0;
            temp_str = strtok(NULL, DELIMITER_CHARACTERS);
            while(NULL != temp_str)
            {
                if( num_of_input > temp_command->num_input_par)
                {
                    break;
                }
                else
                {
                    input_parameter[num_of_input] = temp_str;
                    temp_str = strtok(NULL, DELIMITER_CHARACTERS);
                    num_of_input++;
                }
            }
            break;
        }
        temp_command = temp_command->next_command;
    }
    if (NULL != temp_command)
    {
        if (num_of_input == temp_command->num_input_par)
        {
            temp_command->entry_function(num_of_input, input_parameter);
        }
        else
        {
            /* message error */
            #ifdef CLI_STAND_ALONE
            usart_send_string("\n\rWrong number of input parameter of command !");
            #else
            printf("\n\rWrong number of input parameter of command: %s !", temp_command->command);
            #endif /* CLI_STAND_ALONE */
        }
        free(input_parameter);
    }
    else
    {
        /* don't support the command */
        #ifdef CLI_STAND_ALONE
        usart_send_string("\n\rDon't support the command !");
        #else
        printf("\n\rDon't support the command !");
        #endif /* CLI_STAND_ALONE */
    }
    free(t_str_command);

}


