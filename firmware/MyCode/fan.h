#ifndef __FAN_H__
#define __FAN_H__

#include "main.h"

/* 风扇控制 (云端命令控制):
 *   PB10 输出高电平 -> 风扇"开"动作
 *   PB1  输出高电平 -> 风扇"关"动作
 * 两者互斥: 执行开时 PB10=1/PB1=0, 执行关时 PB10=0/PB1=1
 * 初始均为低电平 = 上电风扇默认停止
 * 若实际继电器为低电平触发, 请交换 FAN_ON / FAN_OFF 中的电平
 */
#define FAN_STA    HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)  /* 1=风扇开 */

void FAN_Init(void);
void FAN_ON(void);
void FAN_OFF(void);

#endif
