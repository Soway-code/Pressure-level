/****************************************************************************************************************************************************************
** 版权:          2020-2030,深圳市信为科技发展有限公司
** 文件名:        adc_app.c
** 作者:          杨春林
** 版本:          V1.0.0
** 日期:          2020-05-06
** 描述:          ADC模块的应用
** 功能:          ADC DMA 功能配置 ADC温度数据获取与处理
*****************************************************************************************************************************************************************
** 修改者:        No
** 版本:          No
** 修改内容:      No
** 日期:          No
****************************************************************************************************************************************************************/

#include "adc_app.h"
#include "string.h"


static uint16_t Temp30_Vref_Adc;                                //30摄氏度ADC基准值
static uint16_t Temp110_Vref_Adc;                               //110摄氏度ADC基准值
static float TempVarySlope;                                     //温度变化斜率
static uint16_t gADC_GetData[AD_GROUP_MAX][AD_CHANNEL_MAX];     //ADC的DMA数据缓存区
static uint16_t uADC_GetResult[AD_GROUP_MAX][AD_CHANNEL_MAX];   //ADC结果缓存,用于数据处理

static uint8_t ADC_Updata_Flag = 0;                             //ADC数据更新标志


#ifdef __IN_FLASH_APP_H
/**
* 名称       : Sensor_ADC_TemperParam_Init()
* 创建日期   : 2020-05-06
* 作者       : 杨春林
* 功能       : 初始化温度转换需要的参数结构
* 输入参数   : 无
* 输出参数   : ADC_TemperParam,温度转换需要的参数结构指针;
* 返回结果   : 无
* 注意和说明 : 要使用本函数,要加入In_Flash_app.c、In_Flash_app.h、
*              In_Flash.ch、In_Flash.h和文件
* 修改内容   :
*/
void Sensor_ADC_TemperParam_Init(ADC_TemperParam_TypeDef *ADC_TemperParam)
{
    uint8_t Read_Data[4];
    
    InFlash_Read_MultiBytes(TEMPER_K1, Read_Data, 2);
    ADC_TemperParam->Temper_K1 = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InFlash_Read_MultiBytes(TEMPER_B1, Read_Data, 2);
    ADC_TemperParam->Temper_B1 = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InFlash_Read_MultiBytes(TEMPER_K2, Read_Data, 2);
    ADC_TemperParam->Temper_K2 = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InFlash_Read_MultiBytes(TEMPER_B2, Read_Data, 2);
    ADC_TemperParam->Temper_B2 = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InFlash_Read_MultiBytes(TEMPDAMIN, Read_Data, 2);
    ADC_TemperParam->TempDAMin = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InFlash_Read_MultiBytes(TEMPDAMAX, Read_Data, 2);
    ADC_TemperParam->TempDAMax = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];   
    
    ADC_TemperParam->TempDARange = ADC_TemperParam->TempDAMax
                                    - ADC_TemperParam->TempDAMin;
}

