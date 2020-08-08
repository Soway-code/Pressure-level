/**@file        dac_bsp.c
* @brief        DAC����
* @details      DAC ͨ������, ��������, DACת��
* @author       ���
* @date         2020-04-27
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/04/27  <td>1.0.0    <td>���    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#include "dac_bsp.h"
#include "VariaType.h"


static DAC_HandleTypeDef hdac;      ///< DAC�������


/**@brief       DAC��ʼ��
* @return       ����ִ�н��
* - None
* @note         DACͨ��ͨ��DAC_CHANNEL_1_ENABLE��DAC_CHANNEL_2_ENABLE�궨���
*/
void BSP_DAC_Init(void)
{
#if defined(DAC_CHANNEL_1_ENABLE) || defined(DAC_CHANNEL_2_ENABLE)
    
  DAC_ChannelConfTypeDef sConfig = {0};

  hdac.Instance = DACx;
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
#endif // defined(DAC_CHANNEL_1_ENABLE) 
  
#if defined(DAC_CHANNEL_2_ENABLE)
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
#endif // defined(DAC_CHANNEL_2_ENABLE)
  
#endif // defined(DAC_CHANNEL_1_ENABLE) || defined(DAC_CHANNEL_2_ENABLE)
}

/**@brief       DACʱ�ӡ��ײ�ܽų�ʼ��(��HAL_DAC_Init�ص�����)
* @param[in]    dacHandle : DAC�������
* @return       ����ִ�н��
* - None
* @note         DACͨ��ͨ��DAC_CHANNEL_1_ENABLE��DAC_CHANNEL_2_ENABLE�궨���
*/
void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle)
{
#if defined(DAC_CHANNEL_1_ENABLE) || defined(DAC_CHANNEL_2_ENABLE)

    GPIO_InitTypeDef GPIO_InitStruct = {0};


    DACx_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
#if defined(DAC_CHANNEL_1_ENABLE)
    GPIO_InitStruct.Pin |= DACx_CHANNEL_PIN;
    HAL_GPIO_Init(DACx_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);
#endif // defined(DAC_CHANNEL_1_ENABLE)
    
#if defined(DAC_CHANNEL_2_ENABLE)
    GPIO_InitStruct.Pin |= DACx_CHANNEL2_PIN;
    HAL_GPIO_Init(DACx_CHANNEL2_GPIO_PORT, &GPIO_InitStruct);
#endif // defined(DAC_CHANNEL_2_ENABLE)
    

    /* USER CODE BEGIN DAC_MspInit 1 */

    /* USER CODE END DAC_MspInit 1 */
  
#endif // defined(DAC_CHANNEL_1_ENABLE) || defined(DAC_CHANNEL_2_ENABLE)
}

/**@brief       DACʱ�ӡ��ײ�ܽŷ���ʼ��(��HAL_DAC_DeInit�ص�����)
* @param[in]    dacHandle : DAC�������
* @return       ����ִ�н��
* - None
* @note         DACͨ��ͨ��DAC_CHANNEL_1_ENABLE��DAC_CHANNEL_2_ENABLE�궨���
*/
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle)
{
#if defined(DAC_CHANNEL_1_ENABLE) || defined(DAC_CHANNEL_2_ENABLE)

    /* USER CODE BEGIN DAC_MspDeInit 0 */

    /* USER CODE END DAC_MspDeInit 0 */
    /* Peripheral clock disable */
    DACx_CLK_DISABLE();

#if defined(DAC_CHANNEL_1_ENABLE)
    HAL_GPIO_DeInit(DACx_CHANNEL1_GPIO_PORT, DACx_CHANNEL1_PIN);
#endif // defined(DAC_CHANNEL_1_ENABLE)
    
#if defined(DAC_CHANNEL_2_ENABLE)
    HAL_GPIO_DeInit(DACx_CHANNEL2_GPIO_PORT, DACx_CHANNEL2_PIN);
#endif // defined(DAC_CHANNEL_2_ENABLE)


    /* USER CODE BEGIN DAC_MspDeInit 1 */

    /* USER CODE END DAC_MspDeInit 1 */
  
#endif // defined(DAC_CHANNEL_1_ENABLE) || defined(DAC_CHANNEL_2_ENABLE)
} 

