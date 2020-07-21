/**@file        Picocap_app.h
* @details      Picocap_app.c��ͷ�ļ�,������PCapӦ�õĺ궨��,������PCapӦ�õ�API����
* @author       ���
* @date         2020-04-30
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/04/30  <td>1.0.0    <td>���    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#ifndef __PICOCAP_APP_H
#define __PICOCAP_APP_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "Picocap.h"
#include "In_Memory_app.h"


#define PCAP_COLLECT_CYCLE              100     ///< PCap�ɼ����ڣ���λ ms

#define DATA_BUF_MAX                    10      ///< PCap����ԭʼ����ֵ�Ļ�������

#define DATA_2ND_FILTER_BUF_MAX         10      ///< PCap�����˲���������

#define DATA_1ST_FILTER_BUF_MAX         96      ///< PCapһ���˲���������

#define PCAP_DAC_MIN_VALUE              0       ///< PCap DAC�����Сֵ

#define PCAP_DAC_MAX_VALUE              4095    ///< PCap DAC������ֵ

#define PCAP_ADC_MIN_VALUE              0       ///< PCap ADC�����Сֵ

#define PCAP_ADC_MAX_VALUE              65535   ///< PCap ADC������ֵ

/** �����˲���Ҫ�Ĳ����ṹ */
typedef struct {    
    uint8_t FilterStart;                ///< �˲���ʼ��־
    uint8_t InputCountMax;              ///< �ۼ���������ֵ������ΪDATA_BUF_MAX
    uint8_t FilterFactor;               ///< �˲�ϵ��
    uint8_t FilterBufMax;               ///< �˲���󻺴�
    uint16_t FilterCycle;               ///< �˲�����
    uint32_t InputRangeMax;             ///< ���뷶Χ���ֵ
    uint32_t InputRangeMin;             ///< ���뷶Χ��Сֵ 
}DataFilterParam;
    
/** PCap������ת����Ҫ�Ĳ����ṹ */
typedef struct {
    uint8_t CompenEn;                   ///< ����ʹ��
    uint16_t HeightRange;               ///< �߶�����
    uint16_t CapADMin;                  ///< ����ADֵ���
    uint16_t CapADLow;                  ///< ����ADֵ�¿̶�
    uint16_t CapADHigh;                 ///< ����ADֵ�Ͽ̶�
    uint16_t CapADMax;                  ///< ����ADֵ������
    uint16_t CapDAMin;                  ///< ����DAֵ���
    uint16_t CapDALow;                  ///< ����DAֵ�¿̶�
    uint16_t CapDAHigh;                 ///< ����DAֵ�Ͽ̶�
    uint16_t CapDAMax;                  ///< ����DAֵ������
    uint32_t CapMin;                    ///< �������
    uint32_t CapMax;                    ///< ����������
    uint32_t CapMinBak;                 ///< �������
    uint32_t CapMaxBak;                 ///< ����������
    uint8_t CapDA_ClibEn;               ///< ����DA�궨ʹ��
    float Correct_K;                    ///< ��������ϵ��K
    float Correct_B;                    ///< ��������ϵ��B
}PCap_DataConvert_Param;

/** PCapת����������ݵĽṹ */
typedef struct {
    uint16_t LiquidHeightAD;            ///< Һλ�߶�ADֵ
    float LiquidHeight;                 ///< Һλ�߶�
    uint16_t PCapDA_ResultValue;        ///< ����DAԭʼ���ֵ
    uint16_t PCapDA_OutValue;           ///< ����DAת�����ֵ
    uint32_t PCap_ResultValue;          ///< PCapԭʼֵ
}PCap_DataConvert_Out_Param;

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
#include <rtthread.h>

#define PCAP_DEVICE_NAME        "pcap"

struct rt_pcap_device_obj {
    struct rt_device            dev;
    DataFilterParam             DataFilter;
    PCap_DataConvert_Param      PCap_DataConvert;
    PCap_DataConvert_Out_Param  PCap_DataConvert_Out;
};
#endif


/**@brief       �л��˲�ˮƽ����
* @param[in]    FiltFactor : �˲�ϵ��;
* @param[in]    FilterParam : ָ���˲������ṹ��;
* @return       ����ִ�н��
* - None
* @note         �˲�ˮƽ��9�������ִӵ͵��߶�Ӧ�˲���ȴӵ͵���
*/
void SwitchCurFilter(uint8_t FiltFactor, DataFilterParam *FilterParam);

#ifdef __IN_MEMORY_APP_H

/**@brief       ��ʼ�������˲���Ҫ�Ĳ����ṹ
* @param[in]    Filterfactor_CountMax : �˲�ϵ�����ۼ���������ֵ,�˲�ϵ���ڸ�8λ,
* �ۼ���������ֵ�ڵ�8λ;
* @param[out]   FilterParam : �����˲���Ҫ�Ĳ����ṹָ��;
* @return       ����ִ�н��
* - None
* @note         Ҫʹ�ñ�����,Ҫ����In_Memory_app.c��In_Memory_app.h��
* In_Flash.c��In_Flash.h�ļ�(STM32L0ϵ�������In_EEPROM.c��In_EEPROM.h�ļ�)
*/
void DataFilterParam_Init(DataFilterParam *FilterParam, uint16_t Filterfactor_CountMax);
#else

