/**@file        In_EEPROM.c
* @brief        ��д�ڲ�EEPROM��������
* @details      ���ڲ�EEPROM��ȡ��д������
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

#include "In_EEPROM.h"



/**@brief       ���ڲ�EEPROMָ��λ��д��һ�ֽ�����
* @param[in]    RWAddr : д���ַ
* @param[in]    WrData : д������
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ EEPROM_END
*/
uint8_t InEEPROM_Write_OneByte(uint16_t RWAddr, uint8_t WrData)
{
    uint32_t WrAddr;
    HAL_StatusTypeDef Halstatus;

    if(IN_EEPROM_END < RWAddr)
    {
        return OP_FAILED;
    }
    
    WrAddr = RWAddr + IN_EEPROM_BASE_ADDRESS;
    assert_param(IS_FLASH_DATA_ADDRESS(WrAddr));
    
    HAL_FLASHEx_DATAEEPROM_Unlock();
    //__disable_irq();
    Halstatus = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, WrAddr, WrData);
    HAL_FLASHEx_DATAEEPROM_Lock();
    //__enable_irq();
    if(Halstatus != HAL_OK)
    {
        return OP_FAILED;
    }

    return OP_SUCCESS;
}

/**@brief       ���ڲ�EEPROMָ��λ�ö�ȡһ�ֽ�����
* @param[in]    RWAddr : ��ȡ��ַ
* @return       ����ִ�н��
* - 1���ֽ�����
* @note         ��ַ����Խ��,���ֵΪ EEPROM_END
*/
uint8_t InEEPROM_Read_OneByte(uint16_t RWAddr)
{
  uint8_t RdData;
  uint32_t RealAddr;
  
  RdData = 0;
  RealAddr = IN_EEPROM_BASE_ADDRESS + RWAddr;
  assert_param(IS_EEPROM_PROGRAM_ADDRESS(RdAddr));
  RdData = *(__IO uint8_t *)RealAddr;
  return RdData;
}

/**@brief       ���ڲ�EEPROMָ��λ��д����ֽ�
* @param[in]    RWAddr : д��ʼ��ַ
* @param[in]    pWrbuf : ���ݻ���ָ��
* @param[in]    Wrlen : д���ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ EEPROM_END
*/
uint8_t InEEPROM_Write_MultiBytes(uint16_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen)
{
    uint16_t i;
    uint32_t WrAddr;
    HAL_StatusTypeDef Halstatus;

    if(IN_EEPROM_END < RWAddr)
    {
        return OP_FAILED;
    }
    
    WrAddr = RWAddr + IN_EEPROM_BASE_ADDRESS;    
    
    HAL_FLASHEx_DATAEEPROM_Unlock();
    //__disable_irq();
    for(i = 0; i < Wrlen; i++)
    {        
        assert_param(IS_FLASH_DATA_ADDRESS(WrAddr));
        Halstatus = HAL_FLASHEx_DATAEEPROM_Program( FLASH_TYPEPROGRAMDATA_BYTE, 
                                                    WrAddr, 
                                                    pWrbuf[i]);
        if(Halstatus != HAL_OK)
        {
            break;
        }
        WrAddr += 1;
    }
    HAL_FLASHEx_DATAEEPROM_Lock();
    //__enable_irq();
    if(Halstatus != HAL_OK)
    {
        return OP_FAILED;
    }

    return OP_SUCCESS;
}

/**@brief       ���ڲ�EEPROMָ��λ�ö�����ֽ�
* @param[in]    RWAddr : ����ʼ��ַ
* @param[in]    pRdbuf : ���ݻ���ָ��
* @param[in]    Rdlen : �����ݳ���
* @return       ����ִ�н��
* - None
* @note         ��ַ����Խ��,���ֵΪ EEPROM_END
*/
void InEEPROM_Read_MultiBytes(uint16_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen)
{
  uint16_t EEPROMCnt;
  uint16_t RWAddress;
  
  RWAddress = RWAddr;
  for(EEPROMCnt = 0; EEPROMCnt < Rdlen; EEPROMCnt++)
  {
    *(pRdbuf + EEPROMCnt) = InEEPROM_Read_OneByte(RWAddress);
    
    RWAddress++;
  }
}

/**@brief       ���ڲ�EEPROMָ��λ�ò���4���ֽ�
* @param[in]    RWAddr : ������ʼ��ַ
* @param[in]    WordNb : ����Word��Ŀ(4�ֽ���Ŀ)
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ EEPROM_END��RWAddr��ַ����4�ֽڶ���
*/
uint8_t InEEPROM_Erase_MultiWord(uint16_t RWAddr, uint16_t WordNb)
{
    uint16_t i;
    uint32_t EraseAddr;
    HAL_StatusTypeDef Halstatus = HAL_OK;

    if((IN_EEPROM_END < RWAddr))
    {
        return OP_FAILED;
    }
    
    EraseAddr = RWAddr + IN_EEPROM_BASE_ADDRESS;    
    HAL_FLASHEx_DATAEEPROM_Unlock();    

    for(i = 0; i < WordNb; i++)
    {           
        assert_param(IS_FLASH_DATA_ADDRESS(EraseAddr));
        Halstatus = HAL_FLASHEx_DATAEEPROM_Erase(EraseAddr);
        if(Halstatus != HAL_OK)
        {
            break;
        }
        EraseAddr += 4;
    }
    HAL_FLASHEx_DATAEEPROM_Lock();
    if(Halstatus != HAL_OK)
    {
        return OP_FAILED;
    }

    return OP_SUCCESS;
}
