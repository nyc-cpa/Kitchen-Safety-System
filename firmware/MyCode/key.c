#include "stm32f1xx_hal.h"
#include "string.h"
#include "stdio.h"
#include "mqtt.h"
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "staus.h"
#include "key.h"


void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};	
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* ---- PB0：消音按键 ---- */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	/* ---- PB8：关风扇按键 ----
	 * 原先只配置了 PB0，PB8/PB10 从未配置为 EXTI、NVIC 也从未使能，
	 * 因此 HAL_GPIO_EXTI_Callback 里那两个分支永远不会执行，
	 * “开/关风扇”物理按键在实物上完全无效。此处补齐。
	 * 注意：EXTI9_5 覆盖 EXTI5~EXTI9，PB9 已用作 TIM4_CH4 舵机输出
	 *       （复用推挽，不是中断模式），不会与本按键冲突。 */
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	/* ---- PB10：开风扇按键 ----
	 * EXTI15_10 覆盖 EXTI10~EXTI15；PA10 虽被 USART1_RX 占用，但它不是中断模式，
	 * EXTI 的 IMR 对应位为 0，因此不会误触发本中断。 */
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* 说明：原实现每次进中断都调用 HAL_NVIC_DisableIRQ() 关闭自己，
	 * 但工程中没有任何地方重新使能，导致按键“只能生效一次”。
	 * Task_Key 内部已有 20ms 软件去抖，无需再关中断，故此处不再关闭。
	 * 仅当任务尚未创建（极端情况）时才关闭，避免通知丢失后中断持续触发。 */

	if(GPIO_Pin == GPIO_PIN_0)          /* 消音按键 */
	{
		if (Task_Key_Handler != NULL)
		{
			vTaskNotifyGiveFromISR(Task_Key_Handler, &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
		else
		{
			HAL_NVIC_DisableIRQ(EXTI0_IRQn);
		}
	}
	else if(GPIO_Pin == GPIO_PIN_8)     /* 关风扇按键 */
	{
		if (Task_Key_Handler != NULL)
		{
			vTaskNotifyGiveFromISR(Task_Key_Handler, &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
		else
		{
			HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
		}
	}
	else if(GPIO_Pin == GPIO_PIN_10)    /* 开风扇按键 */
	{
		if (Task_Key_Handler != NULL)
		{
			vTaskNotifyGiveFromISR(Task_Key_Handler, &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
		else
		{
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		}
	}
}