/**@brief       ��ʼ�������˲���Ҫ�Ĳ����ṹ
* @param[in]    Filterfactor_CountMax : �˲�ϵ�����ۼ���������ֵ,�˲�ϵ���ڸ�8λ,
* �ۼ���������ֵ�ڵ�8λ;
* @param[in]    InputRangeMax : ���뷶Χ���ֵ;
* @param[in]    InputRangeMin : ���뷶Χ��Сֵ;
* @param[out]   FilterParam : �����˲���Ҫ�Ĳ����ṹָ��;
* @return       ����ִ�н��
* - None
*/
void DataFilterParam_Init(DataFilterParam *FilterParam, uint16_t Filterfactor_CountMax,
                            uint32_t InputRangeMax, uint32_t InputRangeMin);
#endif

#ifdef __IN_MEMORY_APP_H

/**@brief       ��ʼ������ת����Ҫ�Ĳ����ṹ
* @param[out]   DataConvert_Param : ����ת����Ҫ�Ĳ����ṹָ��;
* @return       ����ִ�н��
* - None
* @note         Ҫʹ�ñ�����,Ҫ����In_Memory_app.c��In_Memory_app.h��
* In_Flash.c��In_Flash.h�ļ�(STM32L0ϵ�������In_EEPROM.c��In_EEPROM.h�ļ�)
*/
void DataConvertParam_Init(PCap_DataConvert_Param *DataConvert_Param);
#else

/**@brief       ��ʼ������ת����Ҫ�Ĳ����ṹ
* @param[in]    CompenEn : ����ʹ��;
* @param[in]    Param : Ҫд��DataConvert_Param�Ĳ�������ָ��;
* @param[in]    Param_Size : Param�Ĵ�С;
* @param[out]   DataConvert_Param : ����ת����Ҫ�Ĳ����ṹָ��;
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         Param�����ݷֲ�(���ֽڷֲ�): 0~1,λ�Ƹ߶�; 2~5,�����������;
* 6~9,������������; 10~13,�����������; 14~17,������������; 18~19,����ADֵ���; 
* 20~21,����ADֵ�¿̶�; 22~23,����ADֵ�Ͽ̶�; 24~25,����ADֵ������; 
* 26~27,����DAֵ���; 28~29,����DAֵ�¿̶�; 30~31,����DAֵ�Ͽ̶�; 
* 32~33,����DAֵ������; 34~35,��������ϵ��K; 36~37,��������ϵ��B;
*/
uint8_t DataConvertParam_Init(PCap_DataConvert_Param *DataConvert_Param, uint8_t CompenEn,
                            uint8_t *Param, uint16_t Param_Size);
#endif

/**@brief       ��ȡPCapԭʼ�ɼ�ֵ
* @param[in]    reg_addr : ����Ĵ����ĵ�ַ;
* @param[out]   PCap_Result : ����PCap��������;
* @param[in]    Read_Cnt : ��ȡ�ĵ��ݸ���;
* @return       ����ִ�н��
* - OP_SUCCESS(�����ɹ�)
* - OP_FAILED(����ʧ��)
*/
uint8_t Sensor_PCap_GetResult(uint8_t reg_addr, uint32_t *PCap_Result, uint8_t Read_Cnt);

/**@brief       ���ݰ�ָ���˲����������˲�
* @param[in]    FilterParam : ָ���˲������ṹ��;
* @param[in]    InputValue : ���������;
* @param[out]   OutputValue : ������ݵ�ָ��;
* @return       ����ִ�н��
* - OP_SUCCESS(�����ɹ�)
* - OP_FAILED(����ʧ��)
* @note         ʹ�ñ�����ǰ,�Ƚ�FilterParam��ʼ��(��������ÿһ����Ա�����ʵ�ֵ),����������ʹ��
*/
uint8_t Sensor_DataFilter(DataFilterParam *FilterParam, uint32_t InputValue, uint32_t *OutputValue);

/**@brief       ���ݰ�ָ�����Ʋ�����������ת��
* @param[in]    DataConvert_Param : ָ�����Ʋ����ṹ��;
* @param[in]    InputValue : ���������;
* @param[out]   DataConvert_Out : ������ݵĲ����ṹ��ָ��
* @return       ����ִ�н��
* - None
* @note         ʹ�ñ�����ǰ,�Ƚ�DataConvert_Param��ʼ��(��������ÿһ����Ա�����ʵ�ֵ),����������ʹ��
*/
void Sensor_PCap_DataConvert(PCap_DataConvert_Param *DataConvert_Param, 
                                uint32_t InputValue, 
                                PCap_DataConvert_Out_Param *DataConvert_Out);

#ifdef __cplusplus
}
#endif
#endif
