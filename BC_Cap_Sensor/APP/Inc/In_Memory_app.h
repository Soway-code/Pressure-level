/**@file        In_Memory_app.h
* @details      In_Memory_app.c的头文件,声明了内部存储器应用的API函数
* @author       杨春林
* @date         2020-07-20
* @version      V2.0.0
* @copyright    2020-2030,深圳市信为科技发展有限公司
**********************************************************************************
* @par 修改日志:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2020/07/20  <td>2.0.0    <td>庄明群  <td>杨春林      <td>创建初始版本
* </table>
*
**********************************************************************************
*/
#ifndef __IN_MEMORY_APP_H
#define __IN_MEMORY_APP_H

#include "main.h"

#if defined(STM32F0)
#include "In_Flash.h"
#elif defined(STM32L0)
#include "In_EEPROM.h"
#endif
#include "TypeConvert.h"

/***************************** 设备参数在内部存储器中的映射地址 ****************************/
#define RUN_ADDR_BASE                     0x00                                  ///< 系统参数初始化标志/内部Flash起始地址
#define DEVICE_ADDR                       (RUN_ADDR_BASE + 0x01)                ///< 设备地址
#define BAUDRATE                          (DEVICE_ADDR   + 0x01)                ///< 波特率
#define PARITY                            (BAUDRATE      + 0x01)                ///< 奇偶校验
#define FILTER                            (PARITY        + 0x01)                ///< 滤波系数
#define AUTO_UPLOAD                       (FILTER        + 0x01)                ///< 自动上传周期
#define COMPENSATE                        (AUTO_UPLOAD   + 0x01)                ///< 补偿使能
#define FREEZE                            (COMPENSATE    + 0x01)                ///< 是否冻结设备
#define OUTPUTMODE                        (FREEZE        + 0x01)                ///< 输出方式
#define HEIGHTRANGE                       (OUTPUTMODE    + 0x01)                ///< 量程
#define CAPMIN                            (HEIGHTRANGE   + 0x02)                ///< 零点电容
#define CAPMAX                            (CAPMIN        + 0x04)                ///< 满量程电容
#define CAPMINBAK                         (CAPMAX        + 0x04)                ///< 零点电容备份
#define CAPMAXBAK                         (CAPMINBAK     + 0x04)                ///< 满量程电容备份
#define CAPADMIN                          (CAPMAXBAK     + 0x04)                ///< 电容零点AD值
#define CAPADLOW                          (CAPADMIN      + 0x02)                ///< 电容下刻度AD值
#define CAPADHIGH                         (CAPADLOW      + 0x02)                ///< 电容上刻度AD值
#define CAPADMAX                          (CAPADHIGH     + 0x02)                ///< 电容满量程AD值
#define CAPDAMIN                          (CAPADMAX      + 0x02)                ///< 电容零点DA值
#define CAPDALOW                          (CAPDAMIN      + 0x02)                ///< 电容下刻度DA值
#define CAPDAHIGH                         (CAPDALOW      + 0x02)                ///< 电容上刻度DA值
#define CAPDAMAX                          (CAPDAHIGH     + 0x02)                ///< 电容满量程DA值
#define TEMPDAMIN                         (CAPDAMAX      + 0x02)                ///< 温度零点DA值
#define TEMPDAMAX                         (TEMPDAMIN     + 0x02)                ///< 温度满量程DA值
#define CORRECT_K                         (TEMPDAMAX     + 0x02)                ///< 电容修正系数K
#define CORRECT_B                         (CORRECT_K     + 0x02)                ///< 电容修正系数B
#define TEMPER_K1                         (CORRECT_B     + 0x02)                ///< 温度1修正系数K1
#define TEMPER_B1                         (TEMPER_K1     + 0x02)                ///< 温度1修正系数B1
#define TEMPER_K2                         (TEMPER_B1     + 0x02)                ///< 温度2修正系数K2
#define TEMPER_B2                         (TEMPER_K2     + 0x02)                ///< 温度2修正系数B2
#define SYSTEMPARAM_PROGRAMED             (TEMPER_B2     + 0x02)                /** 用于检查Flash标定参数区
                                                                                是否被写入过初始值，值为
                                                                                0xAA则写入过 */
#define SYSTEM_PARAM_BAK1                 (RUN_ADDR_BASE     + FLASH_PAGE_SIZE) ///< 系统参数备份1起始地址
#define SYSTEM_PARAM_BAK2                 (SYSTEM_PARAM_BAK1 + FLASH_PAGE_SIZE) ///< 系统参数备份2起始地址     

