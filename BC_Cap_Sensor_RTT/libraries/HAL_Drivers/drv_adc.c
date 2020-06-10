/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-05     zylx         first version
 * 2018-12-12     greedyhao    Porting for stm32f7xx
 * 2019-02-01     yuneizhilin   fix the stm32_adc_init function initialization issue
 */

#include <board.h>

#if defined(BSP_USING_ADC1) || defined(BSP_USING_ADC2) || defined(BSP_USING_ADC3)
#include "drv_config.h"

//#define DRV_DEBUG
#define LOG_TAG             "drv.adc"
#include <drv_log.h>

static ADC_HandleTypeDef adc_config[] =
{
#ifdef BSP_USING_ADC1
    ADC1_CONFIG,
#endif

#ifdef BSP_USING_ADC2
    ADC2_CONFIG,
#endif

#ifdef BSP_USING_ADC3
    ADC3_CONFIG,
#endif
};

#ifdef BSP_ADC_USING_DMA
#include "drv_dma.h"
static DMA_HandleTypeDef adc_dma_config[] =
{
#ifdef BSP_ADC1_USING_DMA
    ADC1_DMA_CONFIG,
#endif

#ifdef BSP_ADC2_USING_DMA
    ADC2_DMA_CONFIG,
#endif

#ifdef BSP_ADC3_USING_DMA
    ADC3_DMA_CONFIG,
#endif
};

static struct dma_config adc_dma_channel[] = 
{
#ifdef BSP_ADC1_USING_DMA
    ADC1_DMA_INSTANCE,
    ADC1_DMA_RCC,
    ADC1_DMA_IRQ
#endif

#ifdef BSP_ADC2_USING_DMA
    ADC2_DMA_CONFIG,
#endif

#ifdef BSP_ADC3_USING_DMA
    ADC3_DMA_CONFIG,
#endif
};

enum ADC_DMA_INDIX
{
#ifdef BSP_USING_ADC1
    ADC1_INDEX,
#endif
#ifdef BSP_USING_ADC2
    ADC2_INDEX,
#endif
#ifdef BSP_USING_ADC3
    ADC3_INDEX,
#endif
};
#endif

struct stm32_adc
{
    ADC_HandleTypeDef ADC_Handler;
#if defined(BSP_ADC_USING_DMA)
    DMA_HandleTypeDef DMA_Handler;
    struct dma_config dma_config;
    struct rt_semaphore sem;
#endif
    struct rt_adc_device stm32_adc_device;
};

static struct stm32_adc stm32_adc_obj[sizeof(adc_config) / sizeof(adc_config[0])];

static rt_err_t stm32_adc_enabled(struct rt_adc_device *device, rt_uint32_t channel, rt_bool_t enabled)
{
    ADC_HandleTypeDef *stm32_adc_handler = device->parent.user_data;

    RT_ASSERT(device != RT_NULL);

    if (enabled)
    {
#if defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32G0)
        ADC_Enable(stm32_adc_handler);
#else
        __HAL_ADC_ENABLE(stm32_adc_handler);
#endif
    }
    else
    {
#if defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32G0)
        ADC_Disable(stm32_adc_handler);
#else
        __HAL_ADC_DISABLE(stm32_adc_handler);
#endif
    }

    return RT_EOK;
}

static rt_uint32_t stm32_adc_get_channel(rt_uint32_t channel)
{
    rt_uint32_t stm32_channel = 0;

    switch (channel)
    {
    case  0:
        stm32_channel = ADC_CHANNEL_0;
        break;
    case  1:
        stm32_channel = ADC_CHANNEL_1;
        break;
    case  2:
        stm32_channel = ADC_CHANNEL_2;
        break;
    case  3:
        stm32_channel = ADC_CHANNEL_3;
        break;
    case  4:
        stm32_channel = ADC_CHANNEL_4;
        break;
    case  5:
        stm32_channel = ADC_CHANNEL_5;
        break;
    case  6:
        stm32_channel = ADC_CHANNEL_6;
        break;
    case  7:
        stm32_channel = ADC_CHANNEL_7;
        break;
    case  8:
        stm32_channel = ADC_CHANNEL_8;
        break;
    case  9:
        stm32_channel = ADC_CHANNEL_9;
        break;
    case 10:
        stm32_channel = ADC_CHANNEL_10;
        break;
    case 11:
        stm32_channel = ADC_CHANNEL_11;
        break;
    case 12:
        stm32_channel = ADC_CHANNEL_12;
        break;
    case 13:
        stm32_channel = ADC_CHANNEL_13;
        break;
    case 14:
        stm32_channel = ADC_CHANNEL_14;
        break;
    case 15:
        stm32_channel = ADC_CHANNEL_15;
        break;
    case 16:
        stm32_channel = ADC_CHANNEL_16;
        break;
    case 17:
        stm32_channel = ADC_CHANNEL_17;
        break;
#if defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7) || defined(SOC_SERIES_STM32L4)
    case 18:
        stm32_channel = ADC_CHANNEL_18;
        break;
#endif
    }

    return stm32_channel;
}

