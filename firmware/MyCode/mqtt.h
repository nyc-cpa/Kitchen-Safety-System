#ifndef __MQTT_H__
#define __MQTT_H__

#include "main.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#define  R_NUM					 4							//接受缓冲区个数
#define  RBUFF_UNIT			 128						//接受缓冲区长度

#define  T_NUM					 4							//发送缓冲区个数
#define  TBUFF_UNIT			 300						//发送缓冲区长度

#define  C_NUM					 3							//命令缓冲区个数
#define  CBUFF_UNIT			 128						//命令缓冲区长度

extern  unsigned char MQTT_RxDataBuf[R_NUM][RBUFF_UNIT];						//数据接收缓冲区
extern	unsigned char *MQTT_RxDataInPtr;														//指向接收缓冲区存放数据的位置
extern	unsigned char *MQTT_RxDataOutPtr;														//指向接收缓冲区读取数据的位置
extern  unsigned char *MQTT_RxDataEndPtr;														//指向接收缓冲区结束的位置

extern	unsigned char MQTT_TxDataBuf[T_NUM][TBUFF_UNIT];						//数据发送缓冲区
extern	unsigned char *MQTT_TxDataInPtr;														//指向发送缓冲区存放数据的位置
extern	unsigned char *MQTT_TxDataOutPtr;														//指向发送缓冲区读取数据的位置
extern	unsigned char *MQTT_TxDataEndPtr;	

extern	unsigned char MQTT_CMDBuf[C_NUM][CBUFF_UNIT];								//命令数据缓冲区
extern	unsigned char *MQTT_CMDInPtr;																//指向命令缓冲区存放数据的位置
extern	unsigned char *MQTT_CMDOutPtr;															//指向命令缓冲区读取数据的位置
extern	unsigned char *MQTT_CMDEndPtr;	

extern  char SeverIP[128];                                          
extern  int  SeverPort;

typedef struct {
    volatile uint8_t ping_flag;
    volatile uint8_t connect_pack_flag;
    volatile uint8_t subscribe_pack_flag;
} MQTT_State_t;
extern volatile MQTT_State_t g_mqtt_state;
//extern  char ConnectPack_flag;
//extern  char SubcribePack_flag;
//extern  char Ping_flag;

#define  DEVICENAME      			"YOUR_DEVICENAME"															//产品名称
#define  PRODUCTKEY      			"YOUR_PRODUCTKEY"															//产品ID
#define  DEVICESECRE     			"YOUR_DEVICESECRE" 
#define  DEVICESECRE_LEN      strlen(DEVICESECRE)javascript:;

// 订阅主题（服务器下发指令给设备）
#define S_TOPIC_NAME  				"YOUR_S_TOPIC_NAME"
// 发布主题（设备上报数据给服务器）
#define P_TOPIC_NAME  				"P_TOPIC_NAME"

void MQTT_Buff_Init(void);
void Onenet_Parameter_Init(void);
void MQTT_ConnectPack(void);
void CMDBuf_Deal(unsigned char *data, int size);
void TxDataBuf_Deal(unsigned char *data, int size);
void MQTT_PingREQ(void);
void MQTT_Subscribe(char *topic_name,int Qos);
void MQTT_Publish(char *topic,char *data,int data_len);
void MQTT_DealPushdata_Qos(unsigned char *redata);

#endif

