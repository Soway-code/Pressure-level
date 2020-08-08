/**@file        adc_app.h
* @details      adc_app.c��ͷ�ļ�,������ADCӦ�õĺ궨��,������ADCӦ�õ�API����
* @author       ���
* @date         2020-05-06
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/05/06  <td>1.0.0    <td>���    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#ifndef __ADC_APP_H
#define __ADC_APP_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "adc_bsp.h"
#include "In_Memory_app.h"
#include "VariaType.h"

#if defined(STM32F0)
#define VREF30ADDR          ((uint16_t*) ((uint32_t) 0x1FFFF7B8))   ///< 30���϶�ADC�ο�ֵ��ַ
#define VREF110ADDR         ((uint16_t*) ((uint32_t) 0x1FFFF7C2))   ///< 110���϶�ADC�ο�ֵ��ַ
#elif defined(STM32L0)
#define VREF30ADDR          ((uint16_t*) ((uint32_t) 0x1FF8007A))   ///< 130���϶�ADC�ο�ֵ��ַ
#define VREF130ADDR         ((uint16_t*) ((uint32_t) 0x1FF8007E))   ///< 130���϶�ADC�ο�ֵ��ַ
#define VDD_CALIB           ((uint16_t) (300))
#define VDD_APPLI           ((uint16_t) (330))
#endif // defined(STM32F0) or defined(STM32L0)

#define AD_GROUP_MAX        10                                  ///< ÿ��ͨ���ɼ��������û������Զ���
#define AD_CHANNEL_MAX      AD_CHANNEL_TOTAL                    ///< ʹ��ADCͨ�������û������Զ���
#define AD_SEQBUFF_MAX      (AD_GROUP_MAX * AD_CHANNEL_MAX)     ///< ADC���л����С

/** ADC�¶ȴ�����Ҫ�Ĳ����ṹ */
typedef struct {
    float Temper_K1;                                            ///< �¶�1����ϵ��K1
    float Temper_B1;                                            ///< �¶�1����ϵ��B1
    float Temper_K2;                                            ///< �¶�2����ϵ��K2
    float Temper_B2;                                            ///< �¶�2����ϵ��B2
    uint16_t TempDAMin;                                         ///< �¶�DAֵ���
    uint16_t TempDAMax;                                         ///< �¶�DAֵ������
    uint16_t TempDARange;                                       ///< �¶�DAֵ����
}ADC_TemperParam_TypeDef;

/** ADC�¶ȴ���������ݵĽṹ */
typedef struct {
    uint16_t TemperInAirAD;                                     ///< �����¶�ADֵ
    uint16_t TemperInLiquidAD;                                  ///< Һ���¶�ADֵ
    float TemperInAir;                                          ///< �����¶�
    float TemperInLiquid;                                       ///< Һ���¶�
}ADC_TemperOut_TypeDef;

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
#include <rtthread.h>

#define ADC_DEVICE_NAME         "adc"

/** ADC�豸���� */
struct rt_adc_device_obj {
    struct rt_device        dev;
    ADC_TemperParam_TypeDef ADC_TemperParam;
    ADC_TemperOut_TypeDef   ADC_TemperOut;
};
#endif // USING_RT_THREAD_OS

#ifdef __IN_MEMORY_APP_H

/**@brief       ��ʼ���¶�ת����Ҫ�Ĳ����ṹ
* @param[out]   ADC_TemperParam : �¶�ת����Ҫ�Ĳ����ṹָ��; 
* @return       ����ִ�н��
* - None
* @note         Ҫʹ�ñ�����,Ҫ����In_Memory_app.c��In_Memory_app.h��
* In_Flash.c��In_Flash.h�ļ�(STM32L0ϵ�������In_EEPROM.c��In_EEPROM.h�ļ�)
*/
void Sensor_ADC_TemperParam_Init(ADC_TemperParam_TypeDef *ADC_TemperParam);
#else

/**@brief       ��ʼ���¶�ת����Ҫ�Ĳ����ṹ
* @param[in]    Param : Ҫд��ADC_TemperParam�Ĳ�������ָ��;
* @param[out]   ADC_TemperParam : �¶�ת����Ҫ�Ĳ����ṹָ��; 
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         Param����������(���ֽ�˳������): 0~1,�¶�1����ϵ��K1; \n
* 2~3,�¶�1����ϵ��B1; 4~5,�¶�2����ϵ��K2; 6~7,�¶�2����ϵ��B2; \n
* 8~9,�¶�DAת�����ֵ; 10~11,�¶�DAת������ֵ;
*/
uint8_t Sensor_ADC_TemperParam_Init(ADC_TemperParam_TypeDef *ADC_TemperParam, uint8_t *Param, 
                            uint16_t Param_Size);
#endif // __IN_MEMORY_APP_H

/**@brief       ��ʼ��ADC�����stm32оƬ�ڲ��¶ȴ��������¶ȱ仯б�ʣ�����ADC��DMA����
* @return       ����ִ�н��
* - None
*/
void Sensor_ADC_Init(void);

/**@brief       �û���ȡADCͨ����ƽ���˲��������
* @param[in]    Channel_Num : ͨ����,ָ����ȡ��ͨ��;
* @return       ����ִ�н��
* - ƽ���˲����ADCֵ
*/
uint16_t Sensor_ADC_GetChn_Value(AD_CHANNEL_NUM AD_Channel_Num);

/**@brief       ��ȡADC�¶�����
* @return       ����ִ�н��
* - �¶�ֵ
* @note         �¶ȼ���ɲο�STM32F072�����ֲ�
*/
float Sensor_ADC_Get_TemperData(void);

/**@brief       ��ȡADC���ݸ��±�־
* @return       ����ִ�н��
* - ADC���±�־
*/
uint8_t Sensor_ADC_Get_Updata_Flag(void);
    
/**@brief       ���ADC���±�־
* @return       ����ִ�н��
* - None
*/
void Sensor_ADC_Clean_Updata_Flag(void);

#ifdef __cplusplus
}
#endif
#endif // __ADC_APP_H
