/*
 * app_test.h
 *
 *  Created on: Nov 19, 2020
 *      Author: Yolo
 */

#ifndef MAIN_APP_TEST_APP_TEST_H_
#define MAIN_APP_TEST_APP_TEST_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "../Common.h"
#include "../Interface/shell/vsm_shell.h"
#include "../Task/wifi_task.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
int32_t list_all_file(p_shell_context_t context, int32_t argc, char **argv);
int32_t get_mac(p_shell_context_t context, int32_t argc, char **argv);
int32_t file_test(p_shell_context_t context, int32_t argc, char **argv);
int32_t read_file(p_shell_context_t context, int32_t argc, char **argv);
int32_t read_cmd(p_shell_context_t context, int32_t argc, char **argv);
int32_t json_test(p_shell_context_t context, int32_t argc, char **argv);
int32_t go_smart_config(p_shell_context_t context, int32_t argc, char **argv);
int32_t ir_test(p_shell_context_t context, int32_t argc, char **argv);
int32_t ir_save(p_shell_context_t context, int32_t argc, char **argv);
int32_t ir_send(p_shell_context_t context, int32_t argc, char **argv);
int32_t logger_list_cmd(p_shell_context_t context, int32_t argc, char **argv);
int32_t logger_delete_file(p_shell_context_t context, int32_t argc, char **argv);
int32_t ir_new_cmd(p_shell_context_t context, int32_t argc, char **argv);
int32_t ir_NEC_test(p_shell_context_t context, int32_t argc, char **argv);

#endif /* MAIN_APP_TEST_APP_TEST_H_ */
