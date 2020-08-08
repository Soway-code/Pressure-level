/**@file        usart_bsp.h
* @details      usart_bsp.c��ͷ�ļ�,�����˴���������API����
* @author       ���
* @date         2020-04-28
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/04/28  <td>1.0.0    <td>���    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "VariaType.h"
#if defined(STM32F0)
#include "stm32f0xx_ll_usart.h"
#elif defined(STM32L0)
#include "stm32l0xx_ll_usart.h"
#endif // defined(STM32F0) or defined(STM32L0)

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_CLK_DISABLE()             __HAL_RCC_USART1_CLK_DISABLE()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#if defined(STM32F0)
#define USARTx_TX_AF                     GPIO_AF1_USART1
#elif defined(STM32L0)
#define USARTx_TX_AF                     GPIO_AF4_USART1
#endif // defined(STM32F0) or defined(STM32L0)

#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA
#if defined(STM32F0)
#define USARTx_RX_AF                     GPIO_AF1_USART1
#elif defined(STM32L0)
#define USARTx_RX_AF                     GPIO_AF4_USART1
#endif // defined(STM32F0) or defined(STM32L0)

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_STREAM              DMA1_Channel2
#define USARTx_RX_DMA_STREAM              DMA1_Channel3

#if defined(STM32L0)
#define USARTx_TX_DMA_REQUEST           DMA_REQUEST_3
#define USARTx_RX_DMA_REQUEST           DMA_REQUEST_3
#endif // defined(STM32L0)

/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_RX_IRQn             DMA1_Channel2_3_IRQn
#define USARTx_DMA_TX_RX_IRQHandler       DMA1_Channel2_3_IRQHandler

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler


#ifndef OP_SUCCESS
#define OP_SUCCESS
#endif // OP_SUCCESS

#ifndef OP_FAILED
#define OP_FAILED
#endif // OP_FAILED


/**@brief       ���ڳ�ʼ��
* @param[in]    baudrate : ������; 
* @param[in]    parity : ��żУ��;
* @return       ����ִ�н��
* - None
*/
void BSP_USART_UART_Init(uint32_t baudrate, uint32_t parity);

/**@brief       ���ڷ���ʼ��
* @return       ����ִ�н��
* - None
*/
void BSP_USART_UART_DeInit(void);
    
/**@brief       ����DMA����
* @param[in]    pData : DMA���ͻ���ָ��;
* @param[in]    Size : �����С
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t BSP_UART_Transmit_DMA(uint8_t *pData, uint16_t Size);

/**@brief       ����DMA����
* @param[in]    pData : DMA���ջ���ָ��;
* @param[in]    Size : �����С
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t BSP_UART_Receive_DMA(uint8_t *pData, uint16_t Size);

/**@brief       �������ý��ճ�ʱ�ж�
* @param[in]    TimeoutValue : ��ʱʱ�䣬��λ������λ��;
* @return       ����ִ�н��
* - None
* @note         ʾ����1����ʼλ��8λ���ݣ�1��ֹͣλ�Ĵ������ã�
* ��10λΪ1�������ֽڣ�������4�������ֽڵĳ�ʱʱ�䣬
* TimeoutValue = 40
*/
void BSP_UART_ReceiverTimeout_Config(uint32_t TimeoutValue);

/**@brief       ���������ַ�ƥ���ж�
* @param[in]    Value : ƥ����ַ�;
* @return       ����ִ�н��
* - None
*/
void BSP_UART_CharMatch_Config(uint8_t Value);

/**@brief       ���ڽ��ճ�ʱ�жϻص�����
* @param[in]    huart : ���ڴ������;
* @return       ����ִ�н��
* - None
* @note         �ú������������ļ����ض��壬�����Զ����һ����������HAL���в����ڵ�
*/
void HAL_UART_RxTimoCallback(UART_HandleTypeDef *huart);

/**@brief       �����ַ�ƥ���жϻص�����
* @param[in]    huart : ���ڴ������;
* @return       ����ִ�н��
* - None
* @note         �ú������������ļ����ض��壬�����Զ����һ����������HAL���в����ڵ�
*/
void HAL_UART_CMatchCallback(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif
#endif // __usart_H
