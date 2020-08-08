/**@file        adc_app.c
* @brief        ADCģ���Ӧ��
* @details      ADC DMA �������� ADC�¶����ݻ�ȡ�봦��
* @author       ���
* @date         2020-05-06
* @version      V1.0.1
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/07/06  <td>1.0.1    <td>���    <td>���L0ϵ��֧�ֳ���
* </table>
*
**********************************************************************************
*/

#include "adc_app.h"
#include "string.h"


static uint16_t gADC_GetData[AD_GROUP_MAX][AD_CHANNEL_MAX];     ///< ADC��DMA���ݻ�����
static uint16_t uADC_GetResult[AD_GROUP_MAX][AD_CHANNEL_MAX];   ///< ADC�������,�������ݴ���

static uint8_t ADC_Updata_Flag = 0;                             ///< ADC���ݸ��±�־


#ifdef __IN_MEMORY_APP_H

/**@brief       ��ʼ���¶�ת����Ҫ�Ĳ����ṹ
* @param[out]   ADC_TemperParam : �¶�ת����Ҫ�Ĳ����ṹָ��; 
* @return       ����ִ�н��
* - None
* @note         Ҫʹ�ñ�����,Ҫ����In_Memory_app.c��In_Memory_app.h��
* In_Flash.c��In_Flash.h�ļ�(STM32L0ϵ�������In_EEPROM.c��In_EEPROM.h�ļ�)
*/
void Sensor_ADC_TemperParam_Init(ADC_TemperParam_TypeDef *ADC_TemperParam)
{
    uint8_t Read_Data[4];
    
    InMemory_Read_MultiBytes(TEMPER_K1, Read_Data, 2);
    ADC_TemperParam->Temper_K1 = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    ADC_TemperParam->Temper_K1 /= 100;
    
    InMemory_Read_MultiBytes(TEMPER_B1, Read_Data, 2);
    ADC_TemperParam->Temper_B1 = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    ADC_TemperParam->Temper_B1 -= 100;
    
    InMemory_Read_MultiBytes(TEMPER_K2, Read_Data, 2);
    ADC_TemperParam->Temper_K2 = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    ADC_TemperParam->Temper_K2 /= 100;
    
    InMemory_Read_MultiBytes(TEMPER_B2, Read_Data, 2);
    ADC_TemperParam->Temper_B2 = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    ADC_TemperParam->Temper_B2 -= 100;
    
    InMemory_Read_MultiBytes(TEMPDAMIN, Read_Data, 2);
    ADC_TemperParam->TempDAMin = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InMemory_Read_MultiBytes(TEMPDAMAX, Read_Data, 2);
    ADC_TemperParam->TempDAMax = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];   
    
    ADC_TemperParam->TempDARange = ADC_TemperParam->TempDAMax
                                    - ADC_TemperParam->TempDAMin;
}

#else

/**@brief       ��ʼ���¶�ת����Ҫ�Ĳ����ṹ
* @param[in]    Param : Ҫд��ADC_TemperParam�Ĳ�������ָ��;
* @param[out]   ADC_TemperParam : �¶�ת����Ҫ�Ĳ����ṹָ��; 
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         Param����������(���ֽ�˳������): 0~1,�¶�1����ϵ��K1; \n
* 2~3,�¶�1����ϵ��B1; 4~5,�¶�2����ϵ��K2; 6~7,�¶�2����ϵ��B2; \n
* 8~9,�¶�DAת�����ֵ; 10~11,�¶�DAת������ֵ;
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
#endif  //__IN_MEMORY_APP_H

/**@brief       ��ʼ��ADC�����stm32оƬ�ڲ��¶ȴ��������¶ȱ仯б�ʣ�����ADC��DMA����
* @return       ����ִ�н��
* - None
*/
void Sensor_ADC_Init(void)
{    
    //ADC������ʼ��
    BSP_ADC_Init();
    //��ʼADC DMA����
    BSP_ADC_Start_DMA((uint32_t *)gADC_GetData,  AD_SEQBUFF_MAX);
}

