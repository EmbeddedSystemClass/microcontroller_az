
#include "cli.h"

/* ============================== Global Variable ================== */
static cli_t *data_list_command = NULL;

/* ============================== API ============================== */

/*
 * Name function: help
 * Brief        :
 * Parameter    :
 * Return       : None
*/
void help(u8_t argc, u8_t *argv)
{

}


/*
 * Name function:
 * Brief        :
 * Parameter    :
 * Return       : None
*/
void init_cli(void)
{
    cli_t *temp_command = (cli_t *)malloc(sizeof(cli_t));
    const u8_t *help_command = "help";
    const u8_t *help_desciption = "Show all commands on terminal";
    
    if(NULL == temp_command)
    {
        /* lock program here, because it can't allocate new
            area memory for this variable */
        while(1);
    }
    temp_command->command = help_command;
    temp_command->entry_function = help;
    temp_command->num_in_par = 0;
    temp_command->description = help_desciption;
    temp_command->next_command = NULL;
    
    data_list_command = temp_command;
}


/*
 * Name function: add_cli
 * Brief        :
 * Parameter    :
 * Return       : None
*/
void add_cli(cli_t *new_command)
{
}


/*
 * Name function: parse_cli
 * Brief        :
 * Parameter    :
 * Return       : None
*/
void parse_cli(cli_t *new_command)
{
}


