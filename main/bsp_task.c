/*
 * @Author: yblin
 * @Date: 2022-03-10 16:40:32
 * @LastEditors: yblin
 * @LastEditTime: 2022-03-11 13:21:02
 * @Description: 
 */
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "event_bus.h"
#include "freertos/queue.h"

//*******************宏定义***************
#define BspTaskSTK_SIZE (2048)
#define BspTaskPRO (tskIDLE_PRIORITY + 3)
#define BspTaskQUEUE_SIZE (50)

/*****************静态变量*****************/
static const char* TAG = "bsp_task";
static QueueHandle_t bsp_task_queue = NULL;

/*****************内部功能函数*****************/
//gpio中断函数
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    Event_t event;
    event.id = KEY_EVENT;
    event.d.key.key = kKEY1;
    event.d.key.type = kRelease;
    xQueueSendFromISR(bsp_task_queue, &event, NULL);
}

static void bsp_task_process(void* args)
{
    Event_t event;
    while(1) {
        if(xQueueReceive(bsp_task_queue, &event, portMAX_DELAY)) {
            if(event.id == TASK_ITERATION_EVENT){
                int num = event.d.iteration.num;
                ESP_LOGI(TAG, "handling TASK_ITERATION_EVENT num:%d",num);
            }else if(event.id == KEY_EVENT){
                //按键事件处理
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
        xQueueSend(bsp_task_queue, ((Event_t*) event_data), NULL);
    }

/*************************************外部函数**********************************/
void bsp_task_init(){
    ESP_LOGI(TAG, "bsp_task_init");
    bsp_task_queue = xQueueCreate(BspTaskQUEUE_SIZE, sizeof(Event_t));
    if (bsp_task_queue == NULL) {
        ESP_LOGE(TAG, "failed to alloc bsp_task_queue");
        return;
    }
    //在这里订阅需要的自定义（APP_EVENTS）消息和系统消息
    ESP_ERROR_CHECK(evnet_bus_register(APP_EVENTS,TASK_ITERATION_EVENT,event_bus_callback));
    // Create the application task with the same priority as the current task
    xTaskCreate(bsp_task_process, "bsp task_", BspTaskSTK_SIZE, NULL, BspTaskPRO, NULL);
}