/**@brief       �û���ȡADCͨ����ƽ���˲��������
* @param[in]    Channel_Num : ͨ����,ָ����ȡ��ͨ��;
* @return       ����ִ�н��
* - ƽ���˲����ADCֵ
*/
uint16_t Sensor_ADC_GetChn_Value(AD_CHANNEL_NUM AD_Channel_Num)
{
    uint8_t cn;
    uint32_t AD_Sum = 0;

    if(AD_Channel_Num >= AD_CHANNEL_MAX)
        return 0;
    
    for(cn = 0; cn < AD_GROUP_MAX; cn++)
    {
        //��ͨ�������������ۼ�
        AD_Sum += uADC_GetResult[cn][AD_Channel_Num];
    }
    //����ƽ��ֵ
    return AD_Sum / AD_GROUP_MAX;
}

/**@brief       ��ȡADC�¶�����
* @return       ����ִ�н��
* - �¶�ֵ
* @note         �¶ȼ���ɲο�STM32F072�����ֲ�
*/
float Sensor_ADC_Get_TemperData(void)
{
    uint8_t cn;
    float Result;
    uint32_t AD_Sum = 0;
    uint16_t AD_Average;

    for(cn = 0; cn < AD_GROUP_MAX; cn++)
    {
        //���¶�ͨ�������������ۼ�
        AD_Sum += uADC_GetResult[cn][AD_CHANNEL_TEMPSENSOR];
    }
    //����ƽ��ֵ
    AD_Average = AD_Sum / AD_GROUP_MAX;
    
#if defined(STM32F0)
    /* ����õ��¶�ADCֵת����ʵ���¶� */
    Result = (AD_Average - (int32_t) *VREF30ADDR );
    Result = Result * (int32_t)(110 - 30);
    Result = Result / (int32_t)(*VREF110ADDR - *VREF30ADDR);
    Result = Result + 30;
#elif defined(STM32L0)
    /* ����õ��¶�ADCֵת����ʵ���¶�, ����ο���оƬ�ο��ֲ�Ĵ���ʾ����д�� */
    Result = ((AD_Average * VDD_APPLI / VDD_CALIB) - (int32_t) *VREF30ADDR );
    Result = Result * (int32_t)(130 - 30);
    Result = Result / (int32_t)(*VREF130ADDR - *VREF30ADDR);
    Result = Result + 30;
#endif // defined(STM32F0) or defined(STM32L0)
    
    return Result;
}

/**@brief       ��ȡADC���ݸ��±�־
* @return       ����ִ�н��
* - ADC���±�־
*/
uint8_t Sensor_ADC_Get_Updata_Flag(void)
{
    return ADC_Updata_Flag;
}

/**@brief       ���ADC���±�־
* @return       ����ִ�н��
* - None
*/
void Sensor_ADC_Clean_Updata_Flag(void)
{    
    ADC_Updata_Flag = UPDATA_NONE;
    BSP_ADC_DMA_Enable_IT();
}

/**@brief       ADCת����ɻص�,������ֹͣADCת��,����DMA���ݵ�
* �������uADC_GetResult��
* @param[in]    hadc : ADC�������ָ��
* @return       ����ִ�н��
* - None
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{    
    BSP_ADC_DMA_Disable_IT();
    memcpy(uADC_GetResult, gADC_GetData, sizeof(uADC_GetResult));
    
    ADC_Updata_Flag = UPDATA_OK;
}

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS

#define ADC_THREAD_STACK        512
#define ADC_THREAD_PRIORITY     RT_THREAD_PRIORITY_MAX - 2

/**@struct ADC�豸���� */
static struct rt_adc_device_obj adc_device_obj;     

/**@brief       ADC�豸��ʼ��,��������rt_device_registerע���,��
* rt_device_init��rt_device_open����
* @param[in]    dev : �豸���
* @return       ����ִ�н��
* - RT_EOK : �豸��ʼ���ɹ�; 
* - ���� : �豸��ʼ��ʧ��
*/
static rt_err_t stm32_adc_init(rt_device_t dev)
{    
    Sensor_ADC_Init();
    
    return RT_EOK;
}

