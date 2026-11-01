# STM32 智能厨房安防系统

基于 **STM32F103C8T6 + FreeRTOS** 的厨房/宿舍安防原型：本地采集温湿度、可燃气体/CO、火焰状态，异常时声光报警并联动排风扇、舵机模拟关阀；通过 **ESP8266 + MQTT + OneNET** 上传数据，配套 **微信小程序** 远程查看与控制。

> 本项目为本科毕业设计原型，测试数据为特定条件下的短时统计，不作为量产安全设备使用。

## 项目状态

- 已完成：硬件搭建、FreeRTOS 多任务、本地联动、OneNET 上传、小程序查看/控制
- 测试：燃气约 160ms、火焰约 100ms、远程约 4s、约 2h 短时统计 12 次报警 0 次误报
- 未完成：标准气体标定、真实火灾验证、24h 连续统计、商用产品对比

## 功能

- DHT11 温湿度采集
- MQ-9 可燃气体/CO 检测（ADC 阈值法）
- F5 火焰检测（模拟量反相 + 阈值）
- 火焰 / 燃气 / 复合三种报警联动
- 蜂鸣器、继电器排风扇、SG90 舵机（0° 开阀 / 180° 关阀）
- OLED 本地显示、按键手动控制
- ESP8266 STA + MQTT + OneNET 上传
- 微信小程序远程查看与控制
- FreeRTOS 6 任务 / 4 队列，ADC 中断任务通知

## 硬件

| 模块 | 型号 | 接口 |
|---|---|---|
| 主控 | STM32F103C8T6 核心板 | — |
| 温湿度 | DHT11 | PA5 单总线 |
| 可燃气体/CO | MQ-9 | PA7 / ADC1_IN7 |
| 火焰 | F5 | PA1 / ADC1_IN1 |
| WiFi | ESP8266（ESP-01S） | USART2：PA2 TX / PA3 RX，PA4 RST |
| 显示 | 0.96" SSD1306 OLED | PB6 SCL / PB7 SDA（软件 I2C） |
| 蜂鸣器 | 低电平触发有源蜂鸣器 | PA0 |
| 风扇 | 5V 排风扇 + 单路继电器 | PA6 |
| 舵机 | SG90 | PB9 / TIM4_CH4 PWM |
| 按键 | 3 只轻触按键 | PB0 消音 / PB8 关风扇 / PB10 开风扇 |

## 软件架构

FreeRTOS 6 任务、4 队列：

| 任务 | 优先级 | 栈（字） | 职责 |
|---|---|---|---|
| Task_Network | 2 | 320（静态） | ESP8266 联网、MQTT、心跳、重连 |
| Task_Sensor | 2 | 256 | DHT11、ADC 数据处理、分发 |
| Task_Logic | 3 | 128 | 阈值判断、报警去抖、联动决策 |
| Task_Control | 2 | 256 | 蜂鸣器、风扇、舵机 |
| Task_Display | 1 | 128 | OLED 刷新 |
| Task_Key | 1 | 128 | 消音/风扇按键 |

队列：`xQueue_Logic`、`xQueue_Display`、`xQueue_Network`、`xCmdQueue`，长度均为 5。

数据节拍：

- ADC：TIM1_CC1 触发，约 100Hz；每 10 次转换更新一次 5 点滑动平均
- Task_Sensor：200ms 向 `xQueue_Logic` 投递
- Task_Network：1s 上报一次 MQTT
- 小程序：5s 轮询

## 目录结构

```text
firmware/            STM32 工程（Core、MyCode、MDK-ARM、.ioc）
hardware/            原理图、PCB、BOM、实物照片
app/                 微信小程序源码
docs/                README 图片、测试数据、接线图
README.md
LICENSE
.gitignore
```

## 编译与烧录

1. 用 STM32CubeMX 打开 `firmware/*.ioc`
2. Keil MDK 打开 `firmware/MDK-ARM/*.uvprojx`
3. 复制 `firmware/Core/Inc/config.example.h` 为 `config.h`，填入自己的 WiFi 与 OneNET 参数
4. 编译、烧录
5. 串口 USART1（PA9/PA10，115200）可看调试信息

## 配置与脱敏

`config.h` 已被 `.gitignore` 忽略，仓库只保留 `config.example.h`。请勿提交：

- WiFi SSID / 密码
- OneNET 产品 ID、设备名、鉴权 token
- 手机号、邮箱等个人信息
- Keil 的 `Objects/`、`Listings/`、`*.hex`、`*.bin`

## 测试结果（特定测试条件）

| 项目 | 结果 | 说明 |
|---|---|---|
| 燃气报警响应 | 约 160ms | ADC 超限至执行器动作 |
| 火焰报警响应 | 约 100ms | 特定测试条件 |
| 远程端到端延迟 | 约 4s | 1s 上报 + 5s 小程序轮询 |
| 短时稳定性 | 约 2h，12 次报警 0 次误报 | 样本有限，24h 未完整统计 |
| 火焰距离 | 10/20cm 触发一致，25cm 以上打火机火源未触发 | 受火源尺寸限制，真实火灾需进一步验证 |

## 已知限制

- MQ-9 未做标准气体标定与 ppm 换算，阈值为 ADC 原始值
- 未做真实火灾/更大火源验证
- 未完成 24h 连续运行与统计置信分析
- 未与商用报警器做对比
- 未涉及 TLS/加密、FMEA、EMC 与精确 BOM 成本核算

## 项目截图

上传后放到 `docs/images/`，在 GitHub 编辑本段插入：

- `hardware.jpg`：实物连接图
- `oled.jpg`：OLED 显示
- `app.jpg`：小程序界面
- `demo.gif`：报警联动演示

## License

本项目代码仅用于学习与求职展示；HAL 库、FreeRTOS、OneNET SDK 等第三方组件遵循其原始许可证。`LICENSE` 可放 MIT（仅覆盖本人编写的代码）。
