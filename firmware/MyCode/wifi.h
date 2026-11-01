#ifndef __WIFI_H__
#define __WIFI_H__

#define WIFIRest_IO(x) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState)((x)?1:0))
#define R_ID   "YOUR_WIFI_ID"
#define passward "YOUR_WIFI_PASSWORD"

typedef enum{
	    NET_STATE_INIT,
			NET_STATE_SET_CWMODE,    	// 设STA模式
			NET_STATE_CONNECT_WIFI,  	// 连WiFi热点
			NET_STATE_SET_SINGLE_CONN,//单连接模式
			NET_STATE_SET_MUX,       	// 关多连接
			NET_STATE_CONNECT_TCP,   	// 连OneNET服务器
			NET_STATE_SET_CIPMODE,   	// 开透传
			NET_STATE_ENTER_CIPSEND, 	// 发CIPSEND进入透传
			NET_STATE_RUNNING,       	// 正常运行，收发MQTT报文
			NET_STATE_RECONNECT
}Net_Sta;


extern char AT_Rx_Buf[256];  // 用来存放ESP8266回复的AT指令
extern uint16_t AT_Rx_Len;     // 记录AT指令的长度
extern char AT_Rx_Buf[256];  // 用来存放ESP8266回复的AT指令
extern uint16_t AT_Rx_Len;     // 记录AT指令的长度

void WIFI_Server_Connect(void);
void WIFI_ResetIoInit(void);
void WIFI_Reset(void);
void WIFI_Send(char* cmd);
void WIFI_Connect_Rounter(char* R_id,char* Passward);
uint8_t Wait_Wifi_Response(char *expected_str, uint32_t timeout_ms);


#endif
