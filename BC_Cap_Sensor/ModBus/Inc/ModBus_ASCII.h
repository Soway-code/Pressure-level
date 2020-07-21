/**@file        modbus_ascii.h
* @details      modbus_ascii.c��ͷ�ļ�,������ModBus ASCII���ݴ����API����
* @author       ׯ��Ⱥ
* @date         2020-07-20
* @version      V2.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2020/07/20  <td>2.0.0    <td>ׯ��Ⱥ  <td>���      <td>����ӵĳ������(ASCIIЭ�鲿��)
* </table>
*
**********************************************************************************
*/

#ifndef _MODBUS_ASCII_H
#define _MODBUS_ASCII_H
#ifdef __cplusplus
 extern "C" {
#endif
     
#include "ModBus.h"


#define CMD_STRING_SIZE       128
#define USART_ASCII_MAX_DATALEN     600
#define USART_NOASC_MAX_DATALEN     300
#define USART_NOMAL_MAX_DATALEN     100
#define USART_ASCII_MIN_DATALEN     80

#define ASCII_HEAD_STATE  0     //ͷ
#define ASCII_END_STATE   1     //����
#define ASCII_IDLE_STATE  2     //����

#define ASCII_HEAD_DATA         ':'         //��ͷ��һ���ֽ�
#define ASCII_TAIL_FIRST_DATA   '\r'        //��β��һ���ֽ�
#define ASCII_TAIL_SECOND_DATA  '\n'        //��β�ڶ����ֽ�

/**@brief       ʮ��������תASCII��
* @param[in]    cyHexData : ʮ��������
* @param[out]   pCyAsciiBuf : ASCII��
* @return       ����ִ�н��
* - None
*/
void MODBUS_ASCII_HexToAscii(uint8_t cyHexData, uint8_t *pCyAsciiBuf);

/**@brief       ASCII��תʮ��������
* @param[in]    pCyAsciiBuf : ASCII��
* @return       ����ִ�н��
* - ʮ��������
*/
uint8_t MODBUS_ASCII_AsciiToHex(uint8_t *pCyAsciiBuf);

/**@brief       ��ȡLRCֵ
* @param[in]    RtuBuf : RTU���ݻ���ָ��
* @param[in]    cyLen : ���ݳ���
* @return       ����ִ�н��
* - LRCֵ
*/
uint8_t MODBUS_GetLrc(uint8_t *pCyRtuBuf, uint16_t cyLen);

/**@brief       ASCII���ݰ�ת��RTU���ݰ�
* @param[in]    pCyAsciiBuf : ASCII���ݰ�ָ��
* @param[in]    cyAsciiLen : ASCII���ݰ�����
* @param[out]   pCyRtuBuf : RTU���ݰ�ָ��
* @return       ����ִ�н��
* - ���� : 0
* - �ɹ� : RTU���ݰ�����
*/
uint16_t MODBUS_ASCII_AsciiPacketToRtuPacket(uint8_t *pCyAsciiBuf, uint16_t cyAsciiLen, uint8_t *pCyRtuBuf);

/**@brief       RTU���ݰ�ת��ASCII���ݰ�
* @param[in]    pCyRtuBuf : RTU���ݰ�ָ��
* @param[in]    cyRtuLen : RTU���ݰ�����
* @param[out]   pCyAsciiBuf : ASCII���ݰ�ָ��
* @return       ����ִ�н��
* - ASCII���ݰ�����
*/
uint16_t MODBUS_ASCII_RtuPacketToAsciiPacket(uint8_t *pCyRtuBuf, uint16_t cyRtuLen, uint8_t *pCyAsciiBuf);

/**@brief       �����Ƿ���Ascii��
* @param[in]    pCyAsciiBuf : ASCII���ݰ�ָ��
* @param[in]    cyLen : ASCII���ݰ�����
* @return       ����ִ�н��
* - ERR_NONE : �޴���
* - ERR_ASCII : ��ȫ��ASCII��
*/
uint8_t MODBUS_ASCII_CheckAscii(uint8_t *pCyAsciiBuf, uint16_t cyLen);

/**@brief       ��ȡһ�� ModBus ASCII ����
* @param[in]    cyRecvBuff : ���ջ���ָ��
* @param[in]    cyLen : ���յ����ݳ���
* @return       ����ִ�н��
* - ERR_INVALID_DATA : ���ջ���ָ����Ч
* - ERR_ASCII : ��ȫ��ASCII��
* - ERR_LRC : LRCУ�����
* - ERR_NONE : �޴���
*/
uint8_t MODBUS_ASCII_RecvData(uint8_t* cyRecvBuff, uint16_t *pCyLen);

/**@brief       ����һ�� ModBus ASCII ����
* @param[in]    cySendBuff : ���ͻ���ָ��
* @param[in]    cyLen : ���͵����ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t MODBUS_ASCII_SendData(ModBusBaseParam_TypeDef *ModBusBaseParam, uint8_t *cySendBuff, uint16_t cyLen, uint8_t Check_Addr);

#ifdef __cplusplus
}
#endif
#endif

