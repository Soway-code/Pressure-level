#ifndef __USART_APP_H
#define __USART_APP_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "usart_bsp.h"
#include "string.h"

#define USART_BAUDRATE_4800_CODE        0
#define USART_BAUDRATE_9600_CODE        1
#define USART_BAUDRATE_19200_CODE       2
#define USART_BAUDRATE_38400_CODE       3
#define USART_BAUDRATE_57600_CODE       4
#define USART_BAUDRATE_115200_CODE      5

#define USART_PARITY_NONE_CODE          0
#define USART_PARITY_ODD_CODE           1
#define USART_PARITY_EVEN_CODE          2

/* 默认波特率 9600 bit/s */
#define DEFAULT_UART_BAUD               9600    

/* 默认超时时间，35表示3.5个字节时间 */
#define DEFAULT_UART_TIMEOUT            35

#define USART_USING_485

#ifdef  USART_USING_485
#define _485_RE_DE_PIN_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define _485_RE_DE_PIN                  GPIO_PIN_8
#define _485_RE_DE_GPIO_PORT            GPIOA

#define TX_ON   HAL_GPIO_WritePin(_485_RE_DE_GPIO_PORT, _485_RE_DE_PIN, GPIO_PIN_SET)
#define RX_ON   HAL_GPIO_WritePin(_485_RE_DE_GPIO_PORT, _485_RE_DE_PIN, GPIO_PIN_RESET)
#endif

/**
* 名称       : Sensor_USART_Init()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 根据波特率和奇偶校验参数初始化传感器串口
* 输入参数   : baudrate_code,波特率代码; parity_code,奇偶校验类型代码;
*              Receive_Data,接收缓存指针; Receive_Data_Size,接收缓存大小;
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Sensor_USART_Init(uint8_t baudrate_code, uint8_t parity_code, 
                    uint8_t *Receive_Data, uint16_t Receive_Data_Size);

/**
* 名称       : Sensor_USART_Get_RX_Len()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取传感器串口一次接收到的数据长度
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 一次接收到的数据长度
* 注意和说明 : 
* 修改内容   :
*/
uint16_t Sensor_USART_Get_RX_Len(void);

/**
* 名称       : Sensor_USART_Get_RXBuf_Used_Len()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取传感器串口接收到数据后，从接收缓存头部到数据尾部的长度
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 从接收缓存头部到数据尾部的长度
* 注意和说明 : 
* 修改内容   :
*/
uint16_t Sensor_USART_Get_RXBuf_Used_Len(void);

/**
* 名称       : Sensor_USART_Get_BaudRate_Code()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取传感器串口的波特率代码
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 串口的波特率代码
* 注意和说明 : 
* 修改内容   :
*/
uint8_t Sensor_USART_Get_BaudRate_Code(void);

/**
* 名称       : Sensor_USART_Get_Parity_Code()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取传感器串口的奇偶校验代码
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 串口的奇偶校验代码
* 注意和说明 : 
* 修改内容   :
*/
uint8_t Sensor_USART_Get_Parity_Code(void);

/**
* 名称       : Sensor_USART_Get_RX_Updata_Flag()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取接收更新标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 接收更新标志
* 注意和说明 : 
* 修改内容   :
*/
uint8_t Sensor_USART_Get_RX_Updata_Flag(void);

/**
* 名称       : Sensor_USART_Get_TX_Cplt_Flag()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取发送完成标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 发送完成标志
* 注意和说明 : 
* 修改内容   :
*/
uint8_t Sensor_USART_Get_TX_Cplt_Flag(void);

/**
* 名称       : Sensor_USART_Clear_RX_Updata_Flag()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 清除接收更新标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Sensor_USART_Clear_RX_Updata_Flag(void);

/**
* 名称       : Sensor_USART_Clear_TX_Cplt_Flag()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 清除发送完成标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Sensor_USART_Clear_TX_Cplt_Flag(void);

#ifdef __cplusplus
}
#endif
#endif
