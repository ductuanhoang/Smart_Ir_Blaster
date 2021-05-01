/*
 * logger_file.c
 *
 *  Created on: Nov 12, 2020
 *      Author: Yolo
 */

/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes <System Includes>
***********************************************************************************************************************/
#include "../../Common.h"

#include "../spiffs_api.h"
#include "../spiffs/src/spiffs.h"
#include "../spiffs/src/spiffs_nucleus.h"

//#include "Spiffs/spiffs_api.h"

#include "logger_file.h"
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/
#define FILE_PATH "/spiffs/"
/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
static uint8_t check_map_size(void);

esp_vfs_spiffs_conf_t conf = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 5,
    .format_if_mount_failed = true};

static esp_err_t ret;
static FILE *f;

static void parse_hex_data(uint16_t * hex_data, char *buffer_data, uint16_t * length);
static uint8_t hexCharToInt8u(uint8_t hexChar);
/***********************************************************************************************************************
* Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
* Imported global variables and functions (from other files)
***********************************************************************************************************************/
void logger_LoadInfo_WifiStation(Device_Data_t *data, bool smart_config)
{
    //
    FILE *file;
    char buffer[256];
    memset(buffer, 0x00, sizeof(buffer));

    if (smart_config == true)
    {
        APP_LOGI("smart config save data");
        APP_LOGI("DeviceData.WifiStation_Info.UserName: %s", DeviceData.WifiStation_Info.UserName);
        APP_LOGI("DeviceData.WifiStation_Info.Password: %s", DeviceData.WifiStation_Info.Password);

        file = fopen("/spiffs/config.txt", "w");
        //ghi de gia tri smartconfig vao
        sprintf(buffer, "%s\n", DeviceData.WifiStation_Info.UserName);
        fputs(buffer, file);

        memset(buffer, 0x00, sizeof(buffer));

        sprintf(buffer, "%s\n", DeviceData.WifiStation_Info.Password);
        fputs(buffer, file);

        memset(buffer, 0x00, sizeof(buffer));

        sprintf(buffer, "%d\n", DeviceData.eCtrlStateDevice);
        fputs(buffer, file);

        memset(buffer, 0x00, sizeof(buffer));
        fclose(file);
    }
    else
    {
        file = fopen("/spiffs/config.txt", "r");
        if (file == NULL)
        {
            sprintf(DeviceData.WifiStation_Info.UserName, "%s", "thao123");
            sprintf(DeviceData.WifiStation_Info.Password, "%s", "123456789");
            DeviceData.eCtrlStateDevice = E_DEVICE_CONTROL_LOCAL;

            APP_LOGE("Error opening file");
            APP_LOGI("Create new file save param");
            //create new file and save old
            file = fopen("/spiffs/config.txt", "w");
            //save param
            //ghi de gia tri smartconfig vao
            sprintf(buffer, "%s\n", DeviceData.WifiStation_Info.UserName);
            fputs(buffer, file);

            memset(buffer, 0x00, sizeof(buffer));

            sprintf(buffer, "%s\n", DeviceData.WifiStation_Info.Password);
            fputs(buffer, file);

            memset(buffer, 0x00, sizeof(buffer));

            sprintf(buffer, "%d\n", DeviceData.eCtrlStateDevice);
            fputs(buffer, file);

            fclose(file);
        }
        else
        {
            APP_LOGD("file da ton tai");
            // da ton tai file
            int length = 0;
            int number_elements = 0;
            // doc thong tin user/password
            while (fgets(buffer, sizeof(buffer), file))
            {
            	switch (number_elements)
            	{
					case 0:
					{
		                length = strlen(buffer);
		                if (buffer[length - 1] == '\n')
		                    buffer[length - 1] = 0x00;
		                APP_LOGI("buffer = %s", buffer);
		                sprintf(DeviceData.WifiStation_Info.UserName, "%s", buffer);
		                memset(buffer, 0x00, sizeof(buffer));
						break;
					}
					case 1:
					{
			            length = strlen(buffer);
			            if (buffer[length - 1] == '\n')
			                buffer[length - 1] = 0x00;
			            APP_LOGI("buffer = %s", buffer);
			            sprintf(DeviceData.WifiStation_Info.Password, "%s", buffer);
			            memset(buffer, 0x00, sizeof(buffer));
						break;
					}
					case 2:
					{
					    length = strlen(buffer);
						if (buffer[length - 1] == '\n')
							buffer[length - 1] = 0x00;
						APP_LOGI("buffer = %s", buffer);
						DeviceData.eCtrlStateDevice = atoi(buffer);
						APP_LOGI("DeviceData.eCtrlStateDevice = %d", DeviceData.eCtrlStateDevice);
						memset(buffer, 0x00, sizeof(buffer));
						break;
					}
					case 3:
						break;
					default:
						break;
				}
            	number_elements++;
            }
            fclose(file);
        }
    }
}
/***********************************************************************************************************************
* Function Name: 
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
uint16_t count = 0;
void logger_file_test(void)
{
    bool status = check_map_size();

    if (status == false)
    {
        APP_LOGE("file not exist");
        return;
    }
    // First create a file.
    APP_LOGD("Opening file");
    f = fopen("/spiffs/hello2.txt", "a");
    if (f == NULL)
    {
        APP_LOGD("Failed to open file for writing");
        return;
    }
    char buffer2[] = "{\"object\":{\"a\":\"3480,1760,400,444,424,1308,420,444,424,444,424,440,424,440,424,444,420,444,420,444,424,444,448,444,420,444,420,448,420,1332,400,444,420,444,420,444,424,444,420,444,420,444,424,444,420,1332,400,1332,396,1312,420,444,452,440,424,1308,420,448,420,444,420,444,420,448,420,444,420,444,424,440,424,444,420,444,424,444,420,444,420,444,424,444,448,444,420,444,424,444,420,444,420,444,420,448,420,444,420,444,424,444,420,444,420,444,424,444,420,444,420,448,448,440,424,444,420,444,420,1336,396,1332,400,444,420,444,424,444,420,444,420,444,424,10008,3484,1736,404,440,424,1308,448,448,420,444,420,444,424,440,424,444,420,444,424,440,424,444,420,444,424,444,420,444,420,1332,400,444,424,440,452,444,420,444,420,444,424,440,424,444,424,1304,424,1332,400,1308,424,440,424,444,420,1332,400,444,424,440,424,440,424,444,424,440,448,444,424,440,424,444,424,440,424,444,420,444,424,440,424,444,424,440,424,440,424,444,424,1308,420,1308,424,1308,424,440,452,440,424,440,428,440,424,1308,424,1304,424,444,424,1304,424,444,424,440,428,440,424,440,428,436,428,436,428,440,428,436,456,436,428,1304,424,1308,424,440,428,436,428,440,428,1300,428,1304,428,440,424,440,428,436,428,440,424,440,428,436,428,440,452,440,424,440,428,440,424,440,428,436,428,440,424,440,428,436,428,440,424,440,424,440,428,440,424,1308,424,1300,432,1304,452,440,424,440,428,440,424,440,428,436,428,440,424,440,428,436,428,440,424,1328,404,1304,428,1304,424,440,428,436,428,440,452,440,428,436,428,440,424,440,428,436,428,440,428,436,428,436,428,440,424,440,428,436,428,440,424,440,428,1304,452,440,428,436,428,1304,428,436,428,440,424,440,428,1304,428,436,428,440,424,440,428,436,428,440,424,440,428,436,428,440,452,440,428,436,428,436,428,440,428,436,428,440,424,440,428,436,428,440,428,1300,428,440,424,440,428,1300,428,440,428,436,456,1324,408\"},\"string\":\"HelloWorld\"}";
    char buffer[] = "tuan";
    char buffer3[2048];
    sprintf(buffer3, "%s%d!%d\r\n", buffer2, count, count);
    count++;
    int step = 20;
    uint16_t length_buffer = strlen(buffer2);
    fputs(buffer3, f);
    //    for (int var = 0; var < max; ++int var) {
    //
    //	}
    fclose(f);
    APP_LOGD("File written size");
}


bool logger_file_save_string_cmd(char *file_name, char *cmd)
{
    bool status = check_map_size();
    char buffer_file_name[50];

    if (status == false)
    {
        APP_LOGE("file not exist");
        return status ;
    }
//    if( logger_file_check_cmd(file_name, cmd) == true)
//    {
//        APP_LOGE("command exist in file");
//        return status = false;
//    }
    // First create a file.
    sprintf(buffer_file_name, "%s%s.txt", FILE_PATH, file_name);
    APP_LOGD("Opening file: %s", buffer_file_name);

    f = fopen(buffer_file_name, "a");
    if (f == NULL)
    {
        APP_LOGD("Failed to open file for writing");
        return status = false;
    }

    char buffer3[2048];
    sprintf(buffer3, "%s\r\n", cmd);

    fputs(buffer3, f);
    APP_LOGI("buffer = %s", buffer3);
    fclose(f);

    return status = true;
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
bool logger_file_save_cmd(char *file_name, char *cmd, uint8_t type_protocol,int data_cmd[], int length)
{
    bool status = check_map_size();
    char buffer_file_name[50];

    if (status == false)
    {
        APP_LOGE("file not exist");
        return status ;
    }
    if( logger_file_check_cmd(file_name, cmd) == true)
    {
    	APP_LOGE("command exist in file");
        return status = false;
    }
    // First create a file.
    sprintf(buffer_file_name, "%s%s.txt", FILE_PATH, file_name);
    APP_LOGD("Opening file: %s", buffer_file_name);

    f = fopen(buffer_file_name, "a");
    if (f == NULL)
    {
        APP_LOGD("Failed to open file for writing");
        return status = false;
    }
    //    char buffer2[] = "4406,4319,568,1598,567,546,547,1597,569,1597,567,546,547,541,542,1623,542,547,546,540,543,1622,543,545,548,539,543,1623,542,1597,568,546,547,1625,551,1587,567,547,546,1597,568,1598,578,1589,576,1590,575,1591,574,1593,572,540,542,1628,548,539,543,545,569,518,575,513,570,517,576,513,569,517,576,1593,572,516,577,510,573,515,578,509,574,513,569,521,572,1591,574,514,569,1601,574,1565,600,1566,599,1567,598,1568,597,1569,596,5145,4440,4291,596,1570,595,518,575,1595,570,1570,595,518,575,513,570,1595,570,520,573,512,570,1596,569,518,575,513,570,1595,570,1570,605,509,574,1597,578,1560,595,519,574,1595,570,1570,605,1561,604,1562,603,1563,602,1566,599,512,570,1600,576,511,571,517,576,511,572,516,577,510,572,517,576,510,573,1596,569,519,574,513,570,518,575,512,570,517,576,514,569,1594,571,517,576,1593,572,1568,597,1569,596,1570,626,1540,625,1543,570,0";
    char buffer3[2048];
    sprintf(buffer3, "%s!%d!%d!%d!", cmd, ePROTOCOL_RAW, 38000, length); // "ON!0!38!28!0x12345";
    for (int i = 0; i < length; i++)
    {
        if (i == (length - 1))
        {
            sprintf(buffer3, "%s%d", buffer3, data_cmd[i]);
        }
        else
        {
            sprintf(buffer3, "%s%d,", buffer3, data_cmd[i]);
        }
    }
    sprintf(buffer3, "%s\r\n", buffer3);
    fputs(buffer3, f);
    APP_LOGI("buffer = %s", buffer3);
    fclose(f);

    return status = true;
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void logger_file_save(char *file_name)
{
    APP_LOGD("Reading file");
    FILE *fileptr;
    int count_lines = 0;
    char filechar[40], chr;
    fseek(f, 0, SEEK_SET);
//    ESP_LOGI(TAG, "Open eof f %d", feof(f));

    fileptr = fopen("/spiffs/hello2.txt", "r");
    //extract character from file and store in chr
    chr = getc(fileptr);
    while (chr != EOF)
    {
        //Count whenever new line is encountered
        if (chr == 'n')
        {
            count_lines = count_lines + 1;
        }
        //take next character from file.
        chr = getc(fileptr);
    }
    fclose(fileptr); //close file.
    printf("There are %d lines in %s  in a file\n", count_lines, filechar);

    fclose(f);
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
bool logger_file_delete(char *file_name)
{
    bool status = false;
    char buffer_file_name[50];
    memset(buffer_file_name, 0x00, sizeof(buffer_file_name));

    sprintf(buffer_file_name, "%s%s.txt", FILE_PATH, file_name);
    if (logger_list_file(file_name, FILE_PATH) == true)
    {
        // TODO delete file here
        remove(buffer_file_name);
        APP_LOGD("remove done");
    }
    else
    {
        APP_LOGE("remove err");
        status = true; // not file
    }
    return status;
}

/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/

bool ReadLine(char *buff, int size, FILE *fp)
{
    buff[0] = '\0';
    buff[size - 1] = '\0'; /* mark end of buffer */
    char *tmp;

    if (fgets(buff, size, fp) == NULL)
    {
        *buff = '\0'; /* EOF */
        return false;
    }
    else
    {
        /* remove newline */
        if ((tmp = strrchr(buff, '\n')) != NULL)
        {
            *tmp = '\0';
        }
    }
    return true;
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/