#else
/*
* 名称       : Sensor_ADC_TemperParam_Init()
* 创建日期   : 2020-05-06
* 作者       : 杨春林
* 功能       : 初始化温度转换需要的参数结构
* 输入参数   : Param, 要写入ADC_TemperParam的参数数组指针;
*              Param_Size, Param的大小;
* 输出参数   : ADC_TemperParam,温度转换需要的参数结构指针;
* 返回结果   : OP_SUCCESS(成功)，OP_FAILED(失败)
* 注意和说明 : Param的数据内容(按字节顺序排列): 0~1,温度1修正系数K1; 
*              2~3,温度1修正系数B1; 4~5,温度2修正系数K2; 6~7,温度2修正系数B2;
*              8~9,温度DA转换零点值; 10~11,温度DA转换满点值;
* 修改内容   :
*/
uint8_t Sensor_ADC_TemperParam_Init(ADC_TemperParam_TypeDef *ADC_TemperParam, uint8_t *Param, 
                            uint16_t Param_Size)
{
    uint8_t cnt = 0;

    if(Param != NULL && Param_Size != 0)
    {
        ADC_TemperParam->Temper_K1 = (uint16_t)(Param[cnt] << 8) | Param[cnt + 1];   
        cnt += 2;
        ADC_TemperParam->Temper_B1 = (uint16_t)(Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        ADC_TemperParam->Temper_K2 = (uint16_t)(Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        ADC_TemperParam->Temper_B2 = (uint16_t)(Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        ADC_TemperParam->TempDAMin = (uint16_t)(Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        ADC_TemperParam->TempDAMax = (uint16_t)(Param[cnt] << 8) | Param[cnt + 1]; 
        
        if(cnt > Param_Size)
        {
            return OP_FAILED;
        }
        return OP_SUCCESS;
    }
    return OP_FAILED;
}
#endif

/**
* 名称       : Sensor_ADC_Init()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : 初始化ADC，求出stm32芯片内部温度传感器的温度变化斜率，启动ADC的DMA传输
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Sensor_ADC_Init(void)
{    
    //ADC驱动初始化
    BSP_ADC_Init();
    //读取30摄氏度ADC基准值
    Temp30_Vref_Adc = *(__IO uint16_t *)VREF30ADDR;
    //读取110摄氏度ADC基准值
    Temp110_Vref_Adc = *(__IO uint16_t *)VREF110ADDR;
    //温度变化斜率计算
    TempVarySlope = 80.0 / (float)(Temp110_Vref_Adc - Temp30_Vref_Adc);
    //开始ADC DMA传输
    BSP_ADC_Start_DMA((uint32_t *)&gADC_GetData[0][0],  AD_SEQBUFF_MAX);
}

/**
* 名称       : Sensor_ADC_GetChn_Value()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : 用户获取ADC通道上平均滤波后的数据
* 输入参数   : Channel_Num:通道号,指定获取的通道
* 输出参数   : 无
* 返回结果   : 平均滤波后的结果
* 注意和说明 : 
* 修改内容   :
*/
uint16_t Sensor_ADC_GetChn_Value(AD_CHANNEL_NUM AD_Channel_Num)
{
    uint8_t cn;
    uint32_t AD_Sum = 0;

    if(AD_Channel_Num >= AD_CHANNEL_MAX)
        return 0;
    
    for(cn = 0; cn < AD_GROUP_MAX; cn++)
    {
        //对通道的所有数据累加
        AD_Sum += uADC_GetResult[cn][AD_Channel_Num];
    }
    //计算平均值
    return AD_Sum / AD_GROUP_MAX;
}

/**
* 名称       : Sensor_ADC_Get_TemperData()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : 获取ADC温度数据
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 温度值
* 注意和说明 : 温度计算可参考STM32F072数据手册
* 修改内容   :
*/
float Sensor_ADC_Get_TemperData(void)
{
    uint8_t cn;
    float Result;
    uint32_t AD_Sum = 0;
    uint16_t AD_Average;

    for(cn = 0; cn < AD_GROUP_MAX; cn++)
    {
        //对温度通道的所有数据累加
        AD_Sum += uADC_GetResult[cn][AD_CHANNEL_TEMPSENSOR];
    }
    //计算平均值
    AD_Average = AD_Sum / AD_GROUP_MAX;
    //根据温度变化斜率得到实际温度
    Result = TempVarySlope * (AD_Average - Temp30_Vref_Adc) + 30;
    
    return Result;
}

/**
* 名称       : Sensor_ADC_Get_Updata_Flag()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : 获取ADC数据更新标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : ADC更新标志
* 注意和说明 : 
* 修改内容   :
*/
uint8_t Sensor_ADC_Get_Updata_Flag(void)
{
    return ADC_Updata_Flag;
}

/**
* 名称       : Sensor_ADC_Clean_Updata_Flag()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : 清除ADC更新标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Sensor_ADC_Clean_Updata_Flag(void)
{
    BSP_ADC_Conver_Start();
    ADC_Updata_Flag = UPDATA_NONE;
}

/**
* 名称       : HAL_ADC_ConvCpltCallback()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : ADC转换完成回调，这里仅置位ADC完成标志
* 输入参数   : ADC处理对象指针
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{    
    BSP_ADC_Conver_Stop();
    memcpy(uADC_GetResult, gADC_GetData, sizeof(uADC_GetResult));
    
    ADC_Updata_Flag = UPDATA_OK;
}

/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS

#define ADC_THREAD_STACK        512
#define ADC_THREAD_PRIORITY     RT_THREAD_PRIORITY_MAX - 2

static struct rt_adc_device_obj adc_device_obj;     //ADC设备对象

/**
* 名称       : stm32_adc_init()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : ADC设备初始化,本函数被rt_device_register注册后,被
*              rt_device_init或rt_device_open调用
* 输入参数   : dev:设备句柄
* 输出参数   : 无
* 返回结果   : RT_EOK:设备初始化成功; 其他:设备初始化失败
* 注意和说明 : 
* 修改内容   :
*/
static rt_err_t stm32_adc_init(rt_device_t dev)
{    
    Sensor_ADC_Init();
    
    return RT_EOK;
}

/**
* 名称       : stm32_adc_open()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : ADC设备打开,本函数被rt_device_register注册后,被
*              rt_device_open调用
* 输入参数   : dev:设备句柄; oflag:设备访问模式标志
* 输出参数   : 无
* 返回结果   : RT_EOK:设备打开成功; 
*              -RT_EBUSY:如果设备注册时指定的参数中包括 
               RT_DEVICE_FLAG_STANDALONE 参数，此设备将不允许重复打开;
*              其他:设备打开失败
* 注意和说明 : 这里不使用设备打开,故只返回一个RT_EOK
* 修改内容   :
*/
static rt_err_t  stm32_adc_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

/**
* 名称       : stm32_adc_close()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : ADC设备关闭,本函数被rt_device_register注册后,被
*              rt_device_close调用
* 输入参数   : dev:设备句柄;
* 输出参数   : 无
* 返回结果   : RT_EOK:设备关闭成功; 
*              -RT_ERROR:设备已经完全关闭，不能重复关闭设备;
*              其他:设备关闭失败
* 注意和说明 : 这里不使用设备关闭,故只返回一个RT_EOK
* 修改内容   :
*/
static rt_err_t  stm32_adc_close(rt_device_t dev)
{
    return RT_EOK;
}

/**
* 名称       : stm32_adc_read()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : ADC设备读数据,本函数被rt_device_register注册后,被
*              rt_device_read调用
* 输入参数   : dev:设备句柄;
*              pos:读取数据偏移量;
*              buffer:内存缓冲区指针，读取的数据将会被保存在缓冲区中;
*              size:读取数据的大小
* 输出参数   : 无
* 返回结果   : size:读到数据的实际大小，如果是字符设备，返回大小以字节为
               单位，如果是块设备，返回的大小以块为单位;
*              0:需要读取当前线程的 errno 来判断错误状态
* 注意和说明 : 
* 修改内容   :
*/
static rt_size_t stm32_adc_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    uint8_t i;
    uint16_t *adc_buf;
    
    if(pos == AD_CHANNEL_TEMPSENSOR)
    {
        *(float *)buffer = Sensor_ADC_Get_TemperData();
        return 1;
    }
    else
    {
        adc_buf = (uint16_t *)buffer;
        for(i = pos; i < (size + pos); i++)
        {
            if(i >= AD_CHANNEL_TEMPSENSOR)
                return i - pos;
            *adc_buf = Sensor_ADC_GetChn_Value((AD_CHANNEL_NUM)i);
            adc_buf += 2;            
        }
    }
    return size;
}

/**
* 名称       : stm32_adc_control()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : ADC设备控制,本函数被rt_device_register注册后,被
*              rt_device_control调用
* 输入参数   : dev:设备句柄;
*              cmd:命令控制字，这个参数通常与设备驱动程序相关;
*              arg:控制的参数
* 输出参数   : 无
* 返回结果   : RT_EOK:函数执行成功;
*              -RT_ENOSYS:执行失败，dev 为空;
*              其他错误码:执行失败
* 注意和说明 : 这里不使用设备控制,故只返回一个RT_EOK
* 修改内容   :
*/
static rt_err_t  stm32_adc_control(rt_device_t dev, int cmd, void *args)
{
    return RT_EOK;
}

/**
* 名称       : adc_thread_entry()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : ADC线程,用于读取ADC更新的数据
* 输入参数   : parameter:线程的参数,这里不使用
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void adc_thread_entry(void *parameter)
{
    rt_device_t adc_device;
    struct rt_adc_device_obj *adc_obj;
    
    adc_device = rt_device_find(ADC_DEVICE_NAME);
    if(adc_device != RT_NULL)
    {
        adc_obj = (struct rt_adc_device_obj *)adc_device->user_data;
        rt_device_open(adc_device, RT_DEVICE_OFLAG_RDONLY);
    }
    
    while(1)
    {
        //判断ADC是否被更新
        if(Sensor_ADC_Get_Updata_Flag() == UPDATA_OK)
        {
            //获取AD转换出的温度值
            rt_device_read(adc_device, AD_CHANNEL_TEMPSENSOR, 
                            &adc_obj->ADC_TemperOut.TemperInAir, 1);
            //清除ADC更新标志,并打开ADC转换
            Sensor_ADC_Clean_Updata_Flag();
        }
        rt_thread_mdelay(10);
    }
}

/**
* 名称       : adc_device_init()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : ADC设备参数初始化,注册ADC设备,创建ADC线程并启动
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : int整型值
* 注意和说明 : 本函数使用RT-Thread的自动初始化组件INIT_DEVICE_EXPORT
               调用执行,系统复位后自动初始化
* 修改内容   :
*/
static int adc_device_init(void)
{
    rt_thread_t adc_thread;
    
    adc_device_obj.dev.type     = RT_Device_Class_Miscellaneous;
    adc_device_obj.dev.init     = stm32_adc_init;
    adc_device_obj.dev.open     = stm32_adc_open;
    adc_device_obj.dev.close    = stm32_adc_close;
    adc_device_obj.dev.read     = stm32_adc_read;
    adc_device_obj.dev.write    = RT_NULL;
    adc_device_obj.dev.control  = stm32_adc_control;    
    
    adc_device_obj.dev.user_data = &adc_device_obj;
    
    Sensor_ADC_TemperParam_Init(&adc_device_obj.ADC_TemperParam);
    
    rt_device_register(&adc_device_obj.dev, ADC_DEVICE_NAME, 
                        RT_DEVICE_FLAG_RDONLY
                        | RT_DEVICE_FLAG_STANDALONE);
    
    adc_thread = rt_thread_create("adc",
                                    adc_thread_entry,
                                    RT_NULL,
                                    ADC_THREAD_STACK,
                                    ADC_THREAD_PRIORITY,
                                    20);
    RT_ASSERT(adc_thread != RT_NULL);
    rt_thread_startup(adc_thread);
    
    return RT_EOK;
}
INIT_DEVICE_EXPORT(adc_device_init);

#endif


