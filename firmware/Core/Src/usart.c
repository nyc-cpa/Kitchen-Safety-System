/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "tim.h"
#include "mqtt.h"
#include "staus.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "queue.h"   /* 原生 FreeRTOS API（原 cmsis_os.h 会强制引入 cmsis_os2.h 的配置断言） */

#include "staus.h"
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
    return ch;
}

uint8_t usart_buf;
uint8_t Rx_wifi_buf[wifi_buf_size];//接收数据缓冲

/* ===== USART2 DMA 发送新增项 ===== */
#define USART2_TX_MAX_LEN  300u                        /* 单包最大长度(对齐 TBUFF_UNIT) */
static uint8_t          Usart2_TxBuf[USART2_TX_MAX_LEN]; /* DMA 发送缓冲(拷贝源数据,防止被覆盖) */
static volatile uint8_t Usart2_Tx_Busy = 0u;           /* 发送忙标志, DMA 完成中断清零 */
/* ================================= */
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Channel6;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */
	/* ===== USART2_TX DMA 初始化 (DMA1_Channel7, 与接收 DMA1_Channel6 分离) ===== */
	hdma_usart2_tx.Instance = DMA1_Channel7;
	hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart2_tx.Init.Mode = DMA_NORMAL;
	hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_LINKDMA(uartHandle, hdmatx, hdma_usart2_tx);

	/* DMA1_Channel7 全局中断: 发送完成时清忙标志 (HAL_UART_TxCpltCallback) */
	HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

	/* 注: 初始化阶段这里不要启动 IDLE+DMA 接收。
	       它会在上电后立即与 MX_USART2_UART_Init 启动的单字节中断接收
	       争抢同一个 Rx_wifi_buf, 导致 WIFI 握手(AT响应)与 MQTT 报文解析错乱。
	       WIFI 握手阶段仅走 HAL_UART_Receive_IT 单字节接收;
	       MQTT 连接成功后由 main.c 连接分支切换为 HAL_UARTEx_ReceiveToIdle_DMA。 */
  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
		

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */
	/* USART2_TX DMA 反初始化 */
	HAL_DMA_DeInit(uartHandle->hdmatx);
	HAL_NVIC_DisableIRQ(DMA1_Channel7_IRQn);
  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/* USART2 DMA 发送完成回调: 由 DMA1_Channel7 传输完成中断触发 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	{
		Usart2_Tx_Busy = 0u;
	}
}

/* USART2 发送错误处理: DMA 传输错误等异常时清忙标志, 防止发送逻辑卡死 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	{
		Usart2_Tx_Busy = 0u;

		__HAL_UART_CLEAR_OREFLAG(huart);
		__HAL_UART_CLEAR_FEFLAG(huart);
		__HAL_UART_CLEAR_NEFLAG(huart);
		__HAL_UART_CLEAR_PEFLAG(huart);

		// 先中止残留接收使 RxState 回 READY，再重挂一次
		HAL_UART_AbortReceive(&huart2);
		memset(Rx_wifi_buf, 0, sizeof(Rx_wifi_buf));
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*)Rx_wifi_buf, 256);
		

	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
 if(huart->Instance==USART2)
 {


	 
 }
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
 if(huart->Instance==USART2)
 { 
	  if (Size >= 256) Size = 255;
		if (Size == 0) return;

		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	 
		if (Size >= 256) Size = 255;
	 	if(g_sys_status.Connect_flag==0)
		 
		{	
				memcpy(AT_Rx_Buf, Rx_wifi_buf, Size);
				AT_Rx_Buf[Size]='\0';
				AT_Rx_Len =Size;
		}
	 else
	  {
						if (Size > (RBUFF_UNIT - 2)) Size = RBUFF_UNIT - 2;
			
            memcpy(&MQTT_RxDataInPtr[2], Rx_wifi_buf, Size);
            MQTT_RxDataInPtr[0] = Size / 256;
            MQTT_RxDataInPtr[1] = Size % 256;
            
            MQTT_RxDataInPtr += RBUFF_UNIT;
            if (MQTT_RxDataInPtr >= MQTT_RxDataEndPtr) {
                MQTT_RxDataInPtr = MQTT_RxDataBuf[0];
            }
            
        }
		if (Task_Network_Handler != NULL) {
			vTaskNotifyGiveFromISR(Task_Network_Handler, &xHigherPriorityTaskWoken);
		}
	
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, Rx_wifi_buf, 256);
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
 }


void WIFI_printf(const char* u2_tx)
{

        HAL_UART_Transmit(&huart2, (uint8_t*)u2_tx, strlen(u2_tx), 100);
    
}


void u2_TxData(unsigned char *data)
{
    uint16_t send_len;
    uint32_t start_tick;

    /* 报文长度 = data[0]<<8 | data[1] (MQTT 环形缓冲前两字节约定) */
    send_len = (data[0] << 8) + data[1];

    /* 长度合法性检查, 防止越界写发送缓冲 */
    if (send_len == 0u || send_len > USART2_TX_MAX_LEN)
    {
        return;
    }

    /* 等待上一包 DMA 发送完成; 正常流程上包早已发完, 几乎不耗时 */
    start_tick = HAL_GetTick();
    while (Usart2_Tx_Busy)
    {
        if ((HAL_GetTick() - start_tick) > 100u)
        {
            break;   /* 100ms 兜底超时, 防止异常情况下无限阻塞主循环 */
        }
    }
    if (Usart2_Tx_Busy)
    {
        return;      /* 上包仍未发完(异常), 丢弃本包避免协议乱序 */
    }

    /* 先拷贝到专用发送缓冲: DMA 发送期间源环形缓冲可被主循环安全覆盖 */
    memcpy(Usart2_TxBuf, &data[2], send_len);

    /* 整包 DMA 发送, 非阻塞立即返回; 完成后 DMA1_Channel7 中断触发
       回调 HAL_UART_TxCpltCallback 清忙标志 */
    Usart2_Tx_Busy = 1u;
    if (HAL_UART_Transmit_DMA(&huart2, Usart2_TxBuf, send_len) != HAL_OK)
    {
        Usart2_Tx_Busy = 0u;   /* 启动失败(如 DMA 通道被占用), 清标志避免卡死 */
    }
}
/* USER CODE END 1 */

