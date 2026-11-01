#include "stdint.h"
#include "oled.h"
#include "gpio.h"
#include "delay.h"
#include "OLED_Font.h"
#include "OLED_HZ16.h"

//起始信号
void IIC_START(void)
{
	SDA_OUT();
	IIC_SDA(1);
	IIC_SCL(1);
	Delay_us(4);
	IIC_SDA(0);
	Delay_us(4);
	IIC_SCL(0);
}


//停止信号
void IIC_STOP(void)
{
	SDA_OUT();
	IIC_SCL(0);
	IIC_SDA(0);
	Delay_us(4);
	IIC_SCL(1);
	IIC_SDA(1);
	Delay_us(4);
}


// 发一个字节，忽略应答，和标准库逻辑完全一致
void I2c_Write(uint8_t dat)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        IIC_SDA( !!(dat & (0x80 >> i)) );
        IIC_SCL(1);
        IIC_SCL(0);
    }
    // 第9个时钟：直接跳过，不处理应答
    IIC_SCL(1);
    IIC_SCL(0);
}
void Write_IIC_Command(uint8_t command)
{
    IIC_START();
    I2c_Write(0x78);      // 从机地址 = 0x3C<<1 = 0x78
    I2c_Write(0x00);      // 控制字节：后续为命令
    I2c_Write(command);
    IIC_STOP();
}

void Write_I2c_Data(uint8_t data)
{
    IIC_START();
    I2c_Write(0x78);      // 从机地址
    I2c_Write(0x40);      // 控制字节：后续为数据
    I2c_Write(data);
    IIC_STOP();
}
// 参数：Y页(0-7)，X列(0-127)，和标准库OLED_SetCursor一致
void OLED_Set_Pos(uint8_t y, uint8_t x)
{
    Write_IIC_Command(0xB0 | y);
    Write_IIC_Command(0x10 | ((x & 0xF0) >> 4));
    Write_IIC_Command(0x00 | (x & 0x0F));
}

void oled_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        OLED_Set_Pos(j, 0);
        for(i = 0; i < 128; i++)
        {
            Write_I2c_Data(0x00);
        }
    }
}
void oled_Init(void)
{
    // 1. 长上电延时，对齐标准库的两层for循环
    HAL_Delay(100);

    // 2. 初始化IIC引脚
    MY_IIC_Init();

    // 3. 初始化命令，顺序、参数和标准库完全一致
    Write_IIC_Command(0xAE);   // 关闭显示

    Write_IIC_Command(0xD5);   // 时钟分频/振荡频率
    Write_IIC_Command(0x80);

    Write_IIC_Command(0xA8);   // 多路复用率
    Write_IIC_Command(0x3F);

    Write_IIC_Command(0xD3);   // 显示偏移
    Write_IIC_Command(0x00);

    Write_IIC_Command(0x40);   // 显示开始行

    Write_IIC_Command(0xA1);   // 段重定义（左右方向）

    Write_IIC_Command(0xC8);   // COM扫描方向（上下方向）

    Write_IIC_Command(0xDA);   // COM引脚配置
    Write_IIC_Command(0x12);

    Write_IIC_Command(0x81);   // 对比度
    Write_IIC_Command(0xCF);

    Write_IIC_Command(0xD9);   // 预充电周期
    Write_IIC_Command(0xF1);

    Write_IIC_Command(0xDB);   // VCOMH电压
    Write_IIC_Command(0x30);

    Write_IIC_Command(0xA4);   // 正常显示显存

    Write_IIC_Command(0xA6);   // 正常显示（非反色）

    Write_IIC_Command(0x8D);   // 电荷泵设置
    Write_IIC_Command(0x14);   // 开启电荷泵

    Write_IIC_Command(0xAF);   // 开启显示

    // 4. 清屏
    oled_Clear();
}
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{
    unsigned char c=0, i=0;
    c=chr-' ';
    
    if(x>128-1)
    {
        x=0;
        y=y+2;
    }

    // 参数顺序修正：先传页y，再传列x
    OLED_Set_Pos(y, x); 
    for(i=0; i<8; i++) 
        Write_I2c_Data(OLED_F8x16[c][i]);
    
    OLED_Set_Pos(y+1, x);
    for(i=0; i<8; i++) 
        Write_I2c_Data(OLED_F8x16[c][i+8]);
}
/**
 * @brief  显示ASCII字符串（适配8x16字号）
 * @param  x: 起始列坐标，范围 0~127
 * @param  y: 起始页号，范围 0~7（16号字占2页，y最大为6）
 * @param  str: 字符串首地址
 * @param  size: 字号，固定传16即可
 */
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t size)
{
    while(*str != '\0')
    {
        // 超出屏幕宽度自动换行
        if(x > 128 - 8)
        {
            x = 0;
            y += 2;
        }
        // 防止页号越界，超出屏幕则停止
        if(y > 6) break;
        
        OLED_ShowChar(x, y, *str, size);
        x += 8;     // 每个8x16字符宽度为8像素
        str++;
    }
}

