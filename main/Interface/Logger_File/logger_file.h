/*
 * logger_file.h
 *
 *  Created on: Nov 12, 2020
 *      Author: Yolo
 */

#ifndef MAIN_INTERFACE_LOGGER_FILE_LOGGER_FILE_H_
#define MAIN_INTERFACE_LOGGER_FILE_LOGGER_FILE_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
void logger_file_test( void );
void logger_file_init( void );
bool logger_list_file(char *file_name, char *dir);
void logger_file_save( char *file_name);
void logger_read_number_line( char *file_name );
uint16_t *logger_file_read_cmd(char file_name[], char cmd[],
		uint16_t *protocol_type,
		uint16_t *protocol_freq,
		uint16_t *protocol_number_bits,
		uint16_t *length); // read cmd raw cmd


bool logger_file_save_cmd(char *file_name, char *cmd, uint8_t type_protocol, int data_cmd[], int length);
bool logger_file_save_string_cmd(char *file_name, char *cmd);

void logger_LoadInfo_WifiStation(Device_Data_t *data, bool smart_config);

bool logger_file_check_cmd(char *file_name, char *cmd);

bool logger_file_delete(char *file_name);

#endif /* MAIN_INTERFACE_LOGGER_FILE_LOGGER_FILE_H_ */
