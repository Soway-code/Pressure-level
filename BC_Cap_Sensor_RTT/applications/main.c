/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     zylx         first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "Picocap_app.h"
#include "dac_bsp.h"

int main(void)
{
    rt_device_t                 pcap_device = RT_NULL;
    rt_device_t                 dac_device  = RT_NULL;
    struct rt_pcap_device_obj   *pcap_device_obj;
    rt_uint32_t                 FilterResult;           //滤波后的结果
    
    pcap_device = rt_device_find(PCAP_DEVICE_NAME);
    if(pcap_device != RT_NULL)
    {
        pcap_device_obj = (struct rt_pcap_device_obj *)pcap_device->user_data;
        rt_device_open(pcap_device, RT_DEVICE_OFLAG_RDWR);
    }
    else
    {
        goto __dev_error;
    }
    
    dac_device = rt_device_find(DAC_DEVICE_NAME);
    if(dac_device != RT_NULL)
    {
        rt_device_open(dac_device, RT_DEVICE_OFLAG_WRONLY);
    }
    else
    {
        goto __dev_error;
    }        

    while (1)
    {
        if(rt_device_read(pcap_device, RESULT_REG1_ADDR, 
                    &pcap_device_obj->PCap_DataConvert_Out.PCap_ResultValue, 1) != 0)
        {
            //数据滤波并判断是否成功
            if(Sensor_DataFilter(&pcap_device_obj->DataFilter, 
                                pcap_device_obj->PCap_DataConvert_Out.PCap_ResultValue, 
                                &FilterResult) == OP_SUCCESS)
            {
                //数值转换
                Sensor_PCap_DataConvert(&pcap_device_obj->PCap_DataConvert, 
                                        FilterResult, 
                                        &pcap_device_obj->PCap_DataConvert_Out);
                //DA转换,使用通道2
                if(pcap_device_obj->PCap_DataConvert.CapDA_ClibEn == DAOUTCLIB_ENABLE)
                {                   
                    rt_device_write(dac_device, 
                                    DA_CHANNEL_2, 
                                    &pcap_device_obj->PCap_DataConvert_Out.PCapDA_ResultValue, 
                                    1);
                }
                else
                {
                    rt_device_write(dac_device, 
                                    DA_CHANNEL_2, 
                                    &pcap_device_obj->PCap_DataConvert_Out.PCapDA_OutValue, 
                                    1);
                }
                
            }
        }
        rt_thread_mdelay(PCAP_COLLECT_CYCLE);
    }
__dev_error:
    while(1)
    {
        rt_thread_mdelay(1000);
    }
}

#define WDT_DEVICE_NAME     "wdt"    /* 看门狗设备名称 */
#define WDT_TIMEOUT         1000    /* 超时时间,单位毫秒 */

static rt_device_t wdg_dev;         /* 看门狗设备句柄 */

static void idle_hook(void)
{
    /* 在空闲线程的回调函数里喂狗 */
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
}

static int wdt_init(void)
{
    rt_err_t ret = RT_EOK;
    rt_uint32_t timeout = WDT_TIMEOUT * (LSI_VALUE / 1000) / 32;    /* 溢出时间 */

    /* 根据设备名称查找看门狗设备，获取设备句柄 */
    wdg_dev = rt_device_find(WDT_DEVICE_NAME);
    if (wdg_dev != RT_NULL)
    {
        /* 初始化设备 */
        ret = rt_device_init(wdg_dev);
        if (ret != RT_EOK)
        {
            return RT_ERROR;
        }
        /* 设置看门狗溢出时间 */
        ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
        if (ret != RT_EOK)
        {
            return RT_ERROR;
        }
        /* 设置空闲线程回调函数 */
        rt_thread_idle_sethook(idle_hook);
    }                

    return ret;
}
INIT_APP_EXPORT(wdt_init);
