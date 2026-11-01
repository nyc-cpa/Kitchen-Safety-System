#include "stm32f1xx_hal.h"
#include "wifi.h"
#include "string.h"
#include "usart.h"
#include "stdio.h"
#include "main.h"
#include "mqtt.h"
#include "delay.h"
#include "staus.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"



char AT_Rx_Buf[256] = {0};  // 用来存放ESP8266回复的AT指令
uint16_t AT_Rx_Len = 0;     // 记录AT指令的长度

char AT_Tx_Buf[256] = {0};  // 用来存放ESP8266的AT指令
uint16_t AT_Tx_Len = 0;     // 记录AT指令的长度


/*WIFI复位引脚初始化*/
void WIFI_ResetIoInit()
{
	GPIO_InitTypeDef GPIO_InitStruct={0};	
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	WIFIRest_IO(1);
	
	GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}


/*WIFI复位*/
void WIFI_Reset(void)
{
	HAL_UART_AbortReceive(&huart2); 
	WIFIRest_IO(0);
	vTaskDelay(pdMS_TO_TICKS(100));
	WIFIRest_IO(1);
	vTaskDelay(pdMS_TO_TICKS(500));
	memset(AT_Rx_Buf, 0, sizeof(AT_Rx_Buf));
  AT_Rx_Len = 0;
	memset(Rx_wifi_buf, 0, sizeof(Rx_wifi_buf));
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)Rx_wifi_buf, 256);


}



/*建立服务器初始化*/
void WIFI_Server_Connect(void)
{
	static Net_Sta net_state=NET_STATE_INIT; 
	WIFI_ResetIoInit();
	 if (g_sys_status.Connect_flag == 0) {
        net_state = NET_STATE_INIT;
    }
	while(g_sys_status.Connect_flag==0)
	{	
			switch(net_state){
				case NET_STATE_INIT:
						WIFI_Reset();
						net_state=NET_STATE_SET_CWMODE;
						break;
				case NET_STATE_SET_CWMODE:		
						WIFI_Send("AT+CWMODE=1\r\n");
						if(Wait_Wifi_Response("OK", 1000)==1)
							net_state=NET_STATE_SET_MUX;
						
						break;
				case NET_STATE_SET_MUX:
						WIFI_Send("AT+CWAUTOCONN=0\r\n");
						if(Wait_Wifi_Response("OK", 1000)==1)
							net_state=NET_STATE_CONNECT_WIFI;
						break;
				case NET_STATE_CONNECT_WIFI:
						WIFI_Connect_Rounter(R_ID,passward);
						if(Wait_Wifi_Response("OK", 15000)==1)
							net_state=NET_STATE_SET_SINGLE_CONN;
						break;
				case NET_STATE_SET_SINGLE_CONN:
						WIFI_Send("AT+CIPMUX=0\r\n");
						if(Wait_Wifi_Response("OK", 1000)==1)
							net_state=NET_STATE_SET_CIPMODE;
						break;
				case NET_STATE_SET_CIPMODE:
						WIFI_Send("AT+CIPMODE=1\r\n");
						if(Wait_Wifi_Response("OK", 1000)==1)
							net_state=NET_STATE_CONNECT_TCP;
						break;
				case NET_STATE_CONNECT_TCP:
						AT_Rx_Len=0;
						memset(AT_Rx_Buf, 0, sizeof(AT_Rx_Buf));
						AT_Tx_Len = 0;
						memset(AT_Tx_Buf, 0, sizeof(AT_Tx_Buf));
						sprintf(AT_Tx_Buf,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",SeverIP,SeverPort);
						WIFI_printf(AT_Tx_Buf);
						if(Wait_Wifi_Response("CONNECT", 5000)==1)
							net_state=NET_STATE_ENTER_CIPSEND;
						break;
				case NET_STATE_ENTER_CIPSEND:
						WIFI_Send("AT+CIPSEND\r\n");
						if(Wait_Wifi_Response(">", 1000)==1)
						{
							net_state=NET_STATE_RUNNING;
							g_sys_status.Connect_flag=1;
							AT_Tx_Len = 0;
							memset(AT_Tx_Buf, 0, sizeof(AT_Tx_Buf));
							MQTT_Buff_Init();
						}
						break;
					}
				vTaskDelay(pdMS_TO_TICKS(10));
				
				}			
}



void WIFI_Send(char* cmd)
{
		AT_Rx_Len=0;
		memset(AT_Rx_Buf, 0, sizeof(AT_Rx_Buf));
		WIFI_printf(cmd);
}


void WIFI_Connect_Rounter(char* R_id,char* Passward)
{ 
	AT_Rx_Len=0;
	memset(AT_Rx_Buf, 0, sizeof(AT_Rx_Buf));
	AT_Tx_Len = 0;
	memset(AT_Tx_Buf, 0, sizeof(AT_Tx_Buf));  
	sprintf(AT_Tx_Buf,"AT+CWJAP=\"%s\",\"%s\"\r\n",R_id,Passward);
	WIFI_printf(AT_Tx_Buf);
}




uint8_t Wait_Wifi_Response(char *expected_str, uint32_t timeout_ms){

			uint32_t start_timer = xTaskGetTickCount();
	
			while(xTaskGetTickCount()-start_timer <pdMS_TO_TICKS(timeout_ms))
			{
				    
				 
					if(strstr(AT_Rx_Buf, expected_str)){
						return 1;
					}
					vTaskDelay(pdMS_TO_TICKS(50));
			}
			
			return 0;
}
