/**@file        In_EEPROM.c
* @brief        读写内部EEPROM驱动程序
* @details      向内部EEPROM读取或写入数据
* @author       杨春林
* @date         2020-04-29
* @version      V1.0.0
* @copyright    2020-2030,深圳市信为科技发展有限公司
**********************************************************************************
* @par 修改日志:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/04/29  <td>1.0.0    <td>杨春林    <td>创建初始版本
* </table>
*
**********************************************************************************
*/

#include "In_EEPROM.h"



/**@brief       向内部EEPROM指定位置写入一字节数据
* @param[in]    RWAddr : 写入地址
* @param[in]    WrData : 写入数据
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
* @note         地址不能越界,最大值为 EEPROM_END
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

/**@brief       向内部EEPROM指定位置读取一字节数据
* @param[in]    RWAddr : 读取地址
* @return       函数执行结果
* - 1个字节数据
* @note         地址不能越界,最大值为 EEPROM_END
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

/**@brief       向内部EEPROM指定位置写多个字节
* @param[in]    RWAddr : 写起始地址
* @param[in]    pWrbuf : 数据缓存指针
* @param[in]    Wrlen : 写数据长度
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
* @note         地址不能越界,最大值为 EEPROM_END
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

/**@brief       从内部EEPROM指定位置读多个字节
* @param[in]    RWAddr : 读起始地址
* @param[in]    pRdbuf : 数据缓存指针
* @param[in]    Rdlen : 读数据长度
* @return       函数执行结果
* - None
* @note         地址不能越界,最大值为 EEPROM_END
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

/**@brief       向内部EEPROM指定位置擦除4个字节
* @param[in]    RWAddr : 擦除起始地址
* @param[in]    WordNb : 擦除Word数目(4字节数目)
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
* @note         地址不能越界,最大值为 EEPROM_END，RWAddr地址必须4字节对齐
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
