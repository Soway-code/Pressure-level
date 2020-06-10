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

  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
#if defined(DAC_CHANNEL_1_ENABLE) 
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
#endif
  
#if defined(DAC_CHANNEL_2_ENABLE)
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

/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS

static struct rt_device dac_device_obj;     //DAC设备对象

/**
* 名称       : stm32_dac_init()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : DAC设备初始化,本函数被rt_device_register注册后,被
*              rt_device_init或rt_device_open调用
* 输入参数   : dev:设备句柄
* 输出参数   : 无
* 返回结果   : RT_EOK:设备初始化成功; 其他:设备初始化失败
* 注意和说明 : 
* 修改内容   :
*/
static rt_err_t stm32_dac_init(rt_device_t dev)
{
    BSP_DAC_Init();
    
    return RT_EOK;
}

/**
* 名称       : stm32_dac_open()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : DAC设备打开,本函数被rt_device_register注册后,被
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
static rt_err_t  stm32_dac_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

/**
* 名称       : stm32_dac_close()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : DAC设备关闭,本函数被rt_device_register注册后,被
*              rt_device_close调用
* 输入参数   : dev:设备句柄;
* 输出参数   : 无
* 返回结果   : RT_EOK:设备关闭成功; 
*              -RT_ERROR:设备已经完全关闭，不能重复关闭设备;
*              其他:设备关闭失败
* 注意和说明 : 这里不使用设备关闭,故只返回一个RT_EOK
* 修改内容   :
*/
static rt_err_t  stm32_dac_close(rt_device_t dev)
{
    return RT_EOK;
}

/**
* 名称       : stm32_dac_write()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : DAC设备写数据,本函数被rt_device_register注册后,被
*              rt_device_write调用
* 输入参数   : dev:设备句柄;
*              pos:写入数据偏移量;
*              buffer:内存缓冲区指针，写入的数据将会被保存在缓冲区中;
*              size:写入数据的大小
* 输出参数   : 无
* 返回结果   : size:写入数据的实际大小，如果是字符设备，返回大小以字节为
               单位，如果是块设备，返回的大小以块为单位;
*              0:需要读取当前线程的 errno 来判断错误状态
* 注意和说明 : 
* 修改内容   :
*/
static rt_size_t stm32_dac_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    uint8_t i;
    
    for(i = pos; i < (size + pos); i++)
    {
        if(i >= DA_CHANNEL_TOTAL)
            return i - pos;
        BSP_DAC_Convert(*(uint16_t *)buffer, (DA_CHANNEL_NUM)i);
    }
    return size;
}

/**
* 名称       : stm32_dac_control()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : DAC设备控制,本函数被rt_device_register注册后,被
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
static rt_err_t  stm32_dac_control(rt_device_t dev, int cmd, void *args)
{    
    return RT_EOK;
}

/**
* 名称       : dac_device_init()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : DAC设备参数初始化,注册ADC设备
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : int整型值
* 注意和说明 : 本函数使用RT-Thread的自动初始化组件INIT_DEVICE_EXPORT
               调用执行,系统复位后自动初始化
* 修改内容   :
*/
static int dac_device_init(void)
{
    dac_device_obj.type     = RT_Device_Class_Miscellaneous;
    dac_device_obj.init     = stm32_dac_init;
    dac_device_obj.open     = stm32_dac_open;
    dac_device_obj.close    = stm32_dac_close;
    dac_device_obj.read     = RT_NULL;
    dac_device_obj.write    = stm32_dac_write;
    dac_device_obj.control  = stm32_dac_control;
    
    dac_device_obj.user_data = &dac_device_obj;
    
    rt_device_register(&dac_device_obj, DAC_DEVICE_NAME, 
                        RT_DEVICE_FLAG_WRONLY
                        | RT_DEVICE_FLAG_STANDALONE);
    
    return RT_EOK;
}
INIT_DEVICE_EXPORT(dac_device_init);

#endif

