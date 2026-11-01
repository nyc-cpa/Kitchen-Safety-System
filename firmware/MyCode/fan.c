#include "fan.h"
#include "mqtt.h"


void FAN_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* PB10(PB1 均配置为推挽输出, 初始低电平(风扇默认停止) */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
}


void FAN_ON(void)
{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET);
}
void FAN_OFF(void)
{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_RESET);

}
