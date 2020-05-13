#ifndef __dac_H
#define __dac_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

//#define DAC_CHANNEL_1_ENABLE
#define DAC_CHANNEL_2_ENABLE

typedef enum {
    DA_CHANNEL_1,
    DA_CHANNEL_2,
    DA_CHANNEL_TOTAL
}DA_CHANNEL_NUM;


/**
* 名称       : BSP_DAC_Init()
* 创建日期   : 2020-04-27
* 作者       : 杨春林
* 功能       : DAC初始化
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : DAC通道通过DAC_CHANNEL_4_ENABLE和DAC_CHANNEL_5_ENABLE宏定义打开
* 修改内容   :
*/
void BSP_DAC_Init(void);

/**
* 名称       : BSP_DAC_Convert()
* 创建日期   : 2020-04-27
* 作者       : 杨春林
* 功能       : 指定DAC通道写入DA值
* 输入参数   : DA_Value(DA值),DA_Channel_Num(DA通道号)
* 输出参数   : 无
* 返回结果   : OP_SUCCESS(成功),OP_FAILED(失败)
* 注意和说明 : 输出值不能超过4095,通道号只有两种(DA_CHANNEL_1和DA_CHANNEL_2)
* 修改内容   :
*/
uint8_t BSP_DAC_Convert(uint16_t DA_Value, DA_CHANNEL_NUM DA_Channel_Num);

#ifdef __cplusplus
}
#endif
#endif /*__ dac_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
