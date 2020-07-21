/**@file        modbus_ascii.c
* @brief        ModBus ASCII���ݴ���
* @details      ModBus ASCII������ModBus RTU���ݻ�ת
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

#include "ModBus_ASCII.h"


#if defined(USING_MODBUS_ASCII)
static uint8_t Send_Buf[USART_ASCII_MIN_DATALEN];   ///< ���ڷ������ݻ���


/**@brief       ʮ��������תASCII��
* @param[in]    cyHexData : ʮ��������
* @param[out]   pCyAsciiBuf : ASCII��
* @return       ����ִ�н��
* - None
*/
void MODBUS_ASCII_HexToAscii(uint8_t cyHexData, uint8_t *pCyAsciiBuf)
{
	uint8_t cyTemp;
	
	cyTemp = cyHexData / 16;
	if (10 > cyTemp) //0-9
	{
		*(pCyAsciiBuf + 0) = cyTemp + '0';
	}
	else
	{
		*(pCyAsciiBuf + 0) = (cyTemp - 10) + 'A';
	}

	cyTemp = cyHexData % 16;
	if (10 > cyTemp) //0-9
	{
		*(pCyAsciiBuf + 1) = cyTemp + '0';
	}
	else
	{
		*(pCyAsciiBuf + 1) = (cyTemp - 10) + 'A';
	}
}

/**@brief       ASCII��תʮ��������
* @param[in]    pCyAsciiBuf : ASCII��
* @return       ����ִ�н��
* - ʮ��������
*/
uint8_t MODBUS_ASCII_AsciiToHex(uint8_t *pCyAsciiBuf)
{
	uint8_t cyHexData;
	
	cyHexData = 0;
	if ('A' > *(pCyAsciiBuf + 0) ) //0-9
	{
		cyHexData += *(pCyAsciiBuf + 0) - '0';
	}
	else if ('a' > *(pCyAsciiBuf + 0) ) //��д
	{
		cyHexData += *(pCyAsciiBuf + 0) - 'A' + 10;
	}
	else
	{
		cyHexData += *(pCyAsciiBuf + 0) - 'a' + 10;
	}

	cyHexData *= 16;
	
	if ('A' > *(pCyAsciiBuf + 1) ) //0-9
	{
		cyHexData += *(pCyAsciiBuf + 1) - '0';
	}
	else if ('a' > *(pCyAsciiBuf + 1) ) //��д
	{
		cyHexData += *(pCyAsciiBuf + 1) - 'A' + 10;
	}
	else
	{
		cyHexData += *(pCyAsciiBuf + 1) - 'a' + 10;
	}
	
	return (cyHexData);
}

/**@brief       ��ȡLRCֵ
* @param[in]    RtuBuf : RTU���ݻ���ָ��
* @param[in]    cyLen : ���ݳ���
* @return       ����ִ�н��
* - LRCֵ
*/
uint8_t MODBUS_GetLrc(uint8_t *pCyRtuBuf, uint16_t cyLen)
{
	int8_t cyLrcVal;
    uint16_t i;
	
	cyLrcVal = 0;
	for (i = 0; i < cyLen; i++)
	{
		cyLrcVal += pCyRtuBuf[i];
	}
	//����
	cyLrcVal = -cyLrcVal;
	
	return (uint8_t)cyLrcVal;
}

/**@brief       ASCII���ݰ�ת��RTU���ݰ�
* @param[in]    pCyAsciiBuf : ASCII���ݰ�ָ��
* @param[in]    cyAsciiLen : ASCII���ݰ�����
* @param[out]   pCyRtuBuf : RTU���ݰ�ָ��
* @return       ����ִ�н��
* - ���� : 0
* - �ɹ� : RTU���ݰ�����
*/
uint16_t MODBUS_ASCII_AsciiPacketToRtuPacket(uint8_t *pCyAsciiBuf, uint16_t cyAsciiLen, uint8_t *pCyRtuBuf)
{
	uint16_t i;
	uint16_t cyRtuLen;
	
	if (1 == (cyAsciiLen % 2) )
	{
		return 0;
	}
	
	cyRtuLen = cyAsciiLen / 2;
	for (i = 0; i < cyRtuLen; i++)
	{
		*(pCyRtuBuf + i) = MODBUS_ASCII_AsciiToHex(pCyAsciiBuf + i * 2);
	}
	
	return (cyRtuLen);
}

/**@brief       RTU���ݰ�ת��ASCII���ݰ�
* @param[in]    pCyRtuBuf : RTU���ݰ�ָ��
* @param[in]    cyRtuLen : RTU���ݰ�����
* @param[out]   pCyAsciiBuf : ASCII���ݰ�ָ��
* @return       ����ִ�н��
* - ASCII���ݰ�����
*/
uint16_t MODBUS_ASCII_RtuPacketToAsciiPacket(uint8_t *pCyRtuBuf, uint16_t cyRtuLen, uint8_t *pCyAsciiBuf)
{
	uint16_t i;
	uint16_t cyAsciiLen;
	
	cyAsciiLen = cyRtuLen * 2;
	for (i = 0; i < cyRtuLen; i++)
	{
		MODBUS_ASCII_HexToAscii( *(pCyRtuBuf + i), pCyAsciiBuf + i * 2);
	}
	
	return (cyAsciiLen);
}

