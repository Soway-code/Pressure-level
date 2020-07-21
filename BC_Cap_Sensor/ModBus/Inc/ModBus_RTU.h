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
* <tr><td>2020/07/20  <td>2.0.0    <td>ׯ��Ⱥ  <td>���      <td>����ӵĳ������(RTUЭ�鲿��)
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


/**@brief       ���������Ϣ������CRC16У��
* @param[in]    Msg : ��Ϣ������;
* @param[in]    MsgLen : ��Ϣ������;
* @return       ����ִ�н��
* - CRC16У����
* @note         ��Ϣ���Ȳ���Ϊ0
*/
uint16_t MDB_Get_CRC16(uint8_t *Msg, uint16_t MsgLen);

/**@brief       ���� Modbus RTU ��Ϣ��
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    pMsg : ��Ϣ������; 
* @param[in]    len : ��Ϣ������;
* @param[in]    Check_Addr : �Ƿ����豸��ַ��Ч
* @return       ����ִ�н��
* - None
* @note         ��Ϣ���Ȳ���Ϊ0������ʱ�Ƚⶳ�豸
*/
void MODBUS_RTU_SendData(ModBusBaseParam_TypeDef *ModBusBaseParam, uint8_t *pMsg, uint16_t len, uint8_t Check_Addr);

/**@brief       ��ȡ Modbus RTU ��Ϣ��
* @param[in]    cyRecvBuff : ��Ϣ������; 
* @param[in]    pCyLen : �������Ϣ������;
* @return       ����ִ�н��
* - ERR_INVALID_DATA : ��Ч����
* - ERR_CRC : CRCУ�����
* - ERR_NONE : �޴���
*/
uint8_t MODBUS_RTU_RecvData(uint8_t* cyRecvBuff, uint16_t *pCyLen);

#ifdef __cplusplus
}
#endif
#endif

