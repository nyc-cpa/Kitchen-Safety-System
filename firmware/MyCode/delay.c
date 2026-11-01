#include "delay.h"
#include "tim.h"
/* 定时器微秒延时初始化 */
void Delay_us_Init(void)
{
    // 启动 TIM2
    HAL_TIM_Base_Start(&htim2);
}

/* 精准微秒延时函数 (适用于短延时，比如小于 10ms) */
void Delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    uint32_t last_cnt = 0;
    while(1)
    {
        uint32_t cur_cnt = __HAL_TIM_GET_COUNTER(&htim2);
        // 如果当前计数值 比 上一次计数值小，说明定时器发生了溢出，强制退出
        if (cur_cnt < last_cnt) {
            break; 
        }
        if (cur_cnt >= us) {
            break; // 达到目标时间，正常退出
        }
        last_cnt = cur_cnt;
    }
}

/* 毫秒延时函数 */
void Delay_ms(uint32_t ms)
{
    // 如果是短延时，用你的循环方法
    if(ms < 10) {
        uint32_t i;
        for(i=0; i<ms; i++) {
            Delay_us(1000);
        }
    } 
    // 如果是长延时（比如你图中的 100ms 和 500ms），建议使用系统 HAL 自带延时
    else {
        HAL_Delay(ms); 
    }
}
