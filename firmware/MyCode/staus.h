#ifndef __STAUS_H
#define __STAUS_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"

typedef struct{
	uint16_t fire_val;
	uint16_t mq_val;
	uint16_t temp;
	uint16_t humi;
}SensorData_t;

typedef struct {
    uint8_t fan_cmd;    // 0=关闭风扇，1=开启风扇
    uint8_t valve_cmd;  // 0=关闭燃气阀，1=开启燃气阀
    uint8_t buzzer_cmd; // 0=关闭蜂鸣器，1=鸣响
} ControlData_t;

typedef struct {
    uint8_t fan_state;    // 0=关，1=开
    uint8_t buzzer_state; // 0=关，1=鸣响
    uint8_t valve_state;  // 0=关，1=开
    volatile uint8_t Connect_flag;   // 0=未连接，1=WiFi已连，2=MQTT已连
} SystemStatus_t;


extern QueueHandle_t xQueue_Logic;
extern QueueHandle_t xQueue_Display;
extern QueueHandle_t xQueue_Network;
extern QueueHandle_t xCmdQueue;

extern TaskHandle_t Task_Network_Handler;
extern TaskHandle_t Task_Sensor_Handler;
extern TaskHandle_t Task_Key_Handler;
extern TaskHandle_t Task_Logic_Handler;
extern TaskHandle_t Task_Display_Handler;
extern TaskHandle_t Task_Control_Handler;
extern volatile SystemStatus_t g_sys_status;
extern SensorData_t sendData ;

#endif
