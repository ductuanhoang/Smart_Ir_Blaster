/*
 * tcp_server.c
 *
 *  Created on: Aug 26, 2020
 *      Author: Yolo
 */
#include "lwipopts.h"
#include <math.h>
#include "../Common.h"

#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "../../lwip/lwip/src/include/lwip/err.h"
#include "../../lwip/lwip/src/include/lwip/errno.h"
#include "../../lwip/lwip/src/include/lwip/sockets.h"
#include "../../lwip/lwip/src/include/lwip/sys.h"
#include <../../lwip/lwip/src/include/lwip/netdb.h>
#include "tcp_server.h"

#include "../json_parser/json_parser.h"
#include "../Interface/Logger_File/logger_file.h"
#define PORT 80

static const char *TAG = "piezo";
#define CONFIG_EXAMPLE_IPV4

static bool tcp_server_paser_mess(char *buffer);
static int listen_sock;
static int err = -1;
static int _sock = -1;
static void do_retransmit(const int sock)
{
    int len;
    char rx_buffer[1024];

    do
    {
        len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len < 0)
        {
            ESP_LOGE(TAG, "Error occurred during receiving: errno %d", errno);
        }
        else if (len == 0)
        {
            ESP_LOGW(TAG, "Connection closed");
        }
        else
        {
            rx_buffer[len] = 0; // Null-terminate whatever is received and treat it like a string
            ESP_LOGI(TAG, "Received %d bytes: %s", len, rx_buffer);
            //            tcp_server_paser_mess(rx_buffer);
            bool status = json_parser_message(rx_buffer, E_SOURCE_FROM_TCP);
            char *buffer;
            buffer = (char *)malloc(512 + 1);
            memset(buffer, 0x00, 512);
            buffer = json_packet_message(DeviceData.IrMessageCtr_Info.type_cmd, status);
            //            xEventGroupWaitBits(wifi_event_group, bits, false, true, 1000); // wait bit when ir message done
            // send() can return less bytes than supplied length.
            // Walk-around for robust implementation.
            APP_LOGD("message send = %s -- %d", buffer, strlen(buffer));
            int to_write = strlen(buffer);
            while (to_write > 0)
            {
                int written = send(sock, buffer, to_write, 0);
                if (written < 0)
                {
                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                }
                to_write -= written;
            }
        }

    } while (len > 0);
}

/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void tcp_server_transmit(char* DataTx, uint16_t LenTx)
{
	if(_sock >=0)
	{
		APP_LOGD("Send \"%s\" to AC\r\n", DataTx);
		uint16_t ret = send(_sock, DataTx, LenTx, 0);
		//return status send if need
	}
}

void tcp_server_task(void *pvParameters)
{
    APP_LOGD("tcp_server_task");

    char addr_str[128];
    int addr_family;
    int ip_protocol;

#ifdef CONFIG_EXAMPLE_IPV4
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;
    inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);
#else // IPV6
    struct sockaddr_in6 dest_addr;
    bzero(&dest_addr.sin6_addr.un, sizeof(dest_addr.sin6_addr.un));
    dest_addr.sin6_family = AF_INET6;
    dest_addr.sin6_port = htons(PORT);
    addr_family = AF_INET6;
    ip_protocol = IPPROTO_IPV6;
    inet6_ntoa_r(dest_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
#endif

    listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0)
    {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    ESP_LOGI(TAG, "Socket created");

    err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0)
    {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        goto CLEAN_UP;
    }
    ESP_LOGI(TAG, "Socket bound, port %d", PORT);

    err = listen(listen_sock, 1);
    if (err != 0)
    {
        ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
        goto CLEAN_UP;
    }

    while (1)
    {
        ESP_LOGI(TAG, "Socket listening");
        struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
        uint addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        _sock = sock;
        if (sock < 0)
        {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }

        // Convert ip address to string
        if (source_addr.sin6_family == PF_INET)
        {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
        }
        else if (source_addr.sin6_family == PF_INET6)
        {
            inet6_ntoa_r(source_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
        }
        ESP_LOGI(TAG, "Socket accepted ip address: %s", addr_str);

        do_retransmit(sock);

//        shutdown(sock, 0);
//        close(sock);
    }

CLEAN_UP:
    close(listen_sock);
    vTaskDelete(NULL);
}

static bool tcp_server_paser_mess(char *buffer)
{
    uint8_t freq_value[5] = {0};
    uint8_t turn_value[5] = {0};
    uint8_t value_count = 0;

    bool split_active = false;
    APP_LOGI("buffer = %s\r\n", buffer);
    uint8_t length = strlen((const char *)buffer);
    uint8_t id_field = 0;
    if (buffer[length - 1] != '!')
        return false;
    else
    {
        for (int i = 0; i < length; i++)
        {
            if (buffer[i] == ':')
            {
                id_field++;
                split_active = true;
            }
            else if ((split_active == true) && (buffer[i] != ',') && (id_field == 1))
            {
                freq_value[value_count] = buffer[i];
                value_count++;
            }
            else if ((split_active == true) && (buffer[i] != '!') && (id_field == 2))
            {
                turn_value[value_count] = buffer[i];
                value_count++;
            }
            else
            {
                if ((split_active == true) && (id_field == 1))
                {
                    split_active = false;
                    APP_LOGI("freq_value = %s\r\n", (freq_value));
                    value_count = 0;
                }
                else if ((split_active == true) && (id_field == 2))
                {
                    split_active = false;
                    APP_LOGI("turn_value = %s\r\n", (turn_value));
                    value_count = 0;
                    break;
                }
            }
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

void tcp_server_delete_task(void)
{
    if (err == 0)
        err = -1;
    if (listen_sock >= 0)
        close(listen_sock);
    if (tcp_handle_task != NULL)
        vTaskDelete(tcp_handle_task);
}

/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/

void tcp_server_create_task(void)
{
    xTaskCreate(tcp_server_task, "tcp_server_task", 4 * 1024, NULL, 10, tcp_handle_task);
}
