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
#endif

#else

#define USING_MODBUS_RTU            ///< 使用ModBus RTU协议, 还需要在 usart_app.h 里定义USING_UART_TIMEOUT
//#define USING_MODBUS_ASCII          ///< 使用ModBus ASCII协议, 还需要在 usart_app.h 里定义USING_CHARMATCH
//#define SUBCODE_IS_DEVADDR          ///< 定义ModBus子码为设备地址，不定义则默认为0

#endif

#define SEND_SIZE                       128              ///< 发送缓存大小

#define DAC_VALUE_MAX   4095        ///< DAC最大值

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
#endif
