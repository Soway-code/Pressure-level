/**@file        In_Memory_app.h
* @details      In_Memory_app.c��ͷ�ļ�,�������ڲ��洢��Ӧ�õ�API����
* @author       ���
* @date         2020-07-20
* @version      V2.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2020/07/20  <td>2.0.0    <td>ׯ��Ⱥ  <td>���      <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/
#ifndef __IN_MEMORY_APP_H
#define __IN_MEMORY_APP_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

#if defined(STM32F0)
#include "In_Flash.h"
#elif defined(STM32L0)
#include "In_EEPROM.h"
#endif // defined(STM32F0) or defined(STM32L0)
#include "TypeConvert.h"

/***************************** �豸�������ڲ��洢���е�ӳ���ַ ****************************/
#define RUN_ADDR_BASE                     0x00                                  ///< ϵͳ������ʼ����־/�ڲ�Flash��ʼ��ַ
#define DEVICE_ADDR                       (RUN_ADDR_BASE + 0x01)                ///< �豸��ַ
#define BAUDRATE                          (DEVICE_ADDR   + 0x01)                ///< ������
#define PARITY                            (BAUDRATE      + 0x01)                ///< ��żУ��
#define FILTER                            (PARITY        + 0x01)                ///< �˲�ϵ��
#define AUTO_UPLOAD                       (FILTER        + 0x01)                ///< �Զ��ϴ�����
#define COMPENSATE                        (AUTO_UPLOAD   + 0x01)                ///< ����ʹ��
#define FREEZE                            (COMPENSATE    + 0x01)                ///< �Ƿ񶳽��豸
#define OUTPUTMODE                        (FREEZE        + 0x01)                ///< �����ʽ
#define HEIGHTRANGE                       (OUTPUTMODE    + 0x01)                ///< ����
#define CAPMIN                            (HEIGHTRANGE   + 0x02)                ///< ������
#define CAPMAX                            (CAPMIN        + 0x04)                ///< �����̵���
#define CAPMINBAK                         (CAPMAX        + 0x04)                ///< �����ݱ���
#define CAPMAXBAK                         (CAPMINBAK     + 0x04)                ///< �����̵��ݱ���
#define CAPADMIN                          (CAPMAXBAK     + 0x04)                ///< �������ADֵ
#define CAPADLOW                          (CAPADMIN      + 0x02)                ///< �����¿̶�ADֵ
#define CAPADHIGH                         (CAPADLOW      + 0x02)                ///< �����Ͽ̶�ADֵ
#define CAPADMAX                          (CAPADHIGH     + 0x02)                ///< ����������ADֵ
#define CAPDAMIN                          (CAPADMAX      + 0x02)                ///< �������DAֵ
#define CAPDALOW                          (CAPDAMIN      + 0x02)                ///< �����¿̶�DAֵ
#define CAPDAHIGH                         (CAPDALOW      + 0x02)                ///< �����Ͽ̶�DAֵ
#define CAPDAMAX                          (CAPDAHIGH     + 0x02)                ///< ����������DAֵ
#define TEMPDAMIN                         (CAPDAMAX      + 0x02)                ///< �¶����DAֵ
#define TEMPDAMAX                         (TEMPDAMIN     + 0x02)                ///< �¶�������DAֵ
#define CORRECT_K                         (TEMPDAMAX     + 0x02)                ///< ��������ϵ��K
#define CORRECT_B                         (CORRECT_K     + 0x02)                ///< ��������ϵ��B
#define TEMPER_K1                         (CORRECT_B     + 0x02)                ///< �¶�1����ϵ��K1
#define TEMPER_B1                         (TEMPER_K1     + 0x02)                ///< �¶�1����ϵ��B1
#define TEMPER_K2                         (TEMPER_B1     + 0x02)                ///< �¶�2����ϵ��K2
#define TEMPER_B2                         (TEMPER_K2     + 0x02)                ///< �¶�2����ϵ��B2
#define SYSTEMPARAM_PROGRAMED             (TEMPER_B2     + 0x02)                /** ���ڼ��Flash�궨������
                                                                                �Ƿ�д�����ʼֵ��ֵΪ
                                                                                0xAA��д��� */
#define SYSTEM_PARAM_BAK1                 (RUN_ADDR_BASE     + FLASH_PAGE_SIZE) ///< ϵͳ��������1��ʼ��ַ
#define SYSTEM_PARAM_BAK2                 (SYSTEM_PARAM_BAK1 + FLASH_PAGE_SIZE) ///< ϵͳ��������2��ʼ��ַ     

#define ORGANIZATION                      (SYSTEM_PARAM_BAK2 + \
                                            SYSTEMPARAM_PROGRAMED + 1)          ///< ��֯��������
