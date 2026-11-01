/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define PSC_30S   8399   // 84MHz / 8400 = 10KHz
#define ARR_30S   299999 // 10KHz / 300000 = 0.333Hz -> 3�루ʾ�����������30sֵ��
#define PSC_2S    8399   // ����Ԥ��Ƶһ�£������¼���
#define ARR_2S    19999  // 10KHz / 20000 = 0.5Hz -> 2��
/* USER CODE END Includes */
extern TIM_HandleTypeDef htim1;
	
extern TIM_HandleTypeDef htim2;

//extern TIM_HandleTypeDef htim3;

extern TIM_HandleTypeDef htim4;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */
void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);

/* USER CODE BEGIN Prototypes */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void TIM3_SwitchTo30S(void);
void TIM3_SwitchTo2S(void);
void sero_value(int r_value);
void PWM_count(int value);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

