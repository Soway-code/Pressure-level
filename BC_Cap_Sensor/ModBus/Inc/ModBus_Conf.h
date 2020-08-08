/**@file        Modbus_Conf.h
* @details      Modbus_Conf.c的头文件,声明了电容传感器标定的API函数,定义了
* 设备参数结构体
* @author       庄明群
* @date         2020-07-20
* @version      V2.0.0
* @copyright    2020-2030,深圳市信为科技发展有限公司
**********************************************************************************
* @par 修改日志:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2020/07/20  <td>2.0.0    <td>庄明群  <td>杨春林      <td>新添加的程序代码(指令执行部分)
* </table>
*
**********************************************************************************
*/

#ifndef __MODBUS_CONF_H
#define __MODBUS_CONF_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "Picocap_app.h"
#include "adc_app.h"


/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS
#include "rtconfig.h"

#if defined(APP_USING_MODBUS_RTU)
#define USING_MODBUS_RTU            ///< 使用ModBus RTU协议
#elif defined(APP_USING_MODBUS_ASCII)
#define USING_MODBUS_ASCII          ///< 使用ModBus ASCII协议
#endif // defined(APP_USING_MODBUS_RTU) or defined(APP_USING_MODBUS_ASCII)

#if defined(APP_SUBCODE_IS_DEVADDR)
#define SUBCODE_IS_DEVADDR          ///< 定义ModBus子码为设备地址，不定义则默认为0
#endif // defined(APP_SUBCODE_IS_DEVADDR)

#else

#define USING_MODBUS_RTU            ///< 使用ModBus RTU协议, 还需要在 usart_app.h 里定义USING_UART_TIMEOUT
//#define USING_MODBUS_ASCII          ///< 使用ModBus ASCII协议, 还需要在 usart_app.h 里定义USING_CHARMATCH
//#define SUBCODE_IS_DEVADDR          ///< 定义ModBus子码为设备地址，不定义则默认为0(见 DEFAULT_SUBCODE)

#endif // USING_RT_THREAD_OS

#define AUTOUPLOAD_CYCLE                1000            ///< 自动上传周期,单位 ms

#define SEND_SIZE                       128              ///< 发送缓存大小

#define DAC_VALUE_MAX   4095        ///< DAC最大值

#ifndef SUBCODE_IS_DEVADDR
#define DEFAULT_SUBCODE                 0
#endif  // SUBCODE_IS_DEVADDR

/* 使用soway上位机升级程序(Boot程序), BOOT_PROGRAM在main.h中定义 */
#ifdef BOOT_PROGRAM

#define RESPONSE_ERR_NONE   0     //响应成功
#define RESPONSE_REC_ERR    1     //接收错误
#define RESPONSE_LRC_ERR    2     //校验码错误

#define UPGRADED_DEVICEADDR     65

#define ERASE_FLAG          0x0C
#define ERASE_FLAG_NONE     0xFF

#define UPGRADE_FLAG        0x0C
#define UPGRADE_FLAG_NONE   0xFF

#endif // BOOT_PROGRAM

#if defined(STM32F0)
#define ADDR_DEVICEADDR     (IN_FLASH_END - 2)
#define ADDR_ERASEFLAG      (IN_FLASH_END - 1)
#define ADDR_UPGRADEFLAG    IN_FLASH_END
#elif defined(STM32L0)
#define ADDR_DEVICEADDR     (IN_EEPROM_END - 2)
#define ADDR_ERASEFLAG      (IN_EEPROM_END - 1)
#define ADDR_UPGRADEFLAG    IN_EEPROM_END
#endif // defined(STM32F0) or defined(STM32L0)

/** ModBus管理设备的参数结构,可根据不同的产品加入或删除成员 */
typedef struct {
    DataFilterParam *DataFilter;
    PCap_DataConvert_Param *PCap_DataConvert;
    PCap_DataConvert_Out_Param *PCap_DataConvert_Out; 
    ADC_TemperParam_TypeDef *ADC_TemperParam;
    ADC_TemperOut_TypeDef   *ADC_TemperOut;
}ModBus_Device_Param;


/**@brief       电容标定
* @param[in]    arg : 用户自定义的参数,这里为设备参数
* @return       函数执行结果
* - None
*/
void MB_Cap_Calibration(void *arg);

/**@brief       电容AD值标定
* @param[in]    arg : 用户自定义的参数,这里为设备参数
* @return       函数执行结果
* - None
*/
void MB_CapAD_Calibration(void *arg);

/**@brief       电容DA值标定
* @param[in]    arg : 用户自定义的参数,这里为设备参数
* @return       函数执行结果
* - None
*/
void MB_CapDAOut_Calibration(void *arg);

/**@brief       温度DA值标定功能
* @param[in]    arg : 用户自定义的参数,这里为设备参数
* @return       函数执行结果
* - None
*/
void MB_TempDAOut_Calibration(void *arg);

#ifdef __cplusplus
}
#endif
#endif // __MODBUS_CONF_H
