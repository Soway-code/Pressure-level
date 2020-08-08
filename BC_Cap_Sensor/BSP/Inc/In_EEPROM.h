/**@file        In_EEPROM.h
* @details      In_EEPROM.c的头文件,声明了读写内部EEPROM驱动程序的API函数,
* 定义了操作 EEPROM 的基地址和可操作的地址范围
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

#ifndef __IN_EEPROM_H
#define __IN_EEPROM_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "VariaType.h"

#define IN_EEPROM_BASE_ADDRESS      DATA_EEPROM_BASE                   ///< 操作EEPROM基地址

#define IN_EEPROM_START             0x0000                              ///< EEPROM起始地址
#define IN_EEPROM_END               (DATA_EEPROM_BANK2_END - IN_EEPROM_BASE_ADDRESS)    ///< EEPROM结束地址

#define IN_EEPROM_WR_ENABLE         0x0F                                ///< EEPROM写操作使能
#define IN_EEPROM_WR_DISABLE        0x00                                ///< EEPROM写操作禁止

#define WRITE_EEPROM_ERR_MAX        0x03                                ///< 写EEPROM错误最大次数

/**@brief       向内部EEPROM指定位置写入一字节数据
* @param[in]    RWAddr : 写入地址
* @param[in]    WrData : 写入数据
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
* @note         地址不能越界,最大值为 EEPROM_END
*/
uint8_t InEEPROM_Write_OneByte(uint16_t RWAddr, uint8_t WrData);

/**@brief       向内部EEPROM指定位置读取一字节数据
* @param[in]    RWAddr : 读取地址
* @return       函数执行结果
* - 1个字节数据
* @note         地址不能越界,最大值为 EEPROM_END
*/
uint8_t InEEPROM_Read_OneByte(uint16_t RWAddr);

/**@brief       向内部EEPROM指定位置写多个字节
* @param[in]    RWAddr : 写起始地址
* @param[in]    pWrbuf : 数据缓存指针
* @param[in]    Wrlen : 写数据长度
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
* @note         地址不能越界,最大值为 EEPROM_END
*/
uint8_t InEEPROM_Write_MultiBytes(uint16_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen);

/**@brief       从内部EEPROM指定位置读多个字节
* @param[in]    RWAddr : 读起始地址
* @param[in]    pRdbuf : 数据缓存指针
* @param[in]    Rdlen : 读数据长度
* @return       函数执行结果
* - None
* @note         地址不能越界,最大值为 EEPROM_END
*/
void InEEPROM_Read_MultiBytes(uint16_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen);

/**@brief       向内部EEPROM指定位置擦除4个字节
* @param[in]    RWAddr : 擦除起始地址
* @param[in]    WordNb : 擦除Word数目(4字节数目)
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
* @note         地址不能越界,最大值为 EEPROM_END，RWAddr地址必须4字节对齐
*/
uint8_t InEEPROM_Erase_MultiWord(uint16_t RWAddr, uint16_t WordNb);
  
#ifdef __cplusplus
}
#endif
#endif

