#ifndef __ADC_BSP_H
#define __ADC_BSP_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */



/* USER CODE BEGIN Private defines */

//#define ADC_CHANNEL_0_ENABLE
//#define ADC_CHANNEL_1_ENABLE
#define ADC_CHANNEL_2_ENABLE
//#define ADC_CHANNEL_3_ENABLE
//#define ADC_CHANNEL_6_ENABLE
//#define ADC_CHANNEL_7_ENABLE
//#define ADC_CHANNEL_8_ENABLE
//#define ADC_CHANNEL_9_ENABLE
#define ADC_CHANNEL_TEMPSENSOR_ENABLE

typedef enum {
#ifdef ADC_CHANNEL_0_ENABLE
    AD_CHANNEL_0,
#endif
#ifdef ADC_CHANNEL_1_ENABLE
    AD_CHANNEL_1,
#endif
#ifdef ADC_CHANNEL_2_ENABLE
    AD_CHANNEL_2,
#endif
#ifdef ADC_CHANNEL_3_ENABLE
    AD_CHANNEL_3,
#endif
#ifdef ADC_CHANNEL_6_ENABLE
    AD_CHANNEL_6,
#endif
#ifdef ADC_CHANNEL_7_ENABLE
    AD_CHANNEL_7,
#endif
#ifdef ADC_CHANNEL_8_ENABLE
    AD_CHANNEL_8,
#endif
#ifdef ADC_CHANNEL_9_ENABLE
    AD_CHANNEL_9,
#endif
#ifdef ADC_CHANNEL_TEMPSENSOR_ENABLE
    AD_CHANNEL_TEMPSENSOR,
#endif
    AD_CHANNEL_TOTAL
}AD_CHANNEL_NUM;


/**
* 名称       : BSP_ADC_Init()
* 创建日期   : 2020-04-27
* 作者       : 杨春林
* 功能       : DAC初始化
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : ADC通道通过ADC_CHANNEL_0_ENABLE、ADC_CHANNEL_1_ENABLE,...,ADC_CHANNEL_9_ENABLE,
*                      ADC_CHANNEL_TEMPSENSOR_ENABLE宏定义打开
* 修改内容   :
*/
void BSP_ADC_Init(void);


/**
* 名称       : BSP_ADC_Start_DMA()
* 创建日期   : 2020-04-27
* 作者       : 杨春林
* 功能       : ADC开始DMA传输
* 输入参数   : pData：DMA接收缓存指针;
*              Length：缓存大小
* 输出参数   : 无
* 返回结果   : OP_SUCCESS(成功),OP_FAILED(失败)
* 注意和说明 : 
* 修改内容   :
*/
uint8_t BSP_ADC_Start_DMA(uint32_t *pData, uint32_t Length);


#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
