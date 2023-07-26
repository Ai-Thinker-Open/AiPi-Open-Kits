#ifndef USER_INC_USER_CONFIG_H_
#define USER_INC_USER_CONFIG_H_
/** @addtogroup uni_config_macro
@{*/

/** @defgroup uni_config_gpio_macro GPIO相关宏定义
GPIO settings
@{*/
#define GPIO_OUT_DEF_VAL      1       ///< 0: default is low, 1: default is high, when GPIO set to output mode
/** @}*/

/** @defgroup uni_config_audio_macro audio相关宏定义
Audio player volume settings, default value means (5, 25, 50, 75, 100)
@{*/
#define USER_AUDIO_VOL_LEVEL  5       ///< Level is divided into several 0 - 100
#define USER_AUDIO_VOL_MIN    5       ///< The minimum volume
#define USER_AUDIO_VOL_MAX    100     ///< The maximum volume
#define USER_AUDIO_VOL_MID    50      ///< The middle volume
#define USER_AUDIO_VOL_DEF    50      ///< Default volume if reset device
/** @}*/

/** @defgroup uni_config_communication_macro 通讯相关宏定义
Use Unione communication protocol settings, default USE B6/B7 pin, 115200, 8, 1, 0
@{*/
#define USE_UNIONE_PROTOCOL   0       ///< 0: don't use, 1: use
#define USE_SAMPLE_PROTOCOL   0       ///< 0: don't use, 1: use (cannot set to 0 if USE_UNIONE_PROTOCOL is 1)
#define OPEN_ASR_EVENT_UCP    1       ///< 0: not out, 1: output ASR result by UCP or SUCP uart port
#define UNI_UART_DEVICE_NUM   1       ///< Only 1 can be used on HB-M board
#define UNI_UART_PIN_SELECT   0       ///< 0: B7/B6, 1: B0/B1, 2: B2/B3
#define UNI_UART_BAUD_RATE    115200  ///< 4800/9600/19200/38400/57600/115200
#define UNI_UART_PARITY       0       ///< 0: None, 1: Odd, 2: Even
#define UNI_UART_STOP_BIT     1       ///< 1/2
#define UNI_UART_DATA_BIT     8
/** @}*/

/** @defgroup uni_config_uart_macro 串口相关宏定义
Custom UART settings, default USE B6/B7 pin, 19200, 8, 1, 0
Cannot use it if ::USE_UNIONE_PROTOCOL is 1, only one UART on HB-M board
@{*/
#define USER_UART_DEVICE_NUM  1       ///< Only 1 can be used on HB-M board
#define USER_UART_PIN_SELECT  0       ///< 0: B7/B6, 1: B0/B1, 2: B2/B3
#define USER_UART_BAUD_RATE   115200    ///< 4800/9600/19200/38400/57600/115200
#define USER_UART_PARITY      0       ///< 0: None, 1: Odd, 2: Even
#define USER_UART_STOP_BIT    1       ///< 1/2
#define USER_UART_DATA_BIT    8
/** @}*/

/** @defgroup uni_config_asr_macro 语音相关宏定义
Recognize configurations, default multiply dialogue and no AEC
@{*/
/** 多轮对话模式
- 1: multiply dialogue mode
- 0: single dialogue mode
*/
#define MULTI_DIALOGUE_MODE   1
/** 唤醒打断模式
- 1: wakeup interrupt reply
- 0: wakeup cannot interrupt reply
*/
#define WAKEUP_INTERRUPT_MODE 0
/** @}*/

/** @defgroup uni_config_demo_macro Demo相关宏定义
@brief 使用方法见详细描述
@par Demo使用方法：

1.要使能Demo功能，先将@ref USER_RUN_DEMO 设置为1 \n
2.再通过配置@ref USER_RUN_DEMO_SELECT 定义为需要展示的示例程序宏 \n
3.重新编译固件，烧录后即可体验demo运行效果
@{*/
#define USER_RUN_DEMO         1       ///< 1: select a demo to run, 0: no user demo,
                                      ///< add your code to user_main() to run
#define USER_DEMO_SMART_AC    1       ///< a demo to show smart air fan (have to replace grammar and mp3 to support commands)
#define USER_DEMO_HOUSEKEEPER 2       ///< a demo to show IR controller base on HB1688 what is driven by UART
#define USER_DEMO_KEY_WAKEUP  3       ///< a demo to show how to wakeup device by digital kay base on GPIO
#define USER_DEMO_ADC_CTRL    4       ///< a demo to show how to mute speaker if at night base on photosensitive sensor
#define USER_DEMO_TIMERS      5       ///< a demo to show how to set 15 software timers base on 1 hardware timer
#define USER_DEMO_BUZZER      6       ///< a demo to show how to drive a buzzer base on timer
#define USER_DEMO_PWM_LED     7       ///< a demo to show how to drive a LED base on PWM
#define USER_DEMO_I2C_TIME    8       ///< a demo to show how to get real time from DS3231 base on I2C
#define USER_DEMO_SPI_LCD     9       ///< a demo to show how to draw chars on LCD waht driven by SSD1306 base on SPI
#define USER_DEMO_UART        10      ///< a demo to show how to use uart send and receive data
#define USER_DEMO_UART_UCP    11      ///< a demo to show how to use uart send and receive data with uart communication protocol
#define USER_DEMO_PLAYER      12      ///< a demo to show how to use player
#define USER_DEMO_FLASH       13      ///< a demo to show how to save value into flash
#define USER_DEMO_ASR_CONTROL 14      ///< a demo to show how to control asr state
#define USER_DEMO_UART_SUCP   15      ///< a demo to show how to use uart send and receive data with a sample uart communication protocol
#define USER_DEMO_AUTO_GPIO   16      ///< a demo to support UDP auto GPIO function, the code is automatically generated
#define USER_DEMO_POWER_SLEEP 17      ///< a demo to support deep sleep power mode
#define USER_RUN_DEMO_SELECT  USER_DEMO_AUTO_GPIO ///< select a demo to run

#define AUTO_RESET_TIME       0       ///< 系统自动重置时间（单位：s，0：关闭）
                                      ///< 当语音模组长时间未使用后会自动进行系统级重启
                                      ///< 在一些常供电（特别是不易上下电）的产品上，建议设置2～4小时的自动重置时间，以防开发过程中引入bug导致产品不可用且无法恢复
/** @}*/

/** @}*/

#endif

