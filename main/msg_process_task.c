/*
 * @Author: yblin
 * @Date: 2022-03-10 16:40:32
 * @LastEditors: yblin
 * @LastEditTime: 2022-03-21 15:24:06
 * @Description: 
 */
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "event_bus.h"
#include "freertos/queue.h"

//*******************宏定义***************
#define MsgProcessTaskSTK_SIZE (2048)
#define MsgProcessTaskPRO (2)
#define MsgProcessTaskQUEUE_SIZE (100)

/*****************静态变量*****************/
static const char* TAG = "msg_process_task";
static QueueHandle_t msg_process_task_queue = NULL;

/*****************内部功能函数*****************/
static void msg_process_task_process(void* args)
{
    Event_t event;
    while(1) {
        if(xQueueReceive(msg_process_task_queue, &event, portMAX_DELAY)) {
            if(event.id == TASK_ITERATION_EVENT){
                int num = event.d.iteration.num;
                ESP_LOGI(TAG, "handling TASK_ITERATION_EVENT num:%d",num);
            }
        }
    }
}
//接收event_bus返回的消息处理。如有负责操作，则通过xQueueSend发送给task处理
static void event_bus_callback(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    if(base == APP_EVENTS){
        // Event_t event = *((Event_t*) event_data);
        ESP_LOGI(TAG, "callback %s id: %d ", base, id);
        xQueueSend(msg_process_task_queue, ((Event_t*) event_data), 0);
    }
    // else if(base == WIFI_EVENT){

    // } else if(base == IF_EVENT){

    // }
}

/*************************************外部函数**********************************/
void msg_process_task_init(){
    ESP_LOGI(TAG, "msg_process_task_init");
    msg_process_task_queue = xQueueCreate(MsgProcessTaskQUEUE_SIZE, sizeof(Event_t));
    if (msg_process_task_queue == NULL) {
        ESP_LOGE(TAG, "failed to alloc msg_process_task_queue");
        return;
    }

    //在这里订阅需要的自定义（APP_EVENTS）消息和系统消息（如：WIFI_EVENTS IP_EVENTS）
    ESP_ERROR_CHECK(evnet_bus_register(APP_EVENTS,TASK_ITERATION_EVENT,event_bus_callback));

    // Create the application task with the same priority as the current task
    xTaskCreate(msg_process_task_process, "bsp task_", MsgProcessTaskSTK_SIZE, NULL, MsgProcessTaskPRO, NULL);
}