#ifndef __iwdg_H
#define __iwdg_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/**
* 名称       : BSP_IWDG_Init()
* 创建日期   : 2020-04-27
* 作者       : 杨春林
* 功能       : 独立看门狗初始化
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 看门狗时钟由内部40KHz振荡器分频后得到，溢出时间 = 分频系数*重载值/40 ms，
*              例如8分频，重载值4000，则溢出时间为 800 ms
* 修改内容   :
*/
void BSP_IWDG_Init(void);

/**
* 名称       : BSP_IWDG_Refresh()
* 创建日期   : 2020-04-27
* 作者       : 杨春林
* 功能       : 独立看门狗喂狗
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void BSP_IWDG_Refresh(void);

#ifdef __cplusplus
}
#endif
#endif /*__ iwdg_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
