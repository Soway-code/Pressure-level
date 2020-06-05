/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-24     zylx         first version
 */

#ifndef __ADC_CONFIG_H__
#define __ADC_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BSP_USING_ADC1
#ifndef ADC1_CONFIG
#define ADC1_CONFIG                                                 \
    {                                                               \
       .Instance                   = ADC1,                          \
       .Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1,          \
       .Init.Resolution            = ADC_RESOLUTION_12B,            \
       .Init.DataAlign             = ADC_DATAALIGN_RIGHT,           \
       .Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD,    \
       .Init.EOCSelection          = ADC_EOC_SINGLE_CONV,           \
       .Init.LowPowerAutoWait      = DISABLE,                       \
       .Init.LowPowerAutoPowerOff  = DISABLE,                       \
       .Init.ContinuousConvMode    = DISABLE,                       \
       .Init.DiscontinuousConvMode = ENABLE,                        \
       .Init.ExternalTrigConv      = ADC_SOFTWARE_START,            \
       .Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE, \
       .Init.DMAContinuousRequests = ENABLE,                        \
       .Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN,      \
    }
#endif /* ADC1_CONFIG */
#endif /* BSP_USING_ADC1 */
    
#ifdef BSP_ADC1_USING_DMA
#ifndef ADC1_DMA_CONFIG
#define ADC1_DMA_CONFIG                                             \
    {                                                               \
        .Instance = ADC1_DMA_INSTANCE,                                  \
        .Init.Direction = DMA_PERIPH_TO_MEMORY,                     \
        .Init.PeriphInc = DMA_PINC_DISABLE,                         \
        .Init.MemInc = DMA_MINC_ENABLE,                             \
        .Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD,        \
        .Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD,           \
        .Init.Mode = DMA_NORMAL,                                  \
        .Init.Priority = DMA_PRIORITY_HIGH,                         \
    }
#endif /* ADC1_DMA_CONFIG */
#endif /* BSP_USING_ADC1_DMA */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_CONFIG_H__ */
