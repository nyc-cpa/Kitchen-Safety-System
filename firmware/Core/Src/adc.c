/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */
uint8_t fire_flag=0;
Filter_t fire_filter;
Filter_t mq2_filter;
uint16_t g_fire_filtered = 0;   // 滤波后的火焰值（4095-原始值）
uint16_t g_mq2_filtered  = 0;   // 滤波后的MQ-2值

uint8_t mq2_alarm_flag = 0;
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;  /* 改这里：TIM1 CC1 事件 */
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 2;
    /* 删除了 DMAContinuousRequests，F1 不需要 */
    /* 删除了 EOCSelection，F1 不需要 */
    HAL_ADC_Init(&hadc1);

    /* F1 必须校准 */
    HAL_ADCEx_Calibration_Start(&hadc1);

    /* 通道0：PA0，序列第1位 */
    sConfig.Channel = ADC_CHANNEL_7;
    sConfig.Rank = ADC_REGULAR_RANK_2;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /* 通道1：PA1，序列第2位 */
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}



void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(adcHandle->Instance == ADC1)
    {
        /* 使能 ADC1 和 GPIOA 时钟 */
        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* PA0、PA1 配置为模拟输入模式 */
        GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* DMA 时钟使能 */
        __HAL_RCC_DMA1_CLK_ENABLE();

        /* DMA 中断配置（如果需要传输完成中断的话） */
        HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA7     ------> ADC1_IN7
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

