/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-7      SummerGift   first version
 */

#include "drv_common.h"

#ifdef RT_USING_SERIAL
#include "drv_usart.h"
#else
#include <board.h>
#include <string.h>
#endif

#ifdef RT_USING_FINSH
#include <finsh.h>
static void reboot(uint8_t argc, char **argv)
{
    rt_hw_cpu_reset();
}
FINSH_FUNCTION_EXPORT_ALIAS(reboot, __cmd_reboot, Reboot System);
#endif /* RT_USING_FINSH */

/* SysTick configuration */
void rt_hw_systick_init(void)
{
    HAL_SYSTICK_Config(SystemCoreClock / RT_TICK_PER_SECOND);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_IncTick();
    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

uint32_t HAL_GetTick(void)
{
    return rt_tick_get();
}

void HAL_SuspendTick(void)
{
}

void HAL_ResumeTick(void)
{
}

void HAL_Delay(uint32_t Delay)
{
    rt_thread_mdelay(Delay);
}

/* re-implement tick interface for STM32 HAL */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char *s, int num)
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
    {
    }
    /* USER CODE END Error_Handler */
}

/**
 * This function will delay for some us.
 *
 * @param us the delay time of us
 */
void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t start, now, delta, reload, us_tick;
    start = SysTick->VAL;
    reload = SysTick->LOAD;
    us_tick = SystemCoreClock / 1000000UL;
    do {
        now = SysTick->VAL;
        delta = start > now ? start - now : reload + start - now;
    } while(delta < us_tick * us);
}

#if   (defined ( __CC_ARM ))
__IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
#elif (defined (__ICCARM__))
#pragma location = 0x20000000
__no_init __IO uint32_t VectorTable[48];
#elif defined   (  __GNUC__  )
__IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
#endif

/**
 * This function will initial STM32 board.
 */
RT_WEAK void rt_hw_board_init()
{
    /* F0系列没有中断向量表重映射寄存器，所以，这里使用另一种方法：拷贝中断向量表到
    0x20000000开始的内存空间中，再将这个内存地址到0x00000000，完成中断向量表的重映射 */
    memcpy((void*)VectorTable, (void*)APPLICATION_ADDRESS, VECTOR_TABLE_SIZE); 
    /* 将系统内存映射到0x00000000 */
    __HAL_SYSCFG_REMAPMEMORY_SRAM();
#ifdef SCB_EnableICache
    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();
#endif

#ifdef SCB_EnableDCache
    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();
#endif

    /* HAL_Init() function is called at the beginning of the program */
    HAL_Init();

    /* System clock initialization */
    SystemClock_Config();
    rt_hw_systick_init();

    /* Heap initialization */
#if defined(RT_USING_HEAP)
    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif

    /* Pin driver initialization is open by default */
#ifdef RT_USING_PIN
    rt_hw_pin_init();
#endif

    /* USART driver initialization is open by default */
#ifdef RT_USING_SERIAL
    rt_hw_usart_init();
#endif

    /* Set the shell console output device */
#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

    /* Board underlying hardware initialization */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
}