/**@brief       ADC�豸��,��������rt_device_registerע���,��
* rt_device_open����
* @param[in]    dev : �豸���; 
* @param[in]    oflag : �豸����ģʽ��־
* @return       ����ִ�н��
* - RT_EOK : �豸�򿪳ɹ�; 
* - -RT_EBUSY : ����豸ע��ʱָ���Ĳ����а��� RT_DEVICE_FLAG_STANDALONE 
* ���������豸���������ظ���;
* - ����:�豸��ʧ��
*/
static rt_err_t  stm32_adc_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

/**@brief       ADC�豸�ر�,��������rt_device_registerע���,��
* rt_device_close����
* @param[in]    dev : �豸���; 
* @return       ����ִ�н��
* - RT_EOK:�豸�رճɹ�; 
* - -RT_ERROR:�豸�Ѿ���ȫ�رգ������ظ��ر��豸;
* - ����:�豸�ر�ʧ��
* @note         ���ﲻʹ���豸�ر�,��ֻ����һ��RT_EOK
*/
static rt_err_t  stm32_adc_close(rt_device_t dev)
{
    return RT_EOK;
}

/**@brief       ADC�豸������,��������rt_device_registerע���,��
* rt_device_read����
* @param[in]    dev : �豸���;
* @param[in]    pos : ��ȡ����ƫ����;
* @param[out]   buffer : �ڴ滺����ָ�룬��ȡ�����ݽ��ᱻ�����ڻ�������;
* @param[in]    size : ��ȡ���ݵĴ�С
* @return       ����ִ�н��
* - size:�������ݵ�ʵ�ʴ�С��������ַ��豸�����ش�С���ֽ�Ϊ
* ��λ������ǿ��豸�����صĴ�С�Կ�Ϊ��λ;
* - 0:��Ҫ��ȡ��ǰ�̵߳� errno ���жϴ���״̬
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

/**@brief       ADC�豸����,��������rt_device_registerע���,��
* rt_device_control����
* @param[in]    dev:�豸���;
* @param[in]    cmd:��������֣��������ͨ�����豸�����������;
* @param[in]    arg:���ƵĲ���
* @return       ����ִ�н��
* - RT_EOK:����ִ�гɹ�;
* - -RT_ENOSYS:ִ��ʧ�ܣ�dev Ϊ��;
* - ����������:ִ��ʧ��
* @note         ���ﲻʹ���豸����,��ֻ����һ��RT_EOK
*/
static rt_err_t  stm32_adc_control(rt_device_t dev, int cmd, void *args)
{
    return RT_EOK;
}

/**@brief       ADC�߳�,���ڶ�ȡADC���µ�����
* @param[in]    parameter : �̵߳Ĳ���,���ﲻʹ��
* @return       ����ִ�н��
* - None
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
        //�ж�ADC�Ƿ񱻸���
        if(Sensor_ADC_Get_Updata_Flag() == UPDATA_OK && adc_device != RT_NULL)
        {
            //��ȡADת�������¶�ֵ
            rt_device_read(adc_device, AD_CHANNEL_TEMPSENSOR, 
                            &adc_obj->ADC_TemperOut.TemperInAir, 1);
            //���ADC���±�־,����ADCת��
            Sensor_ADC_Clean_Updata_Flag();
        }
        rt_thread_mdelay(10);
    }
}

/**@brief       ADC�豸������ʼ��,ע��ADC�豸,����ADC�̲߳�����
* @return       ����ִ�н��
* - int����ֵ(RT_EOK)
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
    
    Sensor_ADC_TemperParam_Init(&adc_device_obj.ADC_TemperParam);   // ADC�¶ȴ��������ʼ��    
    rt_device_register(&adc_device_obj.dev, ADC_DEVICE_NAME,        // ע��ADC�豸
                        RT_DEVICE_FLAG_RDONLY
                        | RT_DEVICE_FLAG_STANDALONE);
    /* ���� ADC �����߳� */
    adc_thread = rt_thread_create("adc",
                                    adc_thread_entry,
                                    RT_NULL,
                                    ADC_THREAD_STACK,
                                    ADC_THREAD_PRIORITY,
                                    20);
    RT_ASSERT(adc_thread != RT_NULL);
    // ���� ADC �����߳�
    rt_thread_startup(adc_thread);
    
    return RT_EOK;
}
INIT_DEVICE_EXPORT(adc_device_init);

#endif // USING_RT_THREAD_OS


