#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "VariaType.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

#define USER_USART              USART1
#define USER_USART_ALTERNATE    GPIO_AF1_USART1

/* USER CODE BEGIN Private defines */
#define USER_USART_TX_PIN_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_USART_RX_PIN_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_USART_TX_PIN               GPIO_PIN_9
#define USER_USART_RX_PIN               GPIO_PIN_10
#define USER_USART_TX_PIN_GPIOX         GPIOA
#define USER_USART_RX_PIN_GPIOX         GPIOA

#define USER_USART_IRQHANDLER           USART1_IRQHandler


#ifndef OP_SUCCESS
#define OP_SUCCESS
#endif

#ifndef OP_FAILED
#define OP_FAILED
#endif



/**
* 名称       : BSP_USART_UART_Init()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口初始化
* 输入参数   : baudrate,波特率；parity,奇偶校验
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void BSP_USART_UART_Init(uint32_t baudrate, uint32_t parity);

/**
* 名称       : BSP_UART_Transmit_DMA()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口开始DMA发送
* 输入参数   : pData：DMA发送缓存指针;
*              Size：缓存大小
* 输出参数   : 无
* 返回结果   : OP_SUCCESS(成功),OP_FAILED(失败)
* 注意和说明 : 
* 修改内容   :
*/
uint8_t BSP_UART_Transmit_DMA(uint8_t *pData, uint16_t Size);

/**
* 名称       : BSP_UART_Receive_DMA()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口开始DMA接收
* 输入参数   : pData：DMA接收缓存指针;
*              Size：缓存大小
* 输出参数   : 无
* 返回结果   : OP_SUCCESS(成功),OP_FAILED(失败)
* 注意和说明 : 
* 修改内容   :
*/
uint8_t BSP_UART_Receive_DMA(uint8_t *pData, uint16_t Size);

/**
* 名称       : BSP_UART_ReceiverTimeout_Config()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口配置接收超时中断
* 输入参数   : TimeoutValue：超时时间，单位波特率位数;
*              例如：1个起始位，8位数据，1个停止位的串口配置，
*              则10位为1个数据字节，若设置4个数据字节的超时时间，
*              TimeoutValue = 40
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void BSP_UART_ReceiverTimeout_Config(uint32_t TimeoutValue);

/**
* 名称       : HAL_UART_RxTimoCallback()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口接收超时中断回调函数
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 该函数是个弱定义函数，可在其他文件中重定义，且是自定义的一个函数，在HAL库中不存在的
* 修改内容   :
*/
void HAL_UART_RxTimoCallback(UART_HandleTypeDef *huart);
/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
