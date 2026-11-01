/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "usart.h"
#include "mqtt.h"
/* USER CODE END 0 */
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
//TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* TIM2 init function */
void MX_TIM1_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    /* 基本定时配置：72MHz / (7199+1) = 10kHz，ARR=99 → 100Hz / 10ms */
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 7199;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 99;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim1);

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

    /* 关键：配置通道1输出比较，CCR1=99（等于ARR）
       计数器每次数到99时产生CC1事件，作为ADC触发源
       不需要启动OC输出，内部事件即可，PA8不会有波形 */
    sConfigOC.OCMode = TIM_OCMODE_TOGGLE;     /* 翻转模式，只关心内部事件 */
    sConfigOC.Pulse = 99;                     /* CCR1 = ARR，计数到顶时匹配 */
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);

    /* 不再需要 MasterConfigSynchronization（TRGO 用不上了） */
}

void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
	HAL_TIM_Base_Start(&htim2);
  /* USER CODE END TIM2_Init 2 */

}
/* TIM3 init function */
//void MX_TIM3_Init(void)
//{

//  /* USER CODE BEGIN TIM3_Init 0 */

//  /* USER CODE END TIM3_Init 0 */

//  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//  TIM_MasterConfigTypeDef sMasterConfig = {0};

//  /* USER CODE BEGIN TIM3_Init 1 */

//  /* USER CODE END TIM3_Init 1 */
//  htim3.Instance = TIM3;
//  htim3.Init.Prescaler = 65535;
//  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
//  htim3.Init.Period = 32958;
//  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

//}
/* TIM4 init function */
void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */
	TIM_OC_InitTypeDef sConfigOC = {0};
  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 72-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 20000-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
	
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	HAL_TIM_MspPostInit(&htim4);
  /* USER CODE END TIM4_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

	if(tim_baseHandle->Instance == TIM1)
    {
        __HAL_RCC_TIM1_CLK_ENABLE();   /* TIM1 时钟在 APB2 上 */
    }
  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
//  else if(tim_baseHandle->Instance==TIM3)
//  {
//  /* USER CODE BEGIN TIM3_MspInit 0 */

//  /* USER CODE END TIM3_MspInit 0 */
//    /* TIM3 clock enable */
//    __HAL_RCC_TIM3_CLK_ENABLE();

//    /* TIM3 interrupt Init */
//    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
//  }
  else if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspInit 0 */

  /* USER CODE END TIM4_MspInit 0 */
    /* TIM4 clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();

        /* TIM4 仅用于 PWM 输出（TIM4_CH4 → PB9 驱动 SG90 舵机），
           未使用更新中断，故不使能 TIM4_IRQn。
           原此处为 HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0) + HAL_NVIC_EnableIRQ(TIM4_IRQn)：
           优先级 0 高于 FreeRTOS 允许调用 API 的上限（configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY=5），
           且工程中未实现 TIM4_IRQHandler，一旦使能中断将跳入启动文件的弱定义死循环。
           若将来确需 TIM4 定时中断，请将优先级设为 5，并实现 TIM4_IRQHandler 调用 HAL_TIM_IRQHandler(&htim4)。 */
  /* USER CODE BEGIN TIM4_MspInit 1 */
	
  /* USER CODE END TIM4_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspDeInit 0 */

  /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();

    /* TIM4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
  /* USER CODE BEGIN TIM4_MspDeInit 1 */

  /* USER CODE END TIM4_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspPostInit 0 */

  /* USER CODE END TIM4_MspPostInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM4 GPIO Configuration
    PB9     ------> TIM4_CH4
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM4_MspPostInit 1 */

  /* USER CODE END TIM4_MspPostInit 1 */
  }
}
//void TIM3_SwitchTo2S(void)
//{
//    // 1. ��ֹ��ʱ�������жϣ���ֹ�ĵ�һ�봥���жϣ�
//    HAL_TIM_Base_Stop_IT(&htim3);
//    
//    // 2. �޸�Ԥ��Ƶ���Զ�����ֵ��ʹ��HAL�꣩
//    __HAL_TIM_SET_PRESCALER(&htim3, PSC_2S);
//    __HAL_TIM_SET_AUTORELOAD(&htim3, ARR_2S);
//    
//    
//    // 4. ���¿��������жϲ�������ʱ��
//    HAL_TIM_Base_Start_IT(&htim3);
//}

//void TIM3_SwitchTo30S(void)
//{
//    // 1. ��ֹ��ʱ�������жϣ���ֹ�ĵ�һ�봥���жϣ�
//    HAL_TIM_Base_Stop_IT(&htim3);
//    
//    // 2. �޸�Ԥ��Ƶ���Զ�����ֵ��ʹ��HAL�꣩
//    __HAL_TIM_SET_PRESCALER(&htim3, PSC_30S);
//    __HAL_TIM_SET_AUTORELOAD(&htim3, ARR_30S);
//    
//    
//    // 4. ���¿��������жϲ�������ʱ��
//    HAL_TIM_Base_Start_IT(&htim3);
//}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if(htim->Instance == TIM3)
    {
				    HAL_IncTick();
    }
//  if(htim->Instance==TIM4)
//	{
//		memcpy(&MQTT_RxDataInPtr[2],Rx_wifi_buf,Rounter);
//		MQTT_RxDataInPtr[0]=Rounter/256;
//		MQTT_RxDataInPtr[1]=Rounter%256;
//		MQTT_RxDataInPtr+=RBUFF_UNIT;
//		if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)
//			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];
//		Rounter=0;
////		__HAL_TIM_SET_COUNTER(&htim3,0);
//		HAL_TIM_Base_Stop_IT(&htim4);
//		__HAL_TIM_SET_COUNTER(&htim4,0);
//	}
}

void sero_value(int r_value)
{
    PWM_count(r_value);
}

void PWM_count(int value)
{
	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,value*2000/180+500);
}
/* USER CODE END 1 */

