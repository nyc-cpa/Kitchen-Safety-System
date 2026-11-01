#include "DHT11.h"
#include "gpio.h"
#include "delay.h"
#include "stdio.h"
#include "stdint.h"
#include "main.h"

void DHT11_out(void)
{
    // 1. 清除 PA5 的 4 位配置 (位20-23) -> 0xF << 20
    GPIOA->CRL &= ~(0xF << 20);
    // 2. 设置为推挽输出，最高频率 50MHz (CRL里对应配置为 0011，即十六进制 0x3)
    GPIOA->CRL |= (0x3 << 20);    
}

void DHT11_in(void)
{
    // 1. 清除 PA5 的 4 位配置 (位20-23)
    GPIOA->CRL &= ~(0xF << 20);
    // 2. 设置为上拉/下拉输入模式 (CRL里对应配置为 1000，即十六进制 0x8)
    GPIOA->CRL |= (0x8 << 20); 
    // 3. 【关键】强制输出数据寄存器为高电平，确保内部上拉电阻生效
    GPIOA->ODR |= (1 << 5);    
}

uint8_t DHT11_Init(void)
{
	uint8_t cnt=0;
	DHT11_out();
	DHT11_WRITE(0);
	Delay_ms(20);
	DHT11_WRITE(1);
	Delay_us(30);
	DHT11_in();
	
	while(READ_DHT11 && cnt<100)
	{
		cnt++;
		Delay_us(1);
	}

	if(cnt>=100)
		return 1;
	else
		cnt=0;
	while(!READ_DHT11 && cnt<100)
	{
		cnt++;
		Delay_us(1);
	}

	if(cnt>=100)
		return 2;
	return 0;
}

uint8_t DHT11_Read_Bit(void)
{
	uint8_t cnt=0;
	while(READ_DHT11 && cnt<100)
	{
		cnt++;
		Delay_us(1);
	}
//printf("%d\r\n",cnt);
	if(cnt>100)
		return 2;
	else
		cnt=0;
	while(!READ_DHT11 && cnt<100)
	{
		cnt++;
		Delay_us(1);
	}

	if(cnt>100)
		return 3;
	else
		cnt=0;
	Delay_us(35);
	if(READ_DHT11)
		return 1;
	else
		return 0;
}

uint8_t DHT11_Read_Byte(void)
{
	uint8_t mask;
	uint8_t dat=0;
	for(mask = 0x80;mask > 0;mask=mask>>1)
	{
		if(DHT11_Read_Bit())
		{
			dat|=mask;
		}

	}
	return dat;
}

uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)
{
	uint8_t buf[5];
	uint8_t cnt=0;
	DHT11_out();
	DHT11_WRITE(0);
	Delay_ms(20);
	DHT11_WRITE(1);
	Delay_us(30);
	DHT11_in();
	
	while(READ_DHT11 && cnt<100)
	{
		cnt++;
		Delay_us(1);
	}

	if(cnt>=100)
		return 1;
	else
		cnt=0;
	while(!READ_DHT11 && cnt<100)
	{
		cnt++;
		Delay_us(1);
	}

	if(cnt>=100)
		return 2;

	for(int i = 0;i < 5;i++)
	{
		buf[i]=DHT11_Read_Byte();
		
	}
	if(buf[0]+buf[1]+buf[2]+buf[3] == buf[4])
	{
		*humi=buf[0];
		*temp=buf[2];
		return 0;
	}
	else
	{
		return 1;
	}
}