/**@brief       �����Ƿ���Ascii��
* @param[in]    pCyAsciiBuf : ASCII���ݰ�ָ��
* @param[in]    cyLen : ASCII���ݰ�����
* @return       ����ִ�н��
* - ERR_NONE : �޴���
* - ERR_ASCII : ��ȫ��ASCII��
*/
uint8_t MODBUS_ASCII_CheckAscii(uint8_t *pCyAsciiBuf, uint16_t cyLen)
{
	uint16_t i;
	
	for (i = 0; i < cyLen; i++)
	{
		if ('0' > *(pCyAsciiBuf + i) )
		{
			break;
		}
		
		if ( ('9' < *(pCyAsciiBuf + i) ) && ( *(pCyAsciiBuf + i) < 'A' ) )
		{
			break;
		}
		
		if ( ('F' < *(pCyAsciiBuf + i) ) && ( *(pCyAsciiBuf + i) < 'a' ) )
		{
			break;
		}
		
		if ('f' < *(pCyAsciiBuf + i) )
		{
			break;
		}
	}
	
	
	if (i == cyLen)
	{
		return ERR_NONE;
	}
	
	return ERR_ASCII;
}

/**@brief       ��ȡһ�� ModBus ASCII ����
* @param[in]    cyRecvBuff : ���ջ���ָ��
* @param[in]    cyLen : ���յ����ݳ���
* @return       ����ִ�н��
* - ERR_INVALID_DATA : ���ջ���ָ����Ч
* - ERR_ASCII : ��ȫ��ASCII��
* - ERR_LRC : LRCУ�����
* - ERR_NONE : �޴���
*/
uint8_t MODBUS_ASCII_RecvData(uint8_t* cyRecvBuff, uint16_t *pCyLen)
{
    uint8_t cyLrc;
    uint16_t recv_len;
	
    if (((uint8_t*)0) == cyRecvBuff)
    {
        return ERR_INVALID_DATA;
    }
    recv_len = Sensor_USART_GetReceData(cyRecvBuff);
    
    if((recv_len - 2) <= 0
        || cyRecvBuff[0] != ASCII_HEAD_DATA
        || cyRecvBuff[recv_len - 2] != ASCII_TAIL_FIRST_DATA)
    {
        return ERR_INVALID_DATA;
    }
    
    if (ERR_ASCII == MODBUS_ASCII_CheckAscii(&cyRecvBuff[1], recv_len - 3) )
    {
    	return ERR_ASCII;        
    }
    
    *pCyLen = MODBUS_ASCII_AsciiPacketToRtuPacket(&cyRecvBuff[1], 
            recv_len - 3, cyRecvBuff);
    
    cyLrc = MODBUS_GetLrc(cyRecvBuff, *pCyLen - 1);
    if (cyLrc != cyRecvBuff[*pCyLen - 1])
    {
    	return ERR_LRC;
    }        
    *pCyLen -= 1;
	
    return ERR_NONE;
}

/**@brief       ����һ�� ModBus ASCII ����
* @param[in]    cySendBuff : ���ͻ���ָ��
* @param[in]    cyLen : ���͵����ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t MODBUS_ASCII_SendData(ModBusBaseParam_TypeDef *ModBusBaseParam, uint8_t *cySendBuff, uint16_t cyLen, uint8_t Check_Addr)
{
    uint8_t cyLrc;
    uint16_t cyAsciiLen;
	
    if ( (0 == cyLen) || ( ((uint8_t*)0) == cySendBuff) )
    {
        return OP_FAILED;
    }
    
        //�㲥��ַ
    if((BROADCASTADDR == cySendBuff[0]) && Check_Addr)
    {
        return OP_FAILED;
    }
    
    if ( (cyLen * 2 + 5) > USART_ASCII_MAX_DATALEN)
    {
    	return OP_FAILED;
    }
    
    Send_Buf[0] = ASCII_HEAD_DATA;
    cyAsciiLen = 1;
    
    cyLrc = MODBUS_GetLrc(cySendBuff, cyLen);
    cyAsciiLen += MODBUS_ASCII_RtuPacketToAsciiPacket(cySendBuff, cyLen, &Send_Buf[1]);    
    MODBUS_ASCII_HexToAscii(cyLrc, &Send_Buf[cyAsciiLen]);
    cyAsciiLen += 2;
    Send_Buf[cyAsciiLen] = ASCII_TAIL_FIRST_DATA;
    cyAsciiLen++;
    Send_Buf[cyAsciiLen] = ASCII_TAIL_SECOND_DATA;
    cyAsciiLen++;    
    
    return Send_Data(ModBusBaseParam, Send_Buf, cyAsciiLen);
}
#endif
