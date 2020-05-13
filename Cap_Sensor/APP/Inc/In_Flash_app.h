/****************************************************************************************************************************************************************
** 版权:          2020-2030,深圳市信为科技发展有限公司
** 文件名:        In_Flash_app.c
** 作者:          杨春林
** 版本:          V1.0.0
** 日期:          2020-04-30
** 描述:          读写内部Flash的应用
** 功能:          从STM32F072的内部Flash中读取系统内部参数
*****************************************************************************************************************************************************************
** 修改者:        No
** 版本:          No
** 修改内容:      No
** 日期:          No
****************************************************************************************************************************************************************/

#ifndef __IN_FLASH_APP_H
#define __IN_FLASH_APP_H

#include "In_Flash.h"
#include "TypeConvert.h"


#define RUN_ADDR_BASE                     0x00                                  //系统参数初始化标志
#define DEVICE_ADDR                       (RUN_ADDR_BASE + 0x01)                //设备地址
#define BAUDRATE                          (DEVICE_ADDR   + 0x01)                //波特率
#define PARITY                            (BAUDRATE      + 0x01)                //奇偶校验
#define FILTER                            (PARITY        + 0x01)                //滤波系数
#define AUTO_UPLOAD                       (FILTER        + 0x01)                //自动上传周期
#define COMPENSATE                        (AUTO_UPLOAD   + 0x01)                //补偿使能
#define FREEZE                            (COMPENSATE    + 0x01)                //是否冻结设备
#define OUTPUTMODE                        (FREEZE        + 0x01)                //输出方式

#define USER_DEFAULT_LEN                  (OUTPUTMODE    + 0x01)                //基本参数总长度   

#define HEIGHTRANGE                       (OUTPUTMODE    + 0x01)                //量程
#define CAPMIN                            (HEIGHTRANGE   + 0x02)                //零点电容
#define CAPMAX                            (CAPMIN        + 0x04)                //满量程电容
#define CAPMINBAK                         (CAPMAX        + 0x04)                //零点电容备份
#define CAPMAXBAK                         (CAPMINBAK     + 0x04)                //满量程电容备份
#define CAPADMIN                          (CAPMAXBAK     + 0x04)                //电容零点AD值
#define CAPADLOW                          (CAPADMIN      + 0x02)                //电容下刻度AD值
#define CAPADHIGH                         (CAPADLOW      + 0x02)                //电容上刻度AD值
#define CAPADMAX                          (CAPADHIGH     + 0x02)                //电容满量程AD值
#define CAPDAMIN                          (CAPADMAX      + 0x02)                //电容零点DA值
#define CAPDALOW                          (CAPDAMIN      + 0x02)                //电容下刻度DA值
#define CAPDAHIGH                         (CAPDALOW      + 0x02)                //电容上刻度DA值
#define CAPDAMAX                          (CAPDAHIGH     + 0x02)                //电容满量程DA值
#define TEMPDAMIN                         (CAPDAMAX      + 0x02)                //温度零点DA值
#define TEMPDAMAX                         (TEMPDAMIN     + 0x02)                //温度满量程DA值
#define CORRECT_K                         (TEMPDAMAX     + 0x02)                //电容修正系数K
#define CORRECT_B                         (CORRECT_K     + 0x02)                //电容修正系数B
#define TEMPER_K1                         (CORRECT_B     + 0x02)                //温度1修正系数K1
#define TEMPER_B1                         (TEMPER_K1     + 0x02)                //温度1修正系数B1
#define TEMPER_K2                         (TEMPER_B1     + 0x02)                //温度2修正系数K2
#define TEMPER_B2                         (TEMPER_K2     + 0x02)                //温度2修正系数B2
#define SYSTEMPARAM_PROGRAMED             (TEMPER_B2     + 0x02)              /*用于检查Flash标定参数区
                                                                                是否被写入过初始值，值为
                                                                                0xAA则写入过*/
#define PRO_DEFAULT_LEN                   (SYSTEMPARAM_PROGRAMED + 0x01)        //默认参数总长度   

#define SYSTEMPARAM_IS_PROGRAMED          0xAA                                  //写入初始值标志

#define SYSTEM_PARAM_BAK1                 (RUN_ADDR_BASE     + FLASH_PAGE_SIZE)           //系统参数备份1
#define SYSTEM_PARAM_BAK2                 (SYSTEM_PARAM_BAK1 + FLASH_PAGE_SIZE)           //系统参数备份2

#define ORGANIZATION                       0x0400                               //组织机构代码
#define PRODUCTION                        (ORGANIZATION  + 0x30)                //产品代码
#define HARDWAREVER                       (PRODUCTION    + 0x30)                //硬件版本
#define SOFTWAREVER                       (HARDWAREVER   + 0x30)                //软件版本
#define DEVICENUM                         (SOFTWAREVER   + 0x20)                //设备ID
#define CUSTOMER                          (DEVICENUM     + 0x30)                //客户代码


/**
* 名称       : InFlash_Write3T_MultiBytes()
* 创建日期   : 2020-04-30
* 作者       : 杨春林
* 功能       : 向STM32F072xx内部Flash指定位置写多个字节且备份
* 输入参数   : FlashAddr:写起始地址; pWrbuf:要写入的数据缓存指针; Wrlen:数据长度
* 输出参数   : 无
* 返回结果   : OP_SUCCESS(写入成功)，OP_FAILED(写入失败)
* 注意和说明 :
* 修改内容   :
*/
uint8_t InFlash_Write3T_MultiBytes(uint16_t FlashAddr, const uint8_t *pWrbuf, uint16_t Wrlen);

/**
* 名称       : InFlash_SystemParam_Check()
* 创建日期   : 2020-04-30
* 作者       : 杨春林
* 功能       : 内部Flash系统参数检查,若出现不一致的参数,重新将User_Default_Param
*              写入内部Flash
* 输入参数   : Cur_Param,要检查的参数数组; Num_Of_Cur_Param,参数的大小;
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 :
* 修改内容   :
*/
void InFlash_SystemParam_Check(uint8_t *Cur_Param, uint16_t Num_Of_Cur_Param);

/**
* 名称       : Check_Device_Param()
* 创建日期   : 2020-04-30
* 作者       : 杨春林
* 功能       : 读取并检查保存在内部Flash的设备参数
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 :
* 修改内容   :
*/
void Check_Device_Param(void);

#endif
