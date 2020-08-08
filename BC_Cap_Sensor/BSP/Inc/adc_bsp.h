/**@file        adc_bsp.h
* @details      adc_bsp.c��ͷ�ļ�,������ADC��API����
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

#ifndef __ADC_BSP_H
#define __ADC_BSP_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


//#define ADC_CHANNEL_0_ENABLE
//#define ADC_CHANNEL_1_ENABLE
#define ADC_CHANNEL_2_ENABLE
//#define ADC_CHANNEL_3_ENABLE
//#define ADC_CHANNEL_6_ENABLE
//#define ADC_CHANNEL_7_ENABLE
//#define ADC_CHANNEL_8_ENABLE
//#define ADC_CHANNEL_9_ENABLE
#define ADC_CHANNEL_TEMPSENSOR_ENABLE

typedef enum {
#ifdef ADC_CHANNEL_0_ENABLE
    AD_CHANNEL_0,
#endif // ADC_CHANNEL_0_ENABLE
    
#ifdef ADC_CHANNEL_1_ENABLE
    AD_CHANNEL_1,
#endif // ADC_CHANNEL_1_ENABLE
    
#ifdef ADC_CHANNEL_2_ENABLE
    AD_CHANNEL_2,
#endif // ADC_CHANNEL_2_ENABLE
    
#ifdef ADC_CHANNEL_3_ENABLE
    AD_CHANNEL_3,
#endif // ADC_CHANNEL_3_ENABLE
    
#ifdef ADC_CHANNEL_6_ENABLE
    AD_CHANNEL_6,
#endif // ADC_CHANNEL_6_ENABLE
    
#ifdef ADC_CHANNEL_7_ENABLE
    AD_CHANNEL_7,
#endif // ADC_CHANNEL_7_ENABLE

#ifdef ADC_CHANNEL_8_ENABLE
    AD_CHANNEL_8,
#endif // ADC_CHANNEL_8_ENABLE

#ifdef ADC_CHANNEL_9_ENABLE
    AD_CHANNEL_9,
#endif // ADC_CHANNEL_9_ENABLE

#ifdef ADC_CHANNEL_TEMPSENSOR_ENABLE
    AD_CHANNEL_TEMPSENSOR,
#endif // ADC_CHANNEL_TEMPSENSOR_ENABLE

    AD_CHANNEL_TOTAL
}AD_CHANNEL_NUM;

/* Definition for ADCx clock resources */
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()

#define DMAx_CHANNELx_CLK_ENABLE()      __HAL_RCC_DMA1_CLK_ENABLE()

#define ADCx_CLK_DISABLE()              __HAL_RCC_ADC1_CLK_DISABLE()

/* Definition for ADCx's DMA */
#define ADCx_DMA_STREAM                 DMA1_Channel1

#if defined(STM32L0)
#define ADCx_DMA_REQUEST                DMA_REQUEST_0
#endif // defined(STM32L0)

#define ADCx_DMA_IRQHandler             DMA1_Channel1_IRQHandler    ///< ADC DMA�жϷ�����
#define ADCx_IRQHandler                 ADC1_COMP_IRQHandler        ///< ADC�жϷ�����

#define ADCx_DMA_IRQn                   DMA1_Channel1_IRQn
#define ADCx_IRQn                       ADC1_COMP_IRQn


/**@brief       ADC��ʼ��
* @return       ����ִ�н��
* - None
* @note         ADCͨ��ͨ��ADC_CHANNEL_0_ENABLE, ADC_CHANNEL_1_ENABLE, ..., 
* ADC_CHANNEL_9_ENABLE, ADC_CHANNEL_TEMPSENSOR_ENABLE�궨���
*/
void BSP_ADC_Init(void);


/**@brief       ADC��ʼDMA����
* @param[in]    pData : DMA���ջ���ָ��;
* @param[in]    Length : �����С
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t BSP_ADC_Start_DMA(uint32_t *pData, uint32_t Length);

void BSP_ADC_DMA_Enable_IT(void);
void BSP_ADC_DMA_Disable_IT(void);


#ifdef __cplusplus
}
#endif
#endif // __ADC_BSP_H
