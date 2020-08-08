/**@file        In_EEPROM.h
* @details      In_EEPROM.c��ͷ�ļ�,�����˶�д�ڲ�EEPROM���������API����,
* �����˲��� EEPROM �Ļ���ַ�Ϳɲ����ĵ�ַ��Χ
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

#ifndef __IN_EEPROM_H
#define __IN_EEPROM_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "VariaType.h"

#define IN_EEPROM_BASE_ADDRESS      DATA_EEPROM_BASE                   ///< ����EEPROM����ַ

#define IN_EEPROM_START             0x0000                              ///< EEPROM��ʼ��ַ
#define IN_EEPROM_END               (DATA_EEPROM_BANK2_END - IN_EEPROM_BASE_ADDRESS)    ///< EEPROM������ַ

#define IN_EEPROM_WR_ENABLE         0x0F                                ///< EEPROMд����ʹ��
#define IN_EEPROM_WR_DISABLE        0x00                                ///< EEPROMд������ֹ

#define WRITE_EEPROM_ERR_MAX        0x03                                ///< дEEPROM����������

/**@brief       ���ڲ�EEPROMָ��λ��д��һ�ֽ�����
* @param[in]    RWAddr : д���ַ
* @param[in]    WrData : д������
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ EEPROM_END
*/
uint8_t InEEPROM_Write_OneByte(uint16_t RWAddr, uint8_t WrData);

/**@brief       ���ڲ�EEPROMָ��λ�ö�ȡһ�ֽ�����
* @param[in]    RWAddr : ��ȡ��ַ
* @return       ����ִ�н��
* - 1���ֽ�����
* @note         ��ַ����Խ��,���ֵΪ EEPROM_END
*/
uint8_t InEEPROM_Read_OneByte(uint16_t RWAddr);

/**@brief       ���ڲ�EEPROMָ��λ��д����ֽ�
* @param[in]    RWAddr : д��ʼ��ַ
* @param[in]    pWrbuf : ���ݻ���ָ��
* @param[in]    Wrlen : д���ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ EEPROM_END
*/
uint8_t InEEPROM_Write_MultiBytes(uint16_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen);

/**@brief       ���ڲ�EEPROMָ��λ�ö�����ֽ�
* @param[in]    RWAddr : ����ʼ��ַ
* @param[in]    pRdbuf : ���ݻ���ָ��
* @param[in]    Rdlen : �����ݳ���
* @return       ����ִ�н��
* - None
* @note         ��ַ����Խ��,���ֵΪ EEPROM_END
*/
void InEEPROM_Read_MultiBytes(uint16_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen);

/**@brief       ���ڲ�EEPROMָ��λ�ò���4���ֽ�
* @param[in]    RWAddr : ������ʼ��ַ
* @param[in]    WordNb : ����Word��Ŀ(4�ֽ���Ŀ)
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ EEPROM_END��RWAddr��ַ����4�ֽڶ���
*/
uint8_t InEEPROM_Erase_MultiWord(uint16_t RWAddr, uint16_t WordNb);
  
#ifdef __cplusplus
}
#endif
#endif