#define ORGANIZATION                       SYSTEM_PARAM_BAK2 + \
                                            SYSTEMPARAM_PROGRAMED + 1           ///< 组织机构代码
#define PRODUCTION                        (ORGANIZATION  + 31)                  ///< 产品代码
#define HARDWAREVER                       (PRODUCTION    + 31)                  ///< 硬件版本
#define SOFTWAREVER                       (HARDWAREVER   + 31)                  ///< 软件版本
#define DEVICENUM                         (SOFTWAREVER   + 31)                  ///< 设备ID
#define CUSTOMER                          (DEVICENUM     + 31)                  ///< 客户代码


/***************************** 设备参数占用存储空间的长度 ****************************/                                                                      
#define USER_DEFAULT_LEN                  (OUTPUTMODE    + 0x01)                ///< 基本参数总长度   
#define PRO_DEFAULT_LEN                   (SYSTEMPARAM_PROGRAMED + 0x01)        ///< 全部参数总长度   


/********************************** 内部存储器写入标志值 *********************************/   
#define SYSTEMPARAM_IS_PROGRAMED          0xAA                                  ///< 写入初始值标志


/********************************** 内部存储器操作宏定义 *********************************/   
#if defined(__IN_FLASH_H)
#define IN_MEMORY_WR_ENABLE             IN_FLASH_WR_ENABLE
#define IN_MEMORY_WR_DISABLE            IN_FLASH_WR_DISABLE
#define IN_MEMORY_ERR_MAX               WRITE_FLASH_ERR_MAX
#elif defined(__IN_EEPROM_H)
#define IN_MEMORY_WR_ENABLE             IN_EEPROM_WR_ENABLE
#define IN_MEMORY_WR_DISABLE            IN_EEPROM_WR_DISABLE
#define IN_MEMORY_ERR_MAX               WRITE_EEPROM_ERR_MAX
#endif


/**@brief       向内部存储器指定位置写1个字节
* @param[in]    RWAddr : 写起始地址
* @param[in]    WrData : 要写入的数据;
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
* @note         本函数通过调用内部Flash或EEPROM驱动API实现的
*/
uint8_t InMemory_Write_OneByte(uint16_t RWAddr, uint8_t WrData);

/**@brief       向内部存储器指定位置读1个字节
* @param[in]    RWAddr : 读起始地址
* @return       函数执行结果
* - 1个字节数据
* @note         本函数通过调用内部Flash或EEPROM驱动API实现的
*/
uint8_t InMemory_Read_OneByte(uint16_t RWAddr);

/**@brief       向内部存储器指定位置写多个字节
* @param[in]    RWAddr : 写起始地址
* @param[in]    pWrbuf : 要写入的数据缓存指针;
* @param[in]    Wrlen : 数据长度
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
* @note         本函数通过调用内部Flash或EEPROM驱动API实现的
*/
uint8_t InMemory_Write_MultiBytes(uint16_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen);

/**@brief       向内部存储器指定位置读多个字节
* @param[in]    RWAddr : 读起始地址
* @param[in]    pWrbuf : 要读取的数据缓存指针;
* @param[in]    Wrlen : 数据长度
* @return       函数执行结果
* - None
* @note         本函数通过调用内部Flash或EEPROM驱动API实现的
*/
void InMemory_Read_MultiBytes(uint16_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen);

/**@brief       向STM32F072xx内部Flash指定位置写多个字节且备份3份
* @param[in]    FlashAddr : 写起始地址
* @param[in]    pWrbuf : 要写入的数据缓存指针;
* @param[in]    Wrlen : 数据长度
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
*/
uint8_t InMemory_Write3T_MultiBytes(uint16_t FlashAddr, const uint8_t *pWrbuf, uint16_t Wrlen);

/**@brief       内部Flash系统参数检查,若出现不一致的参数,重新将User_Default_Param
* 写入内部Flash
* @param[in]    Cur_Param : 要检查的参数数组;
* @param[in]    Num_Of_Cur_Param : 参数数组的长度;
* @return       函数执行结果
* - None
*/
void InMemory_SystemParam_Check(uint8_t *Cur_Param, uint16_t Num_Of_Cur_Param);

/**@brief       读取并检查保存在内部Flash的设备参数
* @return       函数执行结果
* - None
*/
void Check_Device_Param(void);

#endif
