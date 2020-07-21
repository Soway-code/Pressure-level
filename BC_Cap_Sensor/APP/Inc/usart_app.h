/**@file        usart_app.h
* @details      usart_app.c��ͷ�ļ�,�����˴���Ӧ�õĺ궨��,�����˴���Ӧ�õ�API����
* @author       ���
* @date         2020-04-29
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/04/29  <td>1.0.0    <td>���    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#ifndef __USART_APP_H
#define __USART_APP_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "usart_bsp.h"
#include "string.h"

#define USART_BAUDRATE_4800_CODE        0   ///< �����ʴ���,����4800 bit/s
#define USART_BAUDRATE_9600_CODE        1   ///< �����ʴ���,����9600 bit/s
#define USART_BAUDRATE_19200_CODE       2   ///< �����ʴ���,����19200 bit/s
#define USART_BAUDRATE_38400_CODE       3   ///< �����ʴ���,����38400 bit/s
#define USART_BAUDRATE_57600_CODE       4   ///< �����ʴ���,����57600 bit/s
#define USART_BAUDRATE_115200_CODE      5   ///< �����ʴ���,����115200 bit/s

#define USART_PARITY_NONE_CODE          0   ///< ��żУ�����,��������żУ��
#define USART_PARITY_ODD_CODE           1   ///< ��żУ�����,������У��
#define USART_PARITY_EVEN_CODE          2   ///< ��żУ�����,����żУ��

/** Ĭ�ϲ����� 9600 bit/s */
#define DEFAULT_UART_BAUD               9600    

/* ʹ��soway��λ����������(Boot����), BOOT_PROGRAM��main.h�ж��� */
#ifdef BOOT_PROGRAM
/** ���ջ���Ĵ�С, Boot����̶�ʹ�� 600 �ֽڳ��� */
#define RECEIVE_SIZE                    600
#else
/** ���ջ���Ĵ�С */
#define RECEIVE_SIZE                    128
#endif

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
#include "rtconfig.h"

#if defined(APP_USING_MODBUS_RTU)
#define USING_UART_TIMEOUT              ///< ʹ�ý��ճ�ʱ
#elif defined(APP_USING_MODBUS_ASCII)
#define USING_CHARMATCH                 ///< ʹ���ַ�ƥ��
#endif

#if defined(APP_USING_USART_485)
/** ����485�շ����ƹܽ�ʹ�� */
#define USART_USING_485
#endif

#else

//#define USING_UART_TIMEOUT              ///< ʹ�ý��ճ�ʱ
#define USING_CHARMATCH                 ///< ʹ���ַ�ƥ��

/** ����485�շ����ƹܽ�ʹ�� */
#define USART_USING_485

#endif

#ifdef  USING_UART_TIMEOUT
/** Ĭ�ϳ�ʱʱ�䣬10��ʾ1���ֽ�ʱ�� */
#define DEFAULT_UART_TIMEOUT            35
#endif

#ifdef  USING_CHARMATCH
/** Ĭ��ƥ����ַ� */
#define DEFAULT_UART_MATCHCHAR          0x0A
#endif



#ifdef  USART_USING_485
#define _485_RE_DE_PIN_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define _485_RE_DE_PIN                  GPIO_PIN_8
#define _485_RE_DE_GPIO_PORT            GPIOA

#define TX_ON   HAL_GPIO_WritePin(_485_RE_DE_GPIO_PORT, _485_RE_DE_PIN, GPIO_PIN_SET)
#define RX_ON   HAL_GPIO_WritePin(_485_RE_DE_GPIO_PORT, _485_RE_DE_PIN, GPIO_PIN_RESET)
#endif

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS

#define USART_TX_LOCK_NAME  "tx_lock"

#endif


/**@brief       ���ݲ����ʺ���żУ�������ʼ������������
* @param[in]    baudrate_code : �����ʴ���; 
* @param[in]    parity_code : ��żУ�����ʹ���;
* @return       ����ִ�н��
* - None
*/
void Sensor_USART_Init(uint8_t baudrate_code, uint8_t parity_code);

/**@brief       ��ȡʵ�ʽ��յ�����
* @param[out]   ReceData : ����ʵ�ʽ������ݵĻ���ָ��
* @return       ����ִ�н��
* - ��õ����ݳ���
* @note         ����ʹ����DMAѭ��ģʽ,���뾭�����������ܱ�֤�������ݵ�����
*/
uint16_t Sensor_USART_GetReceData(uint8_t *ReceData);
    
/**@brief       ��ȡ����������һ�ν��յ������ݳ���
* @return       ����ִ�н��
* - һ�ν��յ������ݳ���
*/
uint16_t Sensor_USART_Get_RX_Len(void);

/**@brief       ��ȡ���������ڽ��յ����ݺ󣬴��ڽ��ջ��汻ռ�ó���
* @return       ����ִ�н��
* - ���ڽ��ջ��汻ռ�ó���
*/
uint16_t Sensor_USART_Get_RXBuf_Used_Len(void);

/**@brief       ��ȡ���ո��±�־
* @return       ����ִ�н��
* - ���ո��±�־
*/
uint8_t Sensor_USART_Get_RX_Updata_Flag(void);

/**@brief       ��ȡ������ɱ�־
* @return       ����ִ�н��
* - ������ɱ�־
*/
uint8_t Sensor_USART_Get_TX_Cplt_Flag(void);

/**@brief       ������ո��±�־
* @return       ����ִ�н��
* - None
*/
void Sensor_USART_Clear_RX_Updata_Flag(void);

/**@brief       ���������ɱ�־
* @return       ����ִ�н��
* - None
*/
void Sensor_USART_Clear_TX_Cplt_Flag(void);

#ifdef __cplusplus
}
#endif
#endif
