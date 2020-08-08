/**@file        Modbus_Conf.h
* @details      Modbus_Conf.c��ͷ�ļ�,�����˵��ݴ������궨��API����,������
* �豸�����ṹ��
* @author       ׯ��Ⱥ
* @date         2020-07-20
* @version      V2.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2020/07/20  <td>2.0.0    <td>ׯ��Ⱥ  <td>���      <td>����ӵĳ������(ָ��ִ�в���)
* </table>
*
**********************************************************************************
*/

#ifndef __MODBUS_CONF_H
#define __MODBUS_CONF_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "Picocap_app.h"
#include "adc_app.h"


/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
#include "rtconfig.h"

#if defined(APP_USING_MODBUS_RTU)
#define USING_MODBUS_RTU            ///< ʹ��ModBus RTUЭ��
#elif defined(APP_USING_MODBUS_ASCII)
#define USING_MODBUS_ASCII          ///< ʹ��ModBus ASCIIЭ��
#endif // defined(APP_USING_MODBUS_RTU) or defined(APP_USING_MODBUS_ASCII)

#if defined(APP_SUBCODE_IS_DEVADDR)
#define SUBCODE_IS_DEVADDR          ///< ����ModBus����Ϊ�豸��ַ����������Ĭ��Ϊ0
#endif // defined(APP_SUBCODE_IS_DEVADDR)

#else

#define USING_MODBUS_RTU            ///< ʹ��ModBus RTUЭ��, ����Ҫ�� usart_app.h �ﶨ��USING_UART_TIMEOUT
//#define USING_MODBUS_ASCII          ///< ʹ��ModBus ASCIIЭ��, ����Ҫ�� usart_app.h �ﶨ��USING_CHARMATCH
//#define SUBCODE_IS_DEVADDR          ///< ����ModBus����Ϊ�豸��ַ����������Ĭ��Ϊ0(�� DEFAULT_SUBCODE)

#endif // USING_RT_THREAD_OS

#define AUTOUPLOAD_CYCLE                1000            ///< �Զ��ϴ�����,��λ ms

#define SEND_SIZE                       128              ///< ���ͻ����С

#define DAC_VALUE_MAX   4095        ///< DAC���ֵ

#ifndef SUBCODE_IS_DEVADDR
#define DEFAULT_SUBCODE                 0
#endif  // SUBCODE_IS_DEVADDR

/* ʹ��soway��λ����������(Boot����), BOOT_PROGRAM��main.h�ж��� */
#ifdef BOOT_PROGRAM

#define RESPONSE_ERR_NONE   0     //��Ӧ�ɹ�
#define RESPONSE_REC_ERR    1     //���մ���
#define RESPONSE_LRC_ERR    2     //У�������

#define UPGRADED_DEVICEADDR     65

#define ERASE_FLAG          0x0C
#define ERASE_FLAG_NONE     0xFF

#define UPGRADE_FLAG        0x0C
#define UPGRADE_FLAG_NONE   0xFF

#endif // BOOT_PROGRAM

#if defined(STM32F0)
#define ADDR_DEVICEADDR     (IN_FLASH_END - 2)
#define ADDR_ERASEFLAG      (IN_FLASH_END - 1)
#define ADDR_UPGRADEFLAG    IN_FLASH_END
#elif defined(STM32L0)
#define ADDR_DEVICEADDR     (IN_EEPROM_END - 2)
#define ADDR_ERASEFLAG      (IN_EEPROM_END - 1)
#define ADDR_UPGRADEFLAG    IN_EEPROM_END
#endif // defined(STM32F0) or defined(STM32L0)

/** ModBus�����豸�Ĳ����ṹ,�ɸ��ݲ�ͬ�Ĳ�Ʒ�����ɾ����Ա */
typedef struct {
    DataFilterParam *DataFilter;
    PCap_DataConvert_Param *PCap_DataConvert;
    PCap_DataConvert_Out_Param *PCap_DataConvert_Out; 
    ADC_TemperParam_TypeDef *ADC_TemperParam;
    ADC_TemperOut_TypeDef   *ADC_TemperOut;
}ModBus_Device_Param;


/**@brief       ���ݱ궨
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void MB_Cap_Calibration(void *arg);

/**@brief       ����ADֵ�궨
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void MB_CapAD_Calibration(void *arg);

/**@brief       ����DAֵ�궨
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void MB_CapDAOut_Calibration(void *arg);

/**@brief       �¶�DAֵ�궨����
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void MB_TempDAOut_Calibration(void *arg);

#ifdef __cplusplus
}
#endif
#endif // __MODBUS_CONF_H
