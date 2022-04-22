/*
 * @Author: yblin
 * @Date: 2022-03-10 16:40:32
 * @LastEditors: yblin
 * @LastEditTime: 2022-04-22 11:41:46
 * @Description: 
 */
#ifndef COMPONENTS_EVENT_BUS_H_
#define COMPONENTS_EVENT_BUS_H_

#include "esp_event.h"
#include "esp_timer.h"
#include "esp_event_base.h"

#ifdef __cplusplus
extern "C" {
#endif

ESP_EVENT_DECLARE_BASE(APP_EVENTS);         // declaration of the task events family

typedef enum {
    TASK_ITERATION_EVENT,
    KEY_EVENT
}EventTypeId_t;

typedef enum {
  kKEY1 = 0,
  kKEY2 = 1,
} keyType_e;

typedef enum {
  kRelease = 0,
  kPress,
} keyEventType_e;

typedef struct {
  keyType_e key;
  keyEventType_e type;
  uint8_t seconds;
} KeyEvent_t;

typedef struct
{
  char data[64];
  uint16_t size;
} OrderEvent_t;
typedef struct
{
  int8_t num;
} IterationEvent_t;
typedef union {
  IterationEvent_t iteration; //EventTypeId_t:TASK_ITERATION_EVENT
  KeyEvent_t key;             //EventTypeId_t:KEY_EVENT
  OrderEvent_t order;
} EventUnion_t;

typedef struct {
  EventTypeId_t id;
  EventUnion_t d;
} Event_t;

void event_bus_init();

esp_err_t evnet_bus_register(int32_t event_id, esp_event_handler_t event_handler);

esp_err_t evnet_bus_send(int32_t event_id,
        const void* event_data, size_t event_data_size, TickType_t ticks_to_wait);

esp_err_t evnet_bus_isr_send(int32_t event_id,
        const void* event_data, size_t event_data_size, BaseType_t *task_unblocked);

#ifdef __cplusplus
}
#endif

#endif // #ifndef EVENT_BUS_H_
