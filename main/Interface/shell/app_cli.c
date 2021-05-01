/******************************************************************************
 * @file:    vsm_cli.c
 * @brief:
 * @version: V0.0.0
 * @date:    2019/11/12
 * @author:
 * @email:
 *
 * THE SOURCE CODE AND ITS RELATED DOCUMENTATION IS PROVIDED "AS IS". VINSMART
 * JSC MAKES NO OTHER WARRANTY OF ANY KIND, WHETHER EXPRESS, IMPLIED OR,
 * STATUTORY AND DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * SATISFACTORY QUALITY, NON INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * THE SOURCE CODE AND DOCUMENTATION MAY INCLUDE ERRORS. VINSMART JSC
 * RESERVES THE RIGHT TO INCORPORATE MODIFICATIONS TO THE SOURCE CODE IN LATER
 * REVISIONS OF IT, AND TO MAKE IMPROVEMENTS OR CHANGES IN THE DOCUMENTATION OR
 * THE PRODUCTS OR TECHNOLOGIES DESCRIBED THEREIN AT ANY TIME.
 *
 * VINSMART JSC SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGE OR LIABILITY ARISING FROM YOUR USE OF THE SOURCE CODE OR
 * ANY DOCUMENTATION, INCLUDING BUT NOT LIMITED TO, LOST REVENUES, DATA OR
 * PROFITS, DAMAGES OF ANY SPECIAL, INCIDENTAL OR CONSEQUENTIAL NATURE, PUNITIVE
 * DAMAGES, LOSS OF PROPERTY OR LOSS OF PROFITS ARISING OUT OF OR IN CONNECTION
 * WITH THIS AGREEMENT, OR BEING UNUSABLE, EVEN IF ADVISED OF THE POSSIBILITY OR
 * PROBABILITY OF SUCH DAMAGES AND WHETHER A CLAIM FOR SUCH DAMAGE IS BASED UPON
 * WARRANTY, CONTRACT, TORT, NEGLIGENCE OR OTHERWISE.
 *
 * (C)Copyright VINSMART JSC 2019 All rights reserved
 ******************************************************************************/
#include "app_cli.h"
#include "vsm_shell.h"
#include "vsm_retarget.h"

#include "../../app_test/app_test.h"

static int32_t cli_reset_system(p_shell_context_t context, int32_t argc, char **argv);

static const shell_command_context_t cli_command_table[] = {
//    {"reset", "\"reset\": reset system\r\n", cli_reset_system, 0},
	{"logger_list_cmd", "\"logger_list_cmd\": logger_list_cmd\r\n", logger_list_cmd, 1},
    {"logger_list_file", "\"logger_list_file\": logger_list_file\r\n", list_all_file, 0},
//	{"get_mac", "\"get_mac\": get_mac\r\n", get_mac, 0},
//	{"logger_delete_file", "\"logger_delete_file\": logger_delete_file\r\n", logger_delete_file, 1},
	{"read_file", "\"read_file\": read_file\r\n", read_file, 1},
//	{"read_cmd", "\"read_cmd\": read_cmd\r\n", read_cmd, 1},
//	{"json_test", "\"json_test\": json_test\r\n", json_test, 0},
//	{"smart_cfg", "\"smart_cfg\": smart_cfg\r\n", go_smart_config, 0},
//	{"ir_test", "\"ir_test\": ir_test\r\n", ir_test, 0},
	{"ir_save", "\"ir_save\": ir_save\r\n", ir_save, 3},
//	{"ir_send", "\"ir_test\": ir_send\r\n", ir_send, 2},
//	{"ir_new_cmd", "\"ir_new_cmd\": ir_new_cmd\r\n", ir_new_cmd, 0},
	{"ir_NEC_test", "\"ir_NEC_test\": ir_NEC_test\r\n", ir_NEC_test, 2},
};

shell_context_struct user_context;

int user_put_char(uint8_t *buf, uint32_t len)
{
    return user_putchar((const char *)*buf);
}

int user_get_char(uint8_t *buf, uint32_t len)
{
    uint8_t ch = user_getchar();
    *buf = ch;
    return ch;
}

void vsm_cli_start(void)
{
    SHELL_Init(&user_context,
               user_put_char,
               user_get_char,
               uart_debug_printf,
               ">");

    /* Register CLI commands */
    for (int i = 0; i < sizeof(cli_command_table) / sizeof(shell_command_context_t); i++)
    {
        SHELL_RegisterCommand(&cli_command_table[i]);
    }
}

void vsm_cli_main_loop(void *pvParameters)
{
    SHELL_Process(&user_context);
}

/* Reset System */
static int32_t cli_reset_system(p_shell_context_t context, int32_t argc, char **argv)
{
	esp32_user_restart();

    return 0;
}
