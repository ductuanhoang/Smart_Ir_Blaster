/* esp_timer (high resolution timer) example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "UserTimer.h"
#include <stdbool.h>

//int Counter_msec = 0;	// conter millisec

static void periodic_timer_callback(void* arg);
void piezo_timer_callback( void *arg);
void piezo_timer_clear_callback( void *arg);

static uint32_t tick_time = 0;
esp_timer_handle_t oneshot_timer;
esp_timer_handle_t piezo_timer;
esp_timer_handle_t piezo_timer_clear;

void UserTimer_Init()
{
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_timer_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "periodic"
    };

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    /* The timer has been created but is not running yet */


    /* Start the timers */
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, TIMER_INTERVAL));
//    /* Clean up and finish the example */
//    ESP_ERROR_CHECK(esp_timer_stop(periodic_timer));
//    ESP_ERROR_CHECK(esp_timer_delete(periodic_timer));
}


void piezo_timer_init( void )
{

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &piezo_timer_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "timer_piezo"
    };

    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &piezo_timer));
    /* The timer has been created but is not running yet */
//    /* Clean up and finish the example */
//    ESP_ERROR_CHECK(esp_timer_stop(periodic_timer));
//    ESP_ERROR_CHECK(esp_timer_delete(periodic_timer));
}

void piezo_timer_clear_init ( void )
{
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &piezo_timer_clear_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "timer_piezo_clear"
    };

    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &piezo_timer_clear));
}

void piezo_timer_disable( void )
{
	ESP_ERROR_CHECK(esp_timer_stop(piezo_timer));
}

void piezo_timer_enable( void )
{
	ESP_ERROR_CHECK(esp_timer_start_periodic(piezo_timer, TIMER_INTERVAL_PIEZO));
}

void piezo_timer_clear_disable( void )
{
    ESP_ERROR_CHECK(esp_timer_stop(piezo_timer_clear));
}

void piezo_timer_clear_enable( void )
{
    ESP_ERROR_CHECK(esp_timer_start_periodic(piezo_timer_clear, TIMER_INTERVAL_PIEZO_CLEAR));
}

uint32_t usertimer_gettick( void )
{
	return tick_time;
}
extern volatile bool timer1_driving_flag;
extern volatile bool timer1_clear_driving_flag;

void piezo_timer_callback( void *arg)
{
	timer1_driving_flag = 1;
}

void piezo_timer_clear_callback( void *arg)
{
    timer1_clear_driving_flag = 1;
}

static void periodic_timer_callback(void* arg)
{
	tick_time++;
}