static rt_err_t stm32_get_adc_value(struct rt_adc_device *device, rt_uint32_t channel, rt_uint32_t *value)
{
    ADC_ChannelConfTypeDef ADC_ChanConf;
    ADC_HandleTypeDef *stm32_adc_handler = device->parent.user_data;

    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(value != RT_NULL);

    rt_memset(&ADC_ChanConf, 0, sizeof(ADC_ChanConf));

#if defined(SOC_SERIES_STM32F1)
    if (channel <= 17)
#elif defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7) \
        || defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32G0)
    if (channel <= 18)
#endif
    {
        /* set stm32 ADC channel */
        ADC_ChanConf.Channel =  stm32_adc_get_channel(channel);
    }
    else
    {
#if defined(SOC_SERIES_STM32F1)
        LOG_E("ADC channel must be between 0 and 17.");
#elif defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7) \
        || defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32G0)
        LOG_E("ADC channel must be between 0 and 18.");
#endif
        return -RT_ERROR;
    }
    ADC_ChanConf.Rank = ADC_RANK_CHANNEL_NUMBER;
#if defined(SOC_SERIES_STM32F0)
    ADC_ChanConf.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
#elif defined(SOC_SERIES_STM32F1)
    ADC_ChanConf.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
#elif defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7)
    ADC_ChanConf.SamplingTime = ADC_SAMPLETIME_112CYCLES;
#elif defined(SOC_SERIES_STM32L4)
    ADC_ChanConf.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
#endif
#if defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7) || defined(SOC_SERIES_STM32L4)
    ADC_ChanConf.Offset = 0;
#endif
#ifdef SOC_SERIES_STM32L4
    ADC_ChanConf.OffsetNumber = ADC_OFFSET_NONE;
    ADC_ChanConf.SingleDiff = LL_ADC_SINGLE_ENDED;
#endif
    HAL_ADC_ConfigChannel(stm32_adc_handler, &ADC_ChanConf);

    /* start ADC */
    HAL_ADC_Start(stm32_adc_handler);

    /* Wait for the ADC to convert */
    HAL_ADC_PollForConversion(stm32_adc_handler, 100);

    /* get ADC value */
    *value = (rt_uint32_t)HAL_ADC_GetValue(stm32_adc_handler);

    return RT_EOK;
}

#ifdef BSP_ADC_USING_DMA
static rt_err_t stm32_get_adc_value_dma(struct rt_adc_device *device, rt_uint32_t channel, rt_uint32_t *value, rt_size_t size)
{
    rt_err_t result = RT_EOK;
    ADC_ChannelConfTypeDef ADC_ChanConf;
    ADC_HandleTypeDef *stm32_adc_handler = device->parent.user_data;
    struct stm32_adc *adc_obj = (struct stm32_adc *)stm32_adc_handler;
    uint8_t i;

    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(value != RT_NULL);
    
    rt_memset(&ADC_ChanConf, 0, sizeof(ADC_ChanConf));

    ADC_ChanConf.Rank = ADC_RANK_CHANNEL_NUMBER;
#if defined(SOC_SERIES_STM32F0)
    ADC_ChanConf.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
#elif defined(SOC_SERIES_STM32F1)
    ADC_ChanConf.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
#elif defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7)
    ADC_ChanConf.SamplingTime = ADC_SAMPLETIME_112CYCLES;
#elif defined(SOC_SERIES_STM32L4)
    ADC_ChanConf.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
#endif
#if defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7) || defined(SOC_SERIES_STM32L4)
    ADC_ChanConf.Offset = 0;
#endif
#ifdef SOC_SERIES_STM32L4
    ADC_ChanConf.OffsetNumber = ADC_OFFSET_NONE;
    ADC_ChanConf.SingleDiff = LL_ADC_SINGLE_ENDED;
#endif
    for(i = 0; i < size; i++)
    {
#if defined(SOC_SERIES_STM32F1)
        if (channel <= 17)
#elif defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7) \
        || defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32G0)
        if (channel <= 18)
#endif
        {
            /* set stm32 ADC channel */
            ADC_ChanConf.Channel =  stm32_adc_get_channel(channel + i);
        }
        else
        {
#if defined(SOC_SERIES_STM32F1)
            LOG_E("ADC channel must be between 0 and 17.");
#elif defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7) \
        || defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32G0)
            LOG_E("ADC channel must be between 0 and 18.");
#endif
            return -RT_ERROR;
        }
        HAL_ADC_ConfigChannel(stm32_adc_handler, &ADC_ChanConf);
    }
    /* start ADC */
    if(HAL_ADC_Start_DMA(stm32_adc_handler, value, size) != HAL_OK)
        result = -RT_ERROR;
    rt_sem_take(&adc_obj->sem, RT_WAITING_FOREVER);
    
    ADC_ChanConf.Rank = ADC_RANK_NONE;
    for(i = 0; i < size; i++)
    {
#if defined(SOC_SERIES_STM32F1)
        if (channel <= 17)
#elif defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7) \
        || defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32G0)
        if (channel <= 18)
