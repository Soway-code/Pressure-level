/**@file        adc_bsp.c
* @brief        ADC驱动
* @details      ADC多通道配置, DMA传输配置, 内部温度传感器配置, 
* ADC模块多通道DMA传输, 内部温度ADC转换
* @author       杨春林
* @date         2020-04-27
* @version      V1.0.0
* @copyright    2020-2030,深圳市信为科技发展有限公司
**********************************************************************************
* @par 修改日志:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/04/27  <td>1.0.0    <td>杨春林    <td>创建初始版本
* </table>
*
**********************************************************************************
*/

#include "adc_bsp.h"
#include "VariaType.h"

/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS
#include "board.h"
#endif // USING_RT_THREAD_OS


static ADC_HandleTypeDef hadc;          ///< ADC处理对象
static DMA_HandleTypeDef hdma_adc;      ///< ADC的DMA处理对象


/**@brief       ADC初始化
* @return       函数执行结果
* - None
* @note         ADC通道通过ADC_CHANNEL_0_ENABLE, ADC_CHANNEL_1_ENABLE, ..., 
* ADC_CHANNEL_9_ENABLE, ADC_CHANNEL_TEMPSENSOR_ENABLE宏定义打开
*/
void BSP_ADC_Init(void)
{
#if defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
    || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
    || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE) \
    || defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)
    
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc.Instance = ADCx;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
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
#if defined(STM32L0)
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_160CYCLES_5;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
#endif // defined(STM32L0)
  
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }  
  
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
#if defined(STM32F0)
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
#endif // defined(STM32F0)

#ifdef ADC_CHANNEL_0_ENABLE
  sConfig.Channel = ADC_CHANNEL_0;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif // ADC_CHANNEL_0_ENABLE
  
#ifdef ADC_CHANNEL_1_ENABLE
  sConfig.Channel = ADC_CHANNEL_1;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif // ADC_CHANNEL_1_ENABLE
  
#ifdef ADC_CHANNEL_2_ENABLE
  sConfig.Channel = ADC_CHANNEL_2;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif // ADC_CHANNEL_2_ENABLE
  
#ifdef ADC_CHANNEL_3_ENABLE
  sConfig.Channel = ADC_CHANNEL_3;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif // ADC_CHANNEL_3_ENABLE
  
#ifdef ADC_CHANNEL_6_ENABLE
  sConfig.Channel = ADC_CHANNEL_6;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif // ADC_CHANNEL_6_ENABLE
  
#ifdef ADC_CHANNEL_7_ENABLE
  sConfig.Channel = ADC_CHANNEL_7;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif // ADC_CHANNEL_7_ENABLE

#ifdef ADC_CHANNEL_8_ENABLE
  sConfig.Channel = ADC_CHANNEL_8;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif // ADC_CHANNEL_8_ENABLE
  
#ifdef ADC_CHANNEL_9_ENABLE
  sConfig.Channel = ADC_CHANNEL_9;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif // ADC_CHANNEL_9_ENABLE
  
#ifdef ADC_CHANNEL_TEMPSENSOR_ENABLE
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
#endif // ADC_CHANNEL_TEMPSENSOR_ENABLE
  
  if (HAL_ADCEx_Calibration_Start(&hadc
#if defined(STM32L0)
    , ADC_SINGLE_ENDED
#endif // defined(STM32L0)
      ) != HAL_OK)
  {
    Error_Handler();
  }

#endif //defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
        || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
        || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE) \
        || defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)
}

/**@brief       ADC时钟、底层管脚初始化(由HAL_ADC_Init回调调用)
* @param[in]    adcHandle : ADC处理对象
* @return       函数执行结果
* - None
* @note         ADC通道通过ADC_CHANNEL_0_ENABLE, ADC_CHANNEL_1_ENABLE, ..., 
* ADC_CHANNEL_9_ENABLE, ADC_CHANNEL_TEMPSENSOR_ENABLE宏定义打开
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{
#if defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
    || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
    || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE) \
    || defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* DMA controller clock enable */
    DMAx_CHANNELx_CLK_ENABLE();
        
    /* ADC1 clock enable */
    ADCx_CLK_ENABLE();      

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
#if defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
    || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
    || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE)
      
    __HAL_RCC_GPIOA_CLK_ENABLE();    
#if defined(ADC_CHANNEL_0_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_0;
#endif // defined(ADC_CHANNEL_0_ENABLE)
    
#if defined(ADC_CHANNEL_1_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_1;
#endif // defined(ADC_CHANNEL_1_ENABLE)

#if defined(ADC_CHANNEL_2_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_2;
#endif // defined(ADC_CHANNEL_2_ENABLE)

#if defined(ADC_CHANNEL_3_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_3;
#endif // defined(ADC_CHANNEL_3_ENABLE)

#if defined(ADC_CHANNEL_6_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_6;
#endif // defined(ADC_CHANNEL_6_ENABLE)

#if defined(ADC_CHANNEL_7_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_7;
#endif // defined(ADC_CHANNEL_7_ENABLE)

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif // defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
        || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
        || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE)
    
#if defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)

    __HAL_RCC_GPIOB_CLK_ENABLE(); 
    GPIO_InitStruct.Pin = 0;
#if defined(ADC_CHANNEL_8_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_0;
#endif // defined(ADC_CHANNEL_8_ENABLE)

