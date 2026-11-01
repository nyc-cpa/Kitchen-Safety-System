/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "oled.h"
//#include "fire.h"
//#include "key.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 2 */

void SDA_OUT(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}



void MY_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIOB 时钟必须显式使能，否则 PB6/PB7 配置无效，OLED 无法通信 */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* GPIO Ports Clock Enable */
  /*Configure GPIO pin Output Level */
  /*Configure GPIO pins : PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	  IIC_SCL(1);
    IIC_SDA(1);

}
void DHT11_GPIO_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  // ??????????????
    GPIO_InitStruct.Pull = GPIO_PULLUP;      // ??????????????????????
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void MX_GPIO6_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 1. 开启 GPIOC 的时钟（极其重要！不开启这行后面全白搭） */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* 2. 配置 PC13 的初始状态为高电平（默认熄灭） */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

    /* 3. 配置 PC13 的引脚参数 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; // 开漏输出（Blue Pill 板载 LED 常用）
    GPIO_InitStruct.Pull = GPIO_NOPULL;         // 不需要上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 低速即可，我们只用来点灯

    /* 4. 初始化引脚 */
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
/* USER CODE END 2 */