#endif
        {
            /* set stm32 ADC channel */
            ADC_ChanConf.Channel =  stm32_adc_get_channel(channel + i);
        }
        else
        {
#if defined(SOC_SERIES_STM32F1)
            LOG_E("ADC channel must be between 0 and 17.");
#elif defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7) \
        || defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32G0)
            LOG_E("ADC channel must be between 0 and 18.");
#endif
            return -RT_ERROR;
        }
        HAL_ADC_ConfigChannel(stm32_adc_handler, &ADC_ChanConf);
    }

    return result;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    struct stm32_adc *adc_obj = (struct stm32_adc *)hadc;
    rt_sem_release(&adc_obj->sem);
}

/**
  * @brief This function handles DMA1 channel 1 global interrupt.
  */
void ADC1_DMA_IRQHandler(void)
{
  rt_interrupt_enter();
  HAL_DMA_IRQHandler(&stm32_adc_obj[ADC1_INDEX].DMA_Handler);
  rt_interrupt_leave();
}

/**
  * @brief This function handles ADC and COMP interrupts (COMP interrupts through EXTI lines 21 and 22).
  */
void ADC1_COMP_IRQHandler(void)
{
  rt_interrupt_enter();
  HAL_ADC_IRQHandler(&stm32_adc_obj[ADC1_INDEX].ADC_Handler);
  rt_interrupt_leave();
}
#endif

static const struct rt_adc_ops stm_adc_ops =
{
    .enabled = stm32_adc_enabled,
#if defined(BSP_ADC_USING_DMA)
    .convert_dma = stm32_get_adc_value_dma,
#endif
    .convert = stm32_get_adc_value,
};

static int stm32_adc_init(void)
{
    int result = RT_EOK;
    /* save adc name */
    char name_buf[5] = {'a', 'd', 'c', '0', 0};
    int i = 0;

    for (i = 0; i < sizeof(adc_config) / sizeof(adc_config[0]); i++)
    {
        /* ADC init */
        name_buf[3] = '0';
        stm32_adc_obj[i].ADC_Handler = adc_config[i];
#if defined(BSP_ADC_USING_DMA)
        stm32_adc_obj[i].DMA_Handler = adc_dma_config[i];
        stm32_adc_obj[i].dma_config  = adc_dma_channel[i];
        {
            rt_uint32_t tmpreg= 0x00U;
#if defined(SOC_SERIES_STM32F1) || defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32G0) \
        || defined(SOC_SERIES_STM32L0)
            /* enable DMA clock && Delay after an RCC peripheral clock enabling*/
            SET_BIT(RCC->AHBENR, stm32_adc_obj[i].dma_config.dma_rcc);
            tmpreg = READ_BIT(RCC->AHBENR, stm32_adc_obj[i].dma_config.dma_rcc);
#elif defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7) || defined(SOC_SERIES_STM32L4)
            /* enable DMA clock && Delay after an RCC peripheral clock enabling*/
            SET_BIT(RCC->AHB1ENR, uart->config->dma_rx->dma_rcc);
            tmpreg = READ_BIT(RCC->AHB1ENR, uart->config->dma_rx->dma_rcc);
#endif  
            UNUSED(tmpreg);   /* To avoid compiler warnings */
        }
#endif
#if defined(ADC1)
        if (stm32_adc_obj[i].ADC_Handler.Instance == ADC1)
        {
            name_buf[3] = '1';
        }
#endif
#if defined(ADC2)
        if (stm32_adc_obj[i].ADC_Handler.Instance == ADC2)
        {
            name_buf[3] = '2';
        }
#endif
#if defined(ADC3)
        if (stm32_adc_obj[i].ADC_Handler.Instance == ADC3)
        {
            name_buf[3] = '3';
        }
#endif
        if (HAL_ADC_Init(&stm32_adc_obj[i].ADC_Handler) != HAL_OK
#if defined(BSP_ADC_USING_DMA)
        || HAL_DMA_Init(&stm32_adc_obj[i].DMA_Handler) != HAL_OK
#endif
        )
        {
            LOG_E("%s init failed", name_buf);
            result = -RT_ERROR;
        }
        else
        {
#if defined(BSP_ADC_USING_DMA)
            __HAL_LINKDMA(&stm32_adc_obj[i].ADC_Handler, DMA_Handle, stm32_adc_obj[i].DMA_Handler);
            HAL_NVIC_SetPriority(stm32_adc_obj[i].dma_config.dma_irq, 1, 0);
            HAL_NVIC_EnableIRQ(stm32_adc_obj[i].dma_config.dma_irq);            
            rt_sem_init(&stm32_adc_obj[i].sem, name_buf, 0, RT_IPC_FLAG_FIFO);
#endif
            
            /* register ADC device */
            if (rt_hw_adc_register(&stm32_adc_obj[i].stm32_adc_device, name_buf, &stm_adc_ops, &stm32_adc_obj[i].ADC_Handler) == RT_EOK)
            {
                LOG_D("%s init success", name_buf);
            }
            else
            {
                LOG_E("%s register failed", name_buf);
                result = -RT_ERROR;
            }
        }
    }

    return result;
}
INIT_BOARD_EXPORT(stm32_adc_init);

#endif /* BSP_USING_ADC */
