#ifndef __DHT11_H
#define __DHT11_H
#include "main.h"  

void DHT11_in(void);
void DHT11_out(void);
uint8_t DHT11_Init(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);

#define DHT11_WRITE(x)   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define READ_DHT11       HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)



#endif
