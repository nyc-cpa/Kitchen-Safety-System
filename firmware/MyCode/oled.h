#ifndef __OLED_H__
#define __OLED_H__
#include "main.h"            

#define IIC_SCL(x)      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define IIC_SDA(x)      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define READ_SDA        HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);

void IIC_START(void);
void IIC_STOP(void);
void I2c_Write(uint8_t dat);
uint8_t I2c_Address(uint8_t address);
void Write_I2c_Data(unsigned char I2c_Data);
void Write_IIC_Command(unsigned char command);
void OLED_Set_Pos(unsigned char x,unsigned char y);
void oled_Init(void);
void oled_Clear(void);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t size);
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t index);
void OLED_ShowChineseStr(uint8_t x, uint8_t y, const uint8_t *hz, uint8_t len);
void OLED_ShowChar6x8(uint8_t x, uint8_t y, uint8_t ch);
void OLED_ShowString6x8(uint8_t x, uint8_t y, char *str);

#endif