#define PRODUCTION                        (ORGANIZATION  + 31)                  ///< ��Ʒ����
#define HARDWAREVER                       (PRODUCTION    + 31)                  ///< Ӳ���汾
#define SOFTWAREVER                       (HARDWAREVER   + 31)                  ///< ����汾
#define DEVICENUM                         (SOFTWAREVER   + 31)                  ///< �豸ID
#define CUSTOMER                          (DEVICENUM     + 31)                  ///< �ͻ�����


/***************************** �豸����ռ�ô洢�ռ�ĳ��� ****************************/                                                                      
#define USER_DEFAULT_LEN                  (OUTPUTMODE    + 0x01)                ///< ���������ܳ���   
#define PRO_DEFAULT_LEN                   (SYSTEMPARAM_PROGRAMED + 0x01)        ///< ȫ�������ܳ���   


/********************************** �ڲ��洢��д���־ֵ *********************************/   
#define SYSTEMPARAM_IS_PROGRAMED          0xAA                                  ///< д���ʼֵ��־


/********************************** �ڲ��洢�������궨�� *********************************/   
#if defined(__IN_FLASH_H)
#define IN_MEMORY_WR_ENABLE             IN_FLASH_WR_ENABLE
#define IN_MEMORY_WR_DISABLE            IN_FLASH_WR_DISABLE
#define IN_MEMORY_ERR_MAX               WRITE_FLASH_ERR_MAX
#elif defined(__IN_EEPROM_H)
#define IN_MEMORY_WR_ENABLE             IN_EEPROM_WR_ENABLE
#define IN_MEMORY_WR_DISABLE            IN_EEPROM_WR_DISABLE
#define IN_MEMORY_ERR_MAX               WRITE_EEPROM_ERR_MAX
#endif // defined(__IN_FLASH_H) or defined(__IN_EEPROM_H)


/**@brief       ���ڲ��洢��ָ��λ��д1���ֽ�
* @param[in]    RWAddr : д��ʼ��ַ
* @param[in]    WrData : Ҫд�������;
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ������ͨ�������ڲ�Flash��EEPROM����APIʵ�ֵ�
*/
uint8_t InMemory_Write_OneByte(uint16_t RWAddr, uint8_t WrData);

/**@brief       ���ڲ��洢��ָ��λ�ö�1���ֽ�
* @param[in]    RWAddr : ����ʼ��ַ
* @return       ����ִ�н��
* - 1���ֽ�����
* @note         ������ͨ�������ڲ�Flash��EEPROM����APIʵ�ֵ�
*/
uint8_t InMemory_Read_OneByte(uint16_t RWAddr);

/**@brief       ���ڲ��洢��ָ��λ��д����ֽ�
* @param[in]    RWAddr : д��ʼ��ַ
* @param[in]    pWrbuf : Ҫд������ݻ���ָ��;
* @param[in]    Wrlen : ���ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ������ͨ�������ڲ�Flash��EEPROM����APIʵ�ֵ�
*/
uint8_t InMemory_Write_MultiBytes(uint16_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen);

/**@brief       ���ڲ��洢��ָ��λ�ö�����ֽ�
* @param[in]    RWAddr : ����ʼ��ַ
* @param[in]    pWrbuf : Ҫ��ȡ�����ݻ���ָ��;
* @param[in]    Wrlen : ���ݳ���
* @return       ����ִ�н��
* - None
* @note         ������ͨ�������ڲ�Flash��EEPROM����APIʵ�ֵ�
*/
void InMemory_Read_MultiBytes(uint16_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen);

/**@brief       ��STM32F072xx�ڲ�Flashָ��λ��д����ֽ��ұ���3��
* @param[in]    FlashAddr : д��ʼ��ַ
* @param[in]    pWrbuf : Ҫд������ݻ���ָ��;
* @param[in]    Wrlen : ���ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t InMemory_Write3T_MultiBytes(uint16_t FlashAddr, const uint8_t *pWrbuf, uint16_t Wrlen);

/**@brief       �ڲ�Flashϵͳ�������,�����ֲ�һ�µĲ���,���½�User_Default_Param
* д���ڲ�Flash
* @param[in]    Cur_Param : Ҫ���Ĳ�������;
* @param[in]    Num_Of_Cur_Param : ��������ĳ���;
* @return       ����ִ�н��
* - None
*/
void InMemory_SystemParam_Check(uint8_t *Cur_Param, uint16_t Num_Of_Cur_Param);

/**@brief       ��ȡ����鱣�����ڲ�Flash���豸����
* @return       ����ִ�н��
* - None
*/
void Check_Device_Param(void);

#ifdef __cplusplus
}
#endif
#endif // __IN_MEMORY_APP_H
