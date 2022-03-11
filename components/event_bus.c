/*
 * @Author: yblin
 * @Date: 2022-03-10 16:40:32
 * @LastEditors: yblin
 * @LastEditTime: 2022-03-11 10:53:30
 * @Description: 
 */
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "event_bus.h"

static const char* TAG = "event_bus";

// Event loops
esp_event_loop_handle_t event_bus_handle;

ESP_EVENT_DEFINE_BASE(APP_EVENTS);

void event_bus_init(){
    ESP_LOGI(TAG, "setting up");

    esp_event_loop_args_t event_bus_task_args = {
        .queue_size = 5,
        .task_name = "event_bus_task", // task will be created
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 2048,
        .task_core_id = tskNO_AFFINITY
    };

    // Create the event loops
    ESP_ERROR_CHECK(esp_event_loop_create(&event_bus_task_args, &event_bus_handle));

}
esp_err_t evnet_bus_register(esp_event_base_t event_base,
                                              int32_t event_id,
                                              esp_event_handler_t event_handler){
    // Register the handler for task iteration event. Notice that the same handler is used for handling event on different loops.
    // The loop handle is provided as an argument in order for this example to display the loop the handler is being run on.
    ESP_LOGI(TAG, "evnet_bus_register");
    return esp_event_handler_instance_register_with(event_bus_handle, event_base, event_id, event_handler,event_bus_handle, NULL);
}

esp_err_t evnet_bus_send(esp_event_base_t event_base, int32_t event_id,
        const void* event_data, size_t event_data_size, TickType_t ticks_to_wait){
    ESP_LOGI(TAG, "sevnet_bus_send");
    return esp_event_post_to(event_bus_handle, event_base, event_id, event_data, event_data_size, ticks_to_wait);
}

esp_err_t evnet_bus_isr_send(esp_event_base_t event_base, int32_t event_id,
        const void* event_data, size_t event_data_size, BaseType_t *task_unblocked){
    ESP_LOGI(TAG, "sevnet_bus_send");
    return esp_event_isr_post_to(event_bus_handle, event_base, event_id, event_data, event_data_size, task_unblocked);
}