void logger_file_init(void)
{
    ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            APP_LOGE("Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            APP_LOGE("Failed to find SPIFFS partition");
        }
        else
        {
            APP_LOGE("Failed to initialize SPIFFS (%d)", ret);
        }
    }
}
/***********************************************************************************************************************
* Function Name: logger_list_file
* Description  : 
* Arguments    : file_name name of file need check
                 dir dir of file name
* Return Value : true/false
***********************************************************************************************************************/
bool logger_list_file(char *file_name, char *location)
{
	bool check_map = check_map_size();
    DIR *d;
    struct dirent *dir;
    bool status = true;
    d = opendir(location);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            APP_LOGD("**** %s\n", dir->d_name);
            if (file_name == NULL)
            {
            }
            else if (strcmp(file_name, dir->d_name) == 0)
            {
                APP_LOGD("**** file name %s have exist", dir->d_name);
                status = false;
            }
        }
        closedir(d);
    }
    return status;
}

/***********************************************************************************************************************
* static functions
***********************************************************************************************************************/
static uint8_t check_map_size(void)
{
    bool status = false;
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
        status = false;
        APP_LOGD("Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    }
    else
    {
        status = true;
        APP_LOGD("Partition size: total: %d Mb, used: %d", total, used);
    }
    return status;
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void logger_read_number_line(char *file_name)
{
    FILE *fp;
    fp = fopen(file_name, "r");
    APP_LOGD("logger_read_number_line\n");
    char buf[256 + 1];
    int number_line = 0;

    if (fp == NULL)
    {
        APP_LOGE("Xay ra loi trong khi doc file");
        return (-1);
    }
    while (fgets(buf, 1024, fp))
    {
        number_line++;
        APP_LOGD("data: %s", buf);
    }
    fclose(fp);
    APP_LOGD("number_line = %d", number_line);
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
uint16_t *logger_file_read_cmd(char file_name[], char cmd[],
		uint16_t *protocol_type,
		uint16_t *protocol_freq,
		uint16_t *protocol_number_bits,
		uint16_t *length) // read cmd raw cmd
{
    APP_LOGD("logger_file_read_cmd = %s --- %s", file_name, cmd);
    APP_LOGI("lengh cmd = %d", strlen(cmd));
    FILE *file;
    char buffer_file_name[50];
    char buffer_cmd[10];
    char buffer_read[1024];
    static uint16_t data_ir[400];
    bool status = false;
    memset(buffer_cmd, 0x00, sizeof(buffer_cmd));
    memset(data_ir, 0x00, sizeof(data_ir) / sizeof(int));

    sprintf(buffer_file_name, "%s%s.txt", FILE_PATH, file_name);
    APP_LOGD("test 1 = %s", buffer_file_name);
    file = fopen(buffer_file_name, "r");
    if (file == NULL)
    {
        APP_LOGE("Xay ra loi trong khi doc file");
        return (-1);
    }
    while (fgets(buffer_read, 1024, file))
    {
    	for (int i = 0; i < strlen(cmd) + 1; i++) // example cmd ON and read ON!9000
		{
			if (buffer_read[i] != '!')
			{
				buffer_cmd[i] = buffer_read[i];
			}
			else
			{
				break;
			}
		}
    	APP_LOGD("buffer_cmd = %s", buffer_cmd);
		if (strcmp(cmd, buffer_cmd) == 0)
		{
			printf("accepted data: %s\r\n", buffer_read);
			status = true;
			int index = 0;
			uint16_t buffer_index = 0;
			char* p;
			p = strtok((char*)buffer_read, "!,\n"); //cat chuoi bang cac ky tu ,. va space
			while (p != NULL)
			{
				APP_LOGD("go 1");
				if (index == 0)
				{
					//TODO
				}
				else
				{
					APP_LOGD("go 2");
					switch (index)
					{
					case 1:
						APP_LOGI("p = %s", p);
						*protocol_type = atoi(p);
						APP_LOGD("go 3");
						break;
					case 2:
						*protocol_freq = atoi(p);
						break;
					case 3:
						*protocol_number_bits = atoi(p);
						break;
					default:
						if (*protocol_type == 0) // type RAW 9000,4000,300
						{
							data_ir[buffer_index++] = atoi(p);
						}
						else
						{
							parse_hex_data(data_ir, p, &buffer_index);
							printf("**p = %s\r\n", p);
						}
						break;
					}
					printf("test p = %s --- index = %d\r\n", p, index);
				}
				index++;
				p = strtok(NULL, "!,\n"); //cat chuoi tu vi tri dung lai truoc do
			}
			*length = buffer_index;
		}
		memset(buffer_cmd, 0x00, sizeof(buffer_cmd));
    }
    if (status == false)
        APP_LOGD("no line in file accepted");

    fclose(file);
    if (status == true)
        return data_ir;
    else
        return 0;
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
bool logger_file_check_cmd(char *file_name, char *cmd)
{
    APP_LOGD("logger_file_list_cmd = %s", file_name);
    FILE *file;
    char buffer_file_name[50];
    char buffer_cmd[10];
    char buffer_read[1024];
    static int data_ir[400];
    bool status = false;
    memset(buffer_cmd, 0x00, sizeof(buffer_cmd));
    memset(data_ir, 0x00, sizeof(data_ir) / sizeof(int));

    sprintf(buffer_file_name, "%s%s.txt", FILE_PATH, file_name);
    file = fopen(buffer_file_name, "r");
    if (file == NULL)
    {
        APP_LOGE("Xay ra loi trong khi doc file");
        return status;
    }
    while (fgets(buffer_read, 1024, file))
    {
        for (int i = 0; i < 20; i++) // example cmd ON and read ON!9000
        {
            if (buffer_read[i] != '!')
            {
                buffer_cmd[i] = buffer_read[i];
            }
            else
            {
                break;
            }
        }
        if( strcmp(buffer_cmd, cmd) == 0)
        {
             status = true;
        }
        APP_LOGI("command support = %s", buffer_cmd);
        memset(buffer_cmd, 0x00, sizeof(buffer_cmd));
    }
    fclose(file);

    return status;
}

/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void logger_file_delete_cmd(char *file_name, char *cmd)
{
}
/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/


static uint8_t hexCharToInt8u(uint8_t hexChar)
{
	if ('0' <= hexChar && hexChar <= '9') {
		return hexChar - '0';
	}
	else if ('a' <= hexChar && hexChar <= 'f') {
		return hexChar - 'a' + 10;
	}
	else if ('A' <= hexChar && hexChar <= 'F') {
		return hexChar - 'A' + 10;
	}
	else {
		return 0;
	}
}

static void parse_hex_data(uint16_t * hex_data, char *buffer_data, uint16_t * length)
{
	printf("test = %s --- %d\r\n", buffer_data, strlen(buffer_data));
	uint16_t buffer_length = 0;
	if (!(strncmp(buffer_data, "0x", 2)))
	{
		buffer_data += 2;
		char hex[2];
		uint16_t a = 0;
		if ((strlen(buffer_data) -1) % 2)
		{
			int i = hexCharToInt8u(buffer_data[0]);
			printf("value 1 = %x\r\n", i);
			hex_data[buffer_length++] = i;
			buffer_data += 1;
		}
		while(*buffer_data)
		{
			int i = (hexCharToInt8u(buffer_data[1]) + hexCharToInt8u(buffer_data[0]) * 16);
			printf("value = %x\r\n", i);
			hex_data[buffer_length++] = i;
			buffer_data += 2;
			//buffer_length += 1;
		}

	}
	*length = buffer_length;
}
