/*
 * @Author: yblin
 * @Date: 2022-03-02 10:34:19
 * @LastEditors: yblin
 * @LastEditTime: 2022-03-21 15:23:36
 * @Description: 
 */
/* esp_event (event loop library) basic example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_event_base.h"
#include "event_bus.h"
#include "bsp_task.h"
#include "msg_process_task.h"
#include "nvs_flash.h"
#include "wifi_helper/wifi_station.h"

// Declarations for the event source
#define TASK_ITERATIONS_COUNT        10      // number of times the task iterates
#define TASK_PERIOD                  500     // period of the task loop in milliseconds

static const char* TAG = "user_event_loops";

/* Event source task related definitions */
TaskHandle_t g_task;

static void task_event_source(void* args)
{
    for(int iteration = 1; iteration <= TASK_ITERATIONS_COUNT; iteration++) {
        ESP_LOGI(TAG, "posting %s:%s to %s, iteration %d out of %d", APP_EVENTS, "TASK_ITERATION_EVENT",
                "loop_with_task" ,
                iteration, TASK_ITERATIONS_COUNT);
        Event_t event;
        event.id = TASK_ITERATION_EVENT;
        event.d.iteration.num = iteration;
        ESP_ERROR_CHECK(evnet_bus_send(APP_EVENTS, event.id, &event, sizeof(Event_t), portMAX_DELAY));

        vTaskDelay(pdMS_TO_TICKS(TASK_PERIOD));
    }

    vTaskDelay(pdMS_TO_TICKS(TASK_PERIOD));

    ESP_LOGI(TAG, "deleting task event source");

    vTaskDelete(NULL);
}

/* Example main */
void app_main(void)
{
    // ESP_LOGI(TAG, "setting up");
        //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    event_bus_init();//事件总线
    bsp_task_init();//bsp处理
    msg_process_task_init();//消息处理
    // 临时代码 用于触发时间，验证event_bus
    xTaskCreate(task_event_source, "task_event_source", 2048, NULL, uxTaskPriorityGet(NULL), NULL);
    wifi_init_sta();
}
