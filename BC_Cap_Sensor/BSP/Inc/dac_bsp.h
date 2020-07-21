/**@file        dac_bsp.h
* @details      dac_bsp.c��ͷ�ļ�,������DAC��API����
* @author       ���
* @date         2020-04-27
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/04/27  <td>1.0.0    <td>���    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#ifndef __dac_H
#define __dac_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


#define DACx                            DAC
#define DACx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#if defined(STM32F0)
#define DACx_CLK_ENABLE()               __HAL_RCC_DAC1_CLK_ENABLE()
#define DACx_CLK_DISABLE()              __HAL_RCC_DAC1_CLK_DISABLE()
#elif defined(STM32L0)
#define DACx_CLK_ENABLE()               __HAL_RCC_DAC_CLK_ENABLE()
#define DACx_CLK_DISABLE()              __HAL_RCC_DAC_CLK_DISABLE()
#endif

/* Definition for DACx Channel Pin */
#define DACx_CHANNEL1_PIN               GPIO_PIN_4
#define DACx_CHANNEL1_GPIO_PORT         GPIOA

#define DACx_CHANNEL2_PIN               GPIO_PIN_5
#define DACx_CHANNEL2_GPIO_PORT         GPIOA


//#define DAC_CHANNEL_1_ENABLE
#define DAC_CHANNEL_2_ENABLE

typedef enum {
    DA_CHANNEL_1,
    DA_CHANNEL_2,
    DA_CHANNEL_TOTAL
}DA_CHANNEL_NUM;

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
#include "board.h"

#define DAC_DEVICE_NAME     "dac"

#endif


/**@brief       DAC��ʼ��
* @return       ����ִ�н��
* - None
* @note         DACͨ��ͨ��DAC_CHANNEL_1_ENABLE��DAC_CHANNEL_2_ENABLE�궨���
*/
void BSP_DAC_Init(void);

/**@brief       ָ��DACͨ��д��DAֵ
* @param[in]    DA_Value : DACֵ
* @param[in]    DA_Channel_Num : DACͨ����
* @return       ����ִ�н��
* - None
* @note         DAֵ���ܳ���4095; ͨ���ż�DA_CHANNEL_NUM�Ķ���,
* ��ֵֻ��ȡDA_CHANNEL_NUM�����ֵ
*/
uint8_t BSP_DAC_Convert(uint16_t DA_Value, DA_CHANNEL_NUM DA_Channel_Num);

#ifdef __cplusplus
}
#endif
#endif /*__ dac_H */
