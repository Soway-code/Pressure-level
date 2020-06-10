/****************************************************************************************************************************************************************
** 版权:          2020-2030,深圳市信为科技发展有限公司
** 文件名:        In_Flash.c
** 作者:          杨春林
** 版本:          V1.0.0
** 日期:          2020-04-29
** 描述:          从STM32F072的内部Flash中读取或写入数据
** 功能:          向内部Flash读取或写入数据
*****************************************************************************************************************************************************************
** 修改者:        No
** 版本:          No
** 修改内容:      No
** 日期:          No
****************************************************************************************************************************************************************/

#include "In_Flash.h"

//临时缓存
static uint32_t Flash_Buf[FLASH_PAGE_SIZE / 4];    

/**
* 名称       : InFlash_Write_OneByte()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 向内部Flash指定位置写入一字节数据
* 输入参数   : RWAddr（写入地址），WrData（写入数据）
* 输出参数   : 无
* 返回结果   : OP_SUCCESS（成功），OP_FAILED（失败）
* 注意和说明 : 地址不能越界
* 修改内容   :
*/
uint8_t InFlash_Write_OneByte(uint16_t RWAddr, uint8_t WrData)
{
    uint32_t WrAddr;
    uint16_t i;
    uint8_t *buf = (uint8_t *)Flash_Buf;
    HAL_StatusTypeDef Halstatus;

    if(FLASH_END < RWAddr)
    {
        return OP_FAILED;
    }
    WrAddr = (RWAddr / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;
    InFlash_Read_MultiBytes(WrAddr, buf, FLASH_PAGE_SIZE);
    buf[RWAddr % FLASH_PAGE_SIZE] = WrData;
    
    Halstatus = (HAL_StatusTypeDef)InFlash_Erase_Page(RWAddr, 1);
    if(HAL_OK != Halstatus)
        return OP_FAILED;
    //__disable_irq();    
    HAL_FLASH_Unlock();
    for(i = 0; i < FLASH_PAGE_SIZE; i += 4)
    {
        Halstatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, \
                                        (WrAddr + i) + FLASH_BASE_ADDRESS, (uint32_t)Flash_Buf[i / 4]);
        if(HAL_OK != Halstatus)
            break;
    }
    HAL_FLASH_Lock();
    //__enable_irq();
    if(HAL_OK != Halstatus)
    {
        return OP_FAILED;
    }

    return OP_SUCCESS;
}

/**
* 名称       : InFlash_Read_OneByte()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 向内部Flash指定位置读取一字节数据
* 输入参数   : RWAddr（读取地址）
* 输出参数   : 无
* 返回结果   : 1个字节数据
* 注意和说明 : 地址不能越界
* 修改内容   :
*/
uint8_t InFlash_Read_OneByte(uint16_t RWAddr)
{
  uint8_t RdData;
  uint32_t RealAddr;
  
  RdData = 0;
  RealAddr = FLASH_BASE_ADDRESS + RWAddr;
  assert_param(IS_FLASH_PROGRAM_ADDRESS(RdAddr));
  RdData = *(__IO uint8_t *)RealAddr;
  return RdData;
}

/**
* 名称       : InFlash_Write_MultiBytes()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 向内部Flash指定位置写多个字节
* 输入参数   : RWAddr(写起始地址)，pWrbuf(固定数据内存),Wrlen(写数据长度)
* 输出参数   : 无
* 返回结果   : OP_SUCCESS（成功），OP_FAILED（失败）
* 注意和说明 : 地址不能越界
* 修改内容   :
*/
uint8_t InFlash_Write_MultiBytes(uint16_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen)
{
    uint16_t WrAddr;
    uint16_t i;
    uint8_t j;
    uint16_t remain;
    uint16_t off;
    uint8_t *wbuf = (uint8_t *)pWrbuf;
    uint8_t *flash_buf = (uint8_t *)Flash_Buf;
    HAL_StatusTypeDef Halstatus;

    if(FLASH_END < RWAddr)
    {
        return OP_FAILED;
    }
    WrAddr = (RWAddr / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;
    off = RWAddr % FLASH_PAGE_SIZE;
    remain = FLASH_PAGE_SIZE - off;
    if(Wrlen <= remain)
        remain = Wrlen;
    for(j = 0; j < (Wrlen / FLASH_PAGE_SIZE) + 1; j++)
    {        
        InFlash_Read_MultiBytes(WrAddr, flash_buf, FLASH_PAGE_SIZE);
        for(i = 0; i < remain; i++)
        {
            flash_buf[i + off] = wbuf[i];
        }
        
        Halstatus = (HAL_StatusTypeDef)InFlash_Erase_Page(WrAddr, 1);
        if(HAL_OK != Halstatus)
            break;
        //__disable_irq();        
        HAL_FLASH_Unlock();                        
        for(i = 0; i < FLASH_PAGE_SIZE; i += 4)
        {
            Halstatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 
                                        (WrAddr + i) + FLASH_BASE_ADDRESS, (uint32_t)Flash_Buf[i / 4]);
            if(HAL_OK != Halstatus)
                break;
        }
        HAL_FLASH_Lock();
        //__enable_irq();
        WrAddr += FLASH_PAGE_SIZE;
        off = 0;
        wbuf += remain;
        Wrlen -= remain;
        if(Wrlen > FLASH_PAGE_SIZE)
            remain = FLASH_PAGE_SIZE;
        else
            remain = Wrlen;
    }
    if(HAL_OK != Halstatus)
    {
        return OP_FAILED;
    }

    return OP_SUCCESS;
}

/**
* 名称       : InFlash_Read_MultiBytes()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 从内部Flash指定位置读多个字节
* 输入参数   : RWAddr(读起始地址)，Rdlen(读数据长度)
* 输出参数   : pRdbuf(数据读内存)
* 返回结果   : 无
* 注意和说明 : 地址不能越界
* 修改内容   :
*/
void InFlash_Read_MultiBytes(uint16_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen)
{
  uint16_t FlashCnt;
  uint16_t RWAddress;
  
  RWAddress = RWAddr;
  for(FlashCnt = 0; FlashCnt < Rdlen; FlashCnt++)
  {
    *(pRdbuf + FlashCnt) = InFlash_Read_OneByte(RWAddress);
    
    RWAddress++;
  }
}

/**
* 名称       : InFlash_Erase_Page()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 向内部Flash指定位置擦除页
* 输入参数   : RWAddr(擦除起始地址)，PageNb(擦除页数目)
* 输出参数   : 无
* 返回结果   : OP_SUCCESS（成功），OP_FAILED（失败）
* 注意和说明 : 地址不能越界
* 修改内容   :
*/
uint8_t InFlash_Erase_Page(uint16_t RWAddr, uint8_t PageNb)
{
    uint8_t sta = OP_SUCCESS;
    uint32_t EepAddress;
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError;

    EepAddress = (RWAddr / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;
    EepAddress += FLASH_BASE_ADDRESS;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = EepAddress;
    EraseInitStruct.NbPages = PageNb;
    HAL_FLASH_Unlock();    
    sta = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
    HAL_FLASH_Lock();    

    return sta;
}
