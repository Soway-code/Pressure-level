/****************************************************************************************************************************************************************
** 版权:          2020-2030,深圳市信为科技发展有限公司
** 文件名:        adc_bsp.c
** 作者:          杨春林
** 版本:          V1.0.0
** 日期:          2020-04-27
** 描述:          ADC多通道配置 DMA传输配置 内部温度传感器配置
** 功能:          ADC模块多通道DMA驱动 内部温度ADC转换
*****************************************************************************************************************************************************************
** 修改者:        No
** 版本:          No
** 修改内容:      No
** 日期:          No
****************************************************************************************************************************************************************/

#include "adc_bsp.h"
#include "VariaType.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

static ADC_HandleTypeDef hadc;
static DMA_HandleTypeDef hdma_adc;

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
void BSP_ADC_Init(void)
{
#if defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
    || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
    || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE) \
    || defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)
    
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;

#ifdef ADC_CHANNEL_0_ENABLE
  sConfig.Channel = ADC_CHANNEL_0;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif
  
#ifdef ADC_CHANNEL_1_ENABLE
  sConfig.Channel = ADC_CHANNEL_1;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif
  
#ifdef ADC_CHANNEL_2_ENABLE
  sConfig.Channel = ADC_CHANNEL_2;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif
  
#ifdef ADC_CHANNEL_3_ENABLE
  sConfig.Channel = ADC_CHANNEL_3;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif
  
#ifdef ADC_CHANNEL_6_ENABLE
  sConfig.Channel = ADC_CHANNEL_6;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif
  
#ifdef ADC_CHANNEL_7_ENABLE
  sConfig.Channel = ADC_CHANNEL_7;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif

#ifdef ADC_CHANNEL_8_ENABLE
  sConfig.Channel = ADC_CHANNEL_8;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif
  
#ifdef ADC_CHANNEL_9_ENABLE
  sConfig.Channel = ADC_CHANNEL_9;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif
  
#ifdef ADC_CHANNEL_TEMPSENSOR_ENABLE
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif

#endif
}

/**
* 名称       : HAL_ADC_MspInit()
* 创建日期   : 2020-04-27
* 作者       : 杨春林
* 功能       : ADC时钟、底层管脚初始化（由HAL_ADC_Init回调调用）
* 输入参数   : adcHandle，ADC处理对象
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : ADC通道通过ADC_CHANNEL_0_ENABLE、ADC_CHANNEL_1_ENABLE,...,ADC_CHANNEL_9_ENABLE,
*                      ADC_CHANNEL_TEMPSENSOR_ENABLE宏定义打开
* 修改内容   :
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{
#if defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
    || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
    || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE) \
    || defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)
    
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
      
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC GPIO Configuration    
    PA0     ------> ADC_IN0
    PA1     ------> ADC_IN1
    PA2     ------> ADC_IN2
    PA3     ------> ADC_IN3
    PA6     ------> ADC_IN6
    PA7     ------> ADC_IN7
    PB0     ------> ADC_IN8
    PB1     ------> ADC_IN9 
    */
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
#if defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
    || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
    || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE)
      
    __HAL_RCC_GPIOA_CLK_ENABLE();    
#if defined(ADC_CHANNEL_0_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_0;
#endif
#if defined(ADC_CHANNEL_1_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_1;
#endif
#if defined(ADC_CHANNEL_2_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_2;
#endif
#if defined(ADC_CHANNEL_3_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_3;
#endif
#if defined(ADC_CHANNEL_6_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_6;
#endif
#if defined(ADC_CHANNEL_7_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_7;
#endif
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif        
    
#if defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)

    __HAL_RCC_GPIOB_CLK_ENABLE(); 
    GPIO_InitStruct.Pin = 0;
#if defined(ADC_CHANNEL_8_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_0;
#endif
#if defined(ADC_CHANNEL_9_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_1;
#endif
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif
    

    /* ADC1 DMA Init */
    /* ADC Init */
    hdma_adc.Instance = DMA1_Channel1;
    hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc.Init.Mode = DMA_CIRCULAR;
    hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_adc) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc);

    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
  
#endif
}

/**
* 名称       : HAL_ADC_MspDeInit()
* 创建日期   : 2020-04-27
* 作者       : 杨春林
* 功能       : ADC时钟、底层管脚反初始化（由HAL_ADC_DeInit回调调用）
* 输入参数   : adcHandle，ADC处理对象
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : ADC通道通过ADC_CHANNEL_0_ENABLE、ADC_CHANNEL_1_ENABLE,...,ADC_CHANNEL_9_ENABLE,
*                      ADC_CHANNEL_TEMPSENSOR_ENABLE宏定义打开
* 修改内容   :
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{
#if defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
    || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
    || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE) \
    || defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)
    
    uint32_t gpio_pin = 0;
    
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC GPIO Configuration    
    PA0     ------> ADC_IN0
    PA1     ------> ADC_IN1
    PA2     ------> ADC_IN2
    PA3     ------> ADC_IN3
    PA6     ------> ADC_IN6
    PA7     ------> ADC_IN7
    PB0     ------> ADC_IN8
    PB1     ------> ADC_IN9 
    */
#if defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
    || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
    || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE)
      
#if defined(ADC_CHANNEL_0_ENABLE)
      gpio_pin |= GPIO_PIN_0;
#endif
#if defined(ADC_CHANNEL_1_ENABLE)
      gpio_pin |= GPIO_PIN_1;
#endif
#if defined(ADC_CHANNEL_2_ENABLE)
      gpio_pin |= GPIO_PIN_2;
#endif
#if defined(ADC_CHANNEL_3_ENABLE)
      gpio_pin |= GPIO_PIN_3;
#endif
#if defined(ADC_CHANNEL_6_ENABLE)
      gpio_pin |= GPIO_PIN_6;
#endif
#if defined(ADC_CHANNEL_7_ENABLE)
      gpio_pin |= GPIO_PIN_7;
#endif

    HAL_GPIO_DeInit(GPIOA, gpio_pin);
                          
#endif

#if defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE) \
      
#if defined(ADC_CHANNEL_8_ENABLE)
      gpio_pin |= GPIO_PIN_0;
#endif
#if defined(ADC_CHANNEL_9_ENABLE)
      gpio_pin |= GPIO_PIN_1;
#endif

    HAL_GPIO_DeInit(GPIOB, gpio_pin);
                          
#endif

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC1_COMP_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
#endif
} 

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
uint8_t BSP_ADC_Start_DMA(uint32_t *pData, uint32_t Length)
{
    if(HAL_ADC_Start_DMA(&hadc, pData,  Length) != HAL_OK)
    {
        return OP_FAILED;
    }
    return OP_SUCCESS;
}

/**
* 名称       : DMA1_Channel1_IRQHandler()
* 创建日期   : 2020-04-27
* 作者       : 杨春林
* 功能       : DMA1通道1中断服务函数
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void DMA1_Channel1_IRQHandler(void)
{

  HAL_DMA_IRQHandler(&hdma_adc);

}

/**
* 名称       : ADC1_COMP_IRQHandler()
* 创建日期   : 2020-04-27
* 作者       : 杨春林
* 功能       : ADC1中断服务函数
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ADC1_COMP_IRQHandler(void)
{

  HAL_ADC_IRQHandler(&hadc);

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
