#ifndef __IN_FLASH_H
#define __IN_FLASH_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "VariaType.h"

#define FLASH_BASE_ADDRESS              0x0801E800                          //操作Flash基地址

#define FLASH_START                     0x0000                              //Flash起始地址
#define FLASH_END                       0x17FF                              //Flash结束地址

#define FLASH_WR_ENABLE                 0x0F                                //Flash写操作使能
#define FLASH_WR_DISABLE                0x00                                //Flash写操作禁止

#define WRITE_FLASH_ERR_MAX             0x03                                //写Flash错误最大次数

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
uint8_t InFlash_Write_OneByte(uint16_t RWAddr, uint8_t WrData);

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
uint8_t InFlash_Read_OneByte(uint16_t RWAddr);

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
uint8_t InFlash_Write_MultiBytes(uint16_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen);

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
void InFlash_Read_MultiBytes(uint16_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen);

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
uint8_t InFlash_Erase_Page(uint16_t RWAddr, uint8_t PageNb);
  
#ifdef __cplusplus
}
#endif
#endif

