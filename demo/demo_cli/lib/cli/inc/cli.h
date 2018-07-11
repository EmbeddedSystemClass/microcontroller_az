
#ifndef __CLI_H__
#define __CLI_H__

#include "type.h"

/* ============================== Type ============================= */
typedef struct _cli_t_ {
    const u8_t *command;
    const void *entry_function(u8_t argc, u8_t *argv);
    const s8_t num_in_par;
    const u8_t *description;
    struct _cli_t_ *next_command;
} cli_t;

/* ============================== Global Variable ================== */


/* ============================== API ============================== */
extern void help(u8_t argc, u8_t *argv);
extern void init_cli(void);
extern void add_cli(cli_t *new_command);



/* end file */
#endif /* __CLI_H__ */