#if defined(ADC_CHANNEL_9_ENABLE)
    GPIO_InitStruct.Pin |= GPIO_PIN_1;
#endif // defined(ADC_CHANNEL_9_ENABLE)

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif // defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)
    

    /* ADC1 DMA Init */
    /* ADC Init */
    hdma_adc.Instance = ADCx_DMA_STREAM;
#if defined(STM32L0)
    hdma_adc.Init.Request = ADCx_DMA_REQUEST;
#endif // defined(STM32L0)
    hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc.Init.Mode = DMA_CIRCULAR;
    hdma_adc.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_adc) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc);

    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADCx_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADCx_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  
#endif // defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
        || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
        || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE) \
        || defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)
}

/**@brief       ADC时钟、底层管脚反初始化(由HAL_ADC_DeInit回调调用)
* @param[in]    adcHandle : ADC处理对象
* @return       函数执行结果
* - None
* @note         ADC通道通过ADC_CHANNEL_0_ENABLE, ADC_CHANNEL_1_ENABLE, ..., 
* ADC_CHANNEL_9_ENABLE, ADC_CHANNEL_TEMPSENSOR_ENABLE宏定义打开
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{
#if defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
    || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
    || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE) \
    || defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)
    
    uint32_t gpio_pin = 0;
    
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    ADCx_CLK_DISABLE();

#if defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
    || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
    || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE)
      
#if defined(ADC_CHANNEL_0_ENABLE)
      gpio_pin |= GPIO_PIN_0;
#endif // defined(ADC_CHANNEL_0_ENABLE)
    
#if defined(ADC_CHANNEL_1_ENABLE)
      gpio_pin |= GPIO_PIN_1;
#endif // defined(ADC_CHANNEL_1_ENABLE)

#if defined(ADC_CHANNEL_2_ENABLE)
      gpio_pin |= GPIO_PIN_2;
#endif // defined(ADC_CHANNEL_2_ENABLE)

#if defined(ADC_CHANNEL_3_ENABLE)
      gpio_pin |= GPIO_PIN_3;
#endif // defined(ADC_CHANNEL_3_ENABLE)

#if defined(ADC_CHANNEL_6_ENABLE)
      gpio_pin |= GPIO_PIN_6;
#endif // defined(ADC_CHANNEL_6_ENABLE)

#if defined(ADC_CHANNEL_7_ENABLE)
      gpio_pin |= GPIO_PIN_7;
#endif // defined(ADC_CHANNEL_7_ENABLE)

    HAL_GPIO_DeInit(GPIOA, gpio_pin);
                          
#endif // defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
        || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
        || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE)

#if defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)
      
#if defined(ADC_CHANNEL_8_ENABLE)
      gpio_pin |= GPIO_PIN_0;
#endif // defined(ADC_CHANNEL_8_ENABLE)
#if defined(ADC_CHANNEL_9_ENABLE)
      gpio_pin |= GPIO_PIN_1;
#endif // defined(ADC_CHANNEL_9_ENABLE)

    HAL_GPIO_DeInit(GPIOB, gpio_pin);
                          
#endif // defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADCx_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
#endif // defined(ADC_CHANNEL_0_ENABLE) || defined(ADC_CHANNEL_1_ENABLE) \
        || defined(ADC_CHANNEL_2_ENABLE) || defined(ADC_CHANNEL_3_ENABLE) \
        || defined(ADC_CHANNEL_6_ENABLE) || defined(ADC_CHANNEL_7_ENABLE) \
        || defined(ADC_CHANNEL_8_ENABLE) || defined(ADC_CHANNEL_9_ENABLE)
} 

/**@brief       ADC开始DMA传输
* @param[in]    pData : DMA接收缓存指针;
* @param[in]    Length : 缓存大小
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
*/
uint8_t BSP_ADC_Start_DMA(uint32_t *pData, uint32_t Length)
{
    if(HAL_ADC_Start_DMA(&hadc, pData,  Length) != HAL_OK)
    {
        return OP_FAILED;
    }
    __HAL_DMA_DISABLE_IT(hadc.DMA_Handle, DMA_IT_HT);
    return OP_SUCCESS;
}

void BSP_ADC_DMA_Enable_IT(void)
{
    __HAL_DMA_ENABLE_IT(hadc.DMA_Handle, DMA_IT_TC);
}

void BSP_ADC_DMA_Disable_IT(void)
{
    __HAL_DMA_DISABLE_IT(hadc.DMA_Handle, DMA_IT_TC);
}

/**@brief       ADC DMA中断服务函数
* @return       函数执行结果
* - None
* @note         USER_ADC_DMA_IRQHANDLER在adc_bsp.h中定义
*/
void ADCx_DMA_IRQHandler(void)
{
/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS
    rt_interrupt_enter();
#endif // USING_RT_THREAD_OS
    
    HAL_DMA_IRQHandler(&hdma_adc);
    
/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS
    rt_interrupt_leave();
#endif // USING_RT_THREAD_OS
}

/**@brief       ADC中断服务函数
* @return       函数执行结果
* - None
* @note         USER_ADC_IRQHANDLER在adc_bsp.h中定义
*/
void ADCx_IRQHandler(void)
{
/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS
    rt_interrupt_enter();
#endif // USING_RT_THREAD_OS
    
    HAL_ADC_IRQHandler(&hadc);

/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS
    rt_interrupt_leave();
#endif // USING_RT_THREAD_OS
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
