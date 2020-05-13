/****************************************************************************************************************************************************************
** 版权:          2020-2030,深圳市信为科技发展有限公司
** 文件名:        dac_bsp.c
** 作者:          杨春林
** 版本:          V1.0.0
** 日期:          2020-04-27
** 描述:          DAC 通道配置传输配置
** 功能:          DAC 通道配置 DAC转换
*****************************************************************************************************************************************************************
** 修改者:        No
** 版本:          No
** 修改内容:      No
** 日期:          No
****************************************************************************************************************************************************************/

#include "dac_bsp.h"
#include "VariaType.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

static DAC_HandleTypeDef hdac;

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
void BSP_DAC_Init(void)
{
#if defined(DAC_CHANNEL_1_ENABLE) || defined(DAC_CHANNEL_2_ENABLE)
    
  DAC_ChannelConfTypeDef sConfig = {0};

  /** DAC Initialization 
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
#if defined(DAC_CHANNEL_1_ENABLE)
  /** DAC channel OUT1 config 
  */  
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
#endif
  
#if defined(DAC_CHANNEL_2_ENABLE)
  /** DAC channel OUT2 config 
  */
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
#endif
  
#endif
}

/**
* 名称       : HAL_DAC_MspInit()
* 创建日期   : 2020-04-27
* 作者       : 杨春林
* 功能       : DAC时钟、底层管脚初始化（由HAL_DAC_Init回调调用）
* 输入参数   : dacHandle，DAC处理对象
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : DAC通道通过DAC_CHANNEL_4_ENABLE和DAC_CHANNEL_5_ENABLE宏定义打开
* 修改内容   :
*/
void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle)
{
#if defined(DAC_CHANNEL_1_ENABLE) || defined(DAC_CHANNEL_2_ENABLE)
    
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(dacHandle->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspInit 0 */

  /* USER CODE END DAC_MspInit 0 */
    /* DAC clock enable */
    __HAL_RCC_DAC1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**DAC GPIO Configuration    
    PA4     ------> DAC_OUT1
    PA5     ------> DAC_OUT2 
    */
    
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
#if defined(DAC_CHANNEL_1_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_4;
#endif
#if defined(DAC_CHANNEL_2_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_5;
#endif
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN DAC_MspInit 1 */

  /* USER CODE END DAC_MspInit 1 */
  }
  
#endif
}

/**
* 名称       : HAL_DAC_MspDeInit()
* 创建日期   : 2020-04-27
* 作者       : 杨春林
* 功能       : DAC时钟、底层管脚反初始化（由HAL_DAC_DeInit回调调用）
* 输入参数   : dacHandle，DAC处理对象
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : DAC通道通过DAC_CHANNEL_4_ENABLE和DAC_CHANNEL_5_ENABLE宏定义打开
* 修改内容   :
*/
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle)
{
#if defined(DAC_CHANNEL_1_ENABLE) || defined(DAC_CHANNEL_2_ENABLE)
  uint32_t gpio_pin = 0;
    
  if(dacHandle->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspDeInit 0 */

  /* USER CODE END DAC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DAC1_CLK_DISABLE();
  
    /**DAC GPIO Configuration    
    PA4     ------> DAC_OUT1
    PA5     ------> DAC_OUT2 
    */
#if defined(DAC_CHANNEL_1_ENABLE)
    gpio_pin |= GPIO_PIN_4;
#endif
#if defined(DAC_CHANNEL_2_ENABLE)
    gpio_pin |= GPIO_PIN_5;
#endif
    HAL_GPIO_DeInit(GPIOA, gpio_pin);

  /* USER CODE BEGIN DAC_MspDeInit 1 */

  /* USER CODE END DAC_MspDeInit 1 */
  }
  
#endif
} 

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
uint8_t BSP_DAC_Convert(uint16_t DA_Value, DA_CHANNEL_NUM DA_Channel_Num)
{
  uint32_t channel;
  //DAC值超出范围
  if(DA_Value > 4095)
  {
    return OP_FAILED;
  }

  if(DA_Channel_Num != DA_CHANNEL_1 && DA_Channel_Num != DA_CHANNEL_2)
  {
    return OP_FAILED;
  }
  
  if(DA_Channel_Num == DA_CHANNEL_1)
      channel = DAC_CHANNEL_1;
  else if(DA_Channel_Num == DA_CHANNEL_2)
      channel = DAC_CHANNEL_2;
  
  HAL_DAC_SetValue(&hdac, channel, DAC_ALIGN_12B_R, DA_Value);
  HAL_DAC_Start(&hdac, channel);
  
  return OP_SUCCESS;
}