/**
 * @brief  Display a 16x16 Chinese char by index (see OLED_HZ16.h)
 * @param  x: start column 0~127
 * @param  y: start page 0~6 (16x16 char occupies 2 pages)
 * @param  index: char index, use HZ_XXX macro in OLED_HZ16.h
 * @note   Same column-major / MSB-first / positive-bit layout as OLED_F8x16
 */
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t index)
{
    unsigned char i = 0;

    if(x > 128 - 16)
    {
        x = 0;
        y = y + 2;
    }
    // prevent page overflow
    if(y > 6) return;

    // upper page: 16 columns, bytes [0..15]
    OLED_Set_Pos(y, x);
    for(i = 0; i < 16; i++)
        Write_I2c_Data(OLED_HZ16[index][i]);

    // lower page: 16 columns, bytes [16..31]
    OLED_Set_Pos(y + 1, x);
    for(i = 0; i < 16; i++)
        Write_I2c_Data(OLED_HZ16[index][i + 16]);
}

/* 显示汉字串：hz 为索引数组，len 为汉字个数，末尾可带 ASCII 后缀 */
void OLED_ShowChineseStr(uint8_t x, uint8_t y, const uint8_t *hz, uint8_t len)
{
    uint8_t i;
    for(i = 0; i < len; i++)
    {
        OLED_ShowChinese(x, y, hz[i]);
        x += 16;
    }
}

/* 6x8 小字号字符 -> OLED_F6x8 表下标 */
static uint8_t OLED_F6x8_Index(uint8_t ch)
{
    if(ch == ' ')          return 0;
    if(ch >= '0' && ch <= '9') return ch - '0' + 1;
    if(ch == ':')          return 11;
    if(ch == '-')          return 13;
    if(ch >= 'A' && ch <= 'Z') return ch - 'A' + 14;
    if(ch >= 'a' && ch <= 'z') return ch - 'a' + 14;   /* 小写按大写显示 */
    return 0;   /* 未知字符按空格处理 */
}

/**
 * @brief  OLED 6x8 小号ASCII字符显示（单页高度8，共6列宽），
 *         用于数字/符号以压缩排版，与16x16汉字混排
 * @param  x: 起始列 0~127
 * @param  y: 起始页 0~7（只占1页）
 * @param  ch: 字符（支持空格/0-9/:/C/-，其余按空格）
 */
void OLED_ShowChar6x8(uint8_t x, uint8_t y, uint8_t ch)
{
    uint8_t i, idx;

    if (x > 122 || y > 7) return;

    idx = OLED_F6x8_Index(ch);

    OLED_Set_Pos(y, x);              /* 先页y，再列x */

    for (i = 0; i < 6; i++)
    {
        Write_I2c_Data(OLED_F6x8[idx][i]);
    }
}




/**
 * @brief  OLED 6x8 小号 ASCII 字符串显示（每字符6列宽, 单页高8）
 * @param  x: 起始列 0~127
 * @param  y: 起始页 0~7
 * @param  str: 字符串（支持空格/0-9/:/C/-/A-Z，小写按大写显示）
 * @note   调用方须保证各项宽度合计<=121，避免 6x8 内部换行打乱布局
 */
void OLED_ShowString6x8(uint8_t x, uint8_t y, char *str)
{
    while(*str != '\0')
    {
        if(y > 7) return;
        OLED_ShowChar6x8(x, y, *str);
        x += 6;
        if(x > 121) break;      /* 防止 6x8 内部换行破坏行内 x 坐标 */
        str++;
    }
}


