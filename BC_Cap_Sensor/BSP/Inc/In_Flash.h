/**@file        In_Flash.h
* @details      In_Flash.c��ͷ�ļ�,�����˶�д�ڲ�Flash���������API����,
* �����˲��� Flash �Ļ���ַ�Ϳɲ����ĵ�ַ��Χ
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

#ifndef __IN_FLASH_H
#define __IN_FLASH_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "VariaType.h"

/* ʹ��soway��λ����������(Boot����), BOOT_PROGRAM��main.h�ж��� */
#ifdef BOOT_PROGRAM
#define IN_FLASH_BASE_ADDRESS           0x0801E000                          ///< ����Flash����ַ
#else
#define IN_FLASH_BASE_ADDRESS           0x0801E800                          ///< ����Flash����ַ
#endif

#define IN_FLASH_START                  0x0000                              ///< Flash��ʼ��ַ
/* ʹ��soway��λ����������(Boot����), BOOT_PROGRAM��main.h�ж��� */
#ifdef BOOT_PROGRAM
#define IN_FLASH_END                    0x07FF                              ///< Flash������ַ
#else
#define IN_FLASH_END                    0x17FF                              ///< Flash������ַ
#endif

#define IN_FLASH_WR_ENABLE              0x0F                                ///< Flashд����ʹ��
#define IN_FLASH_WR_DISABLE             0x00                                ///< Flashд������ֹ

#define WRITE_FLASH_ERR_MAX             0x03                                ///< дFlash����������

/**@brief       ���ڲ�Flashָ��λ��д��һ�ֽ�����
* @param[in]    RWAddr : д���ַ
* @param[in]    WrData : д������
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
*/
uint8_t InFlash_Write_OneByte(uint16_t RWAddr, uint8_t WrData);

/**@brief       ���ڲ�Flashָ��λ�ö�ȡһ�ֽ�����
* @param[in]    RWAddr : ��ȡ��ַ
* @return       ����ִ�н��
* - 1���ֽ�����
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
*/
uint8_t InFlash_Read_OneByte(uint16_t RWAddr);

/**@brief       ���ڲ�Flashָ��λ��д����ֽ�
* @param[in]    RWAddr : д��ʼ��ַ
* @param[in]    pWrbuf : ���ݻ���ָ��
* @param[in]    Wrlen : д���ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
*/
uint8_t InFlash_Write_MultiBytes(uint16_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen);

/**@brief       ���ڲ�Flashָ��λ�ö�����ֽ�
* @param[in]    RWAddr : ����ʼ��ַ
* @param[in]    pRdbuf : ���ݻ���ָ��
* @param[in]    Rdlen : �����ݳ���
* @return       ����ִ�н��
* - None
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
*/
void InFlash_Read_MultiBytes(uint16_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen);

/**@brief       ���ڲ�Flashָ��λ�ò���ҳ
* @param[in]    RWAddr : ������ʼ��ַ
* @param[in]    PageNb : ����ҳ��Ŀ
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
*/
uint8_t InFlash_Erase_Page(uint16_t RWAddr, uint8_t PageNb);
  
#ifdef __cplusplus
}
#endif
#endif

