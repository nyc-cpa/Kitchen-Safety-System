#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "main.h" 

#define BUZZER_STA    HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

void Buzzer_Init(void);
void BUZZER_ON(void);
void BUZZER_OFF(void);

#endif