/**@brief       ָ��DACͨ��д��DAֵ
* @param[in]    DA_Value : DACֵ
* @param[in]    DA_Channel_Num : DACͨ����
* @return       ����ִ�н��
* - None
* @note         DAֵ���ܳ���4095; ͨ���ż�DA_CHANNEL_NUM�Ķ���,
* ��ֵֻ��ȡDA_CHANNEL_NUM�����ֵ
*/
uint8_t BSP_DAC_Convert(uint16_t DA_Value, DA_CHANNEL_NUM DA_Channel_Num)
{
  uint32_t channel;
  //DACֵ������Χ
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

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS

static struct rt_device dac_device_obj;     //DAC�豸����


/**@brief       DAC�豸��ʼ��,��������rt_device_registerע���,
* ��rt_device_init��rt_device_open����
* @param[in]    dev : �豸���
* @return       ����ִ�н��
* - RT_EOK: �豸��ʼ���ɹ�; 
*/
static rt_err_t stm32_dac_init(rt_device_t dev)
{
    BSP_DAC_Init();
    
    return RT_EOK;
}

/**@brief       DAC�豸��,��������rt_device_registerע���,
* ��rt_device_open����
* @param[in]    dev : �豸���
* @param[in]    oflag : �豸����ģʽ��־
* @return       ����ִ�н��
* - RT_EOK: �豸�򿪳ɹ�; 
* @note         ���ﲻʹ���豸��,��ֻ����һ��RT_EOK
*/
static rt_err_t  stm32_dac_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

/**@brief       DAC�豸�ر�,��������rt_device_registerע���,
* ��rt_device_close����
* @param[in]    dev : �豸���
* @param[in]    oflag : �豸����ģʽ��־
* @return       ����ִ�н��
* - RT_EOK: �豸�رճɹ�; 
* @note         ���ﲻʹ���豸�ر�,��ֻ����һ��RT_EOK
*/
static rt_err_t  stm32_dac_close(rt_device_t dev)
{
    return RT_EOK;
}

/**@brief       DAC�豸д����,��������rt_device_registerע���,
* ��rt_device_write����
* @param[in]    dev : �豸���
* @param[in]    pos : д������ƫ����;
* @param[in]    buffer : �ڴ滺����ָ�룬д������ݽ��ᱻ�����ڻ�������;
* @param[in]    size : д�����ݵĴ�С
* @return       ����ִ�н��
* - size: д�����ݵ�ʵ�ʴ�С��������ַ��豸�����ش�С���ֽ�Ϊ
* ��λ������ǿ��豸�����صĴ�С�Կ�Ϊ��λ;
* - 0: ��Ҫ��ȡ��ǰ�̵߳� errno ���жϴ���״̬
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

/**@brief       DAC�豸����,��������rt_device_registerע���,��
* rt_device_control����
* @param[in]    dev : �豸���
* @param[in]    cmd : ��������֣��������ͨ�����豸�����������;
* @param[in]    arg : ���ƵĲ���
* @return       ����ִ�н��
* - RT_EOK:����ִ�гɹ�;
* @note         ���ﲻʹ���豸����,��ֻ����һ��RT_EOK
*/
static rt_err_t  stm32_dac_control(rt_device_t dev, int cmd, void *args)
{    
    return RT_EOK;
}

/**@brief       DAC�豸������ʼ��,ע��ADC�豸
* @return       ����ִ�н��
* - int����ֵ(RT_EOK)
* @note         ������ʹ��RT-Thread���Զ���ʼ�����INIT_DEVICE_EXPORT
* ����ִ��,ϵͳ��λ���Զ���ʼ��
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
    
    rt_device_register(&dac_device_obj, DAC_DEVICE_NAME,    // ע��DAC�豸
                        RT_DEVICE_FLAG_WRONLY
                        | RT_DEVICE_FLAG_STANDALONE);
    
    return RT_EOK;
}
INIT_DEVICE_EXPORT(dac_device_init);

#endif // USING_RT_THREAD_OS

