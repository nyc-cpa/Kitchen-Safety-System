#include "buzzer.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include "stdio.h"
#include "mqtt.h"
#include "main.h"


void Buzzer_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};	
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0,GPIO_PIN_SET);
		
}



void BUZZER_ON(void)
{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0,GPIO_PIN_RESET);

}

void BUZZER_OFF(void)
{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0,GPIO_PIN_SET);
}
