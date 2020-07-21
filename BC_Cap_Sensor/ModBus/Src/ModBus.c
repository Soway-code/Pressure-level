/**@file        Modbus.c
* @brief        Modbus RTU ͨ��Э��
* @details      ��д�������Ĵ����궨���ܡ��ɼ����ݽ���
* @author       ׯ��Ⱥ
* @date         2020-07-20
* @version      V2.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2020/07/20  <td>2.0.0    <td>ׯ��Ⱥ  <td>���      <td>ά�������µİ汾
* </table>
*
**********************************************************************************
*/

#include "ModBus_Conf.h"
#if defined(USING_MODBUS_RTU)
#include "ModBus_RTU.h"
#elif defined(USING_MODBUS_ASCII)
#include "ModBus_ASCII.h"
#endif


static uint8_t ModBus_Receive_Buf[RECEIVE_SIZE];    ///< ModBus���ջ���(ModBus��������ʱ,ʹ���������)
static uint8_t ModBus_Send_Buf[SEND_SIZE];          ///< ModBus���ͻ���

#ifdef __IN_MEMORY_APP_H

/**@brief       ModBus��ʼ��
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��
* @return       ����ִ�н��
* - None
*/
void ModBus_Init(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
    uint8_t Read_Data;
    
    InMemory_Read_MultiBytes(DEVICE_ADDR, &Read_Data, 1);    //�豸��ַ
    ModBusBaseParam->Device_Addr = Read_Data;
    
    InMemory_Read_MultiBytes(BAUDRATE, &Read_Data, 1);       //���ڲ�����
    ModBusBaseParam->BaudRate = Read_Data;
    
    InMemory_Read_MultiBytes(PARITY, &Read_Data, 1);         //������żУ��
    ModBusBaseParam->Parity = Read_Data;
    
    InMemory_Read_MultiBytes(AUTO_UPLOAD, &Read_Data, 1);    //�Զ��ϴ�����
    ModBusBaseParam->AutoUpload = Read_Data;
    
    InMemory_Read_MultiBytes(OUTPUTMODE, &Read_Data, 1);     //���ģʽ
    ModBusBaseParam->Output_Mode = Read_Data;    
    
    InMemory_Read_MultiBytes(FREEZE, &Read_Data, 1);         //�豸�����־
    ModBusBaseParam->Freeze = Read_Data;
    
    ModBusBaseParam->InRomWrEn = IN_MEMORY_WR_DISABLE;          //д�ڲ��洢��ʹ�ܱ�־
    ModBusBaseParam->ModBus_CallBack = NULL;                //ModBus�ص�����
    
    ModBusBaseParam->ModBus_TX_RX.Receive_Buf = ModBus_Receive_Buf;         //ModBus�������ݻ���
    ModBusBaseParam->ModBus_TX_RX.Send_Buf = ModBus_Send_Buf;               //ModBus�������ݻ���
    ModBusBaseParam->ModBus_TX_RX.Send_Len = 0;                             //ModBus�������ݻ��泤��
    
    Sensor_USART_Init(ModBusBaseParam->BaudRate, ModBusBaseParam->Parity);
}
#else

/**@brief       ModBus��ʼ��
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��
* @param[in]    Param : ModBus��ʼ������������
* @param[in]    Param_Size : Param�����ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         Param����������(���ֽ�˳������): 0,�豸��ַ; 
* 1,�����ʴ���; 2��żУ�����; 3�Զ��ϴ�����; 4,���ģʽ;
* 5,�豸�����־;
*/
uint8_t ModBus_Init(ModBusBaseParam_TypeDef *ModBusBaseParam,
                uint8_t *Param, uint16_t Param_Size)
{    
    uint8_t cnt = 0;

    if(Param != NULL && Param_Size != 0)
    {
        ModBusBaseParam->Device_Addr = Param[cnt++];
        ModBusBaseParam->BaudRate = Param[cnt++];
        ModBusBaseParam->Parity = Param[cnt++];
        ModBusBaseParam->AutoUpload = Param[cnt++];        
        ModBusBaseParam->Output_Mode = Param[cnt++];
        ModBusBaseParam->Freeze = Param[cnt++];        
        ModBusBaseParam->InRomWrEn = FLASH_WR_DISABLE;
        ModBusBaseParam->ModBus_CallBack = NULL;
        
        ModBusBaseParam->ModBus_TX_RX.Receive_Buf = ModBus_Receive_Buf;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf = ModBus_Send_Buf;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 0;
        
        Sensor_USART_Init(ModBusBaseParam->BaudRate, ModBusBaseParam->Parity);
        
        if(cnt > Param_Size)
        {
            return OP_FAILED;
        }
        return OP_SUCCESS;
    }
    return OP_FAILED;
}
#endif

/**@brief       ��������
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    Msg : ��Ϣ�׵�ַ;
* @param[in]    MsgLen : ��Ϣ֡����;
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t Send_Data(ModBusBaseParam_TypeDef *ModBusBaseParam, uint8_t *pMsg, uint16_t len)
{
#ifdef  USART_USING_485
    //����ʹ��
    TX_ON;
#endif
/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
    rt_sem_take(ModBusBaseParam->TX_Lock, RT_WAITING_FOREVER);    //��ȡ�ź���
#else
    while(Sensor_USART_Get_TX_Cplt_Flag() == 0);
    Sensor_USART_Clear_TX_Cplt_Flag();
#endif
    //DMA����
    return BSP_UART_Transmit_DMA(pMsg, len);        
}

/**@brief       Modbus��Ϣ֡������
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
*/
void ModbusHandle(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    uint8_t recv_status;
        
#if defined(USING_MODBUS_RTU)
    //��ȡһ֡ModBus RTU����
    recv_status = MODBUS_RTU_RecvData(ModBusBaseParam->ModBus_TX_RX.Receive_Buf, 
                                    &ModBusBaseParam->ModBus_TX_RX.Receive_Len);
#elif defined(USING_MODBUS_ASCII)
    //��ȡһ֡ModBus ASCII����
    recv_status = MODBUS_ASCII_RecvData(ModBusBaseParam->ModBus_TX_RX.Receive_Buf, 
                                    &ModBusBaseParam->ModBus_TX_RX.Receive_Len);
#endif
    //У����󣬵�ַ���󶼲�������Ϣ֡
    if( recv_status != ERR_NONE 
        || ((ModBusBaseParam->Device_Addr != ModBusBaseParam->ModBus_TX_RX.Receive_Buf[0]) 
        && (BROADCASTADDR != ModBusBaseParam->ModBus_TX_RX.Receive_Buf[0])))
    {     
        return;
    }
    switch(ModBusBaseParam->ModBus_TX_RX.Receive_Buf[1])    //�����봦��
    {
        case 0x03:
            ModbusFunc03(ModBusBaseParam, arg);
        break;

        case 0x04:
            ModbusFunc04(ModBusBaseParam, arg);
        break;

        case 0x05:
            ModbusFunc05(ModBusBaseParam, arg);
        break;
     
        case 0x10:
            ModbusFunc10(ModBusBaseParam, arg);
        break;

        case 0x25:
            ModbusFunc25(ModBusBaseParam, arg);
        break;
      
        case 0x26:
            ModbusFunc26(ModBusBaseParam, arg);
        break;
       
        case 0x27:
            ModbusFunc27(ModBusBaseParam, arg);
        break;
     
        case 0x2A:
            ModbusFunc2A(ModBusBaseParam);
        break;

        case 0x2B:
            ModbusFunc2B(ModBusBaseParam);
        break;
          
        case 0x41:
            ModbusFunc41(ModBusBaseParam);
        break;   

        default:
            ModBusBaseParam->ModBus_TX_RX.Send_Buf[0] 
                = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[0];
            ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] 
                = (ModBusBaseParam->ModBus_TX_RX.Receive_Buf[1] | MB_REQ_FAILURE);
            ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_FUNC_EXCEPTION;
            ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        break;
    }
#if defined(USING_MODBUS_RTU)
    //����һ֡ModBus RTU����
    MODBUS_RTU_SendData(ModBusBaseParam, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Buf, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Len,
                        CHECK_ADDRESS);
#elif defined(USING_MODBUS_ASCII)
    //����һ֡ModBus ASCII����
    MODBUS_ASCII_SendData(ModBusBaseParam, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Buf, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Len,
                        CHECK_ADDRESS);
#endif
    if(ModBusBaseParam->ModBus_CallBack != NULL)
    {        
        ModBusBaseParam->ModBus_CallBack(ModBusBaseParam);
        ModBusBaseParam->ModBus_CallBack = NULL;
    }    
}

/**@brief       Modbus 03��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
* @note         ����ʹ��������,ʵ�ʺ�����ModBus_Conf.c���涨��
*/
__weak void ModbusFunc03(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 04��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
* @note         ����ʹ��������,ʵ�ʺ�����ModBus_Conf.c���涨��
*/
__weak void ModbusFunc04(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 05��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
* @note         ����ʹ��������,ʵ�ʺ�����ModBus_Conf.c���涨��
*/
__weak void ModbusFunc05(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 10��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
* @note         ����ʹ��������,ʵ�ʺ�����ModBus_Conf.c���涨��
*/
__weak void ModbusFunc10(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 25��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
* @note         ����ʹ��������,ʵ�ʺ�����ModBus_Conf.c���涨��
*/
__weak void ModbusFunc25(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 26��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
* @note         ����ʹ��������,ʵ�ʺ�����ModBus_Conf.c���涨��
*/
__weak void ModbusFunc26(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 27��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
* @note         ����ʹ��������,ʵ�ʺ�����ModBus_Conf.c���涨��
*/
__weak void ModbusFunc27(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 2A��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @return       ����ִ�н��
* - None
* @note         ����ʹ��������,ʵ�ʺ�����ModBus_Conf.c���涨��
*/
__weak void ModbusFunc2A(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
    UNUSED(ModBusBaseParam);
}

/**@brief       Modbus 2B��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @return       ����ִ�н��
* - None
* @note         ����ʹ��������,ʵ�ʺ�����ModBus_Conf.c���涨��
*/
__weak void ModbusFunc2B(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
    UNUSED(ModBusBaseParam);
}

/**@brief       Modbus 41��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @return       ����ִ�н��
* - None
* @note         ����ʹ��������,ʵ�ʺ�����ModBus_Conf.c���涨��
*/
__weak void ModbusFunc41(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
    UNUSED(ModBusBaseParam);
}

/**@brief       Modbus ��Ϣ֡�Զ��ϴ�����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
* @note         ����ʹ��������,ʵ�ʺ�����ModBus_Conf.c���涨��
*/
__weak void ModbusAutoUpload(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       �������¼�����,�Զ��ϴ����ڲ�Ϊ0ʱ,�����Եص���ModbusAutoUpload
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
*/
void SensorEvent(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    static uint32_t Old_Tick = 0;
    //���ʹ���Զ��ϴ����Զ��ϴ���ʱʱ�䵽
    if((0 != ModBusBaseParam->AutoUpload) 
        && ((ModBusBaseParam->AutoUpload * AUTOUPLOAD_CYCLE) <= (HAL_GetTick() - Old_Tick)))
    {
        //Modbus֡�Զ��ϴ�
        ModbusAutoUpload(ModBusBaseParam, arg);
        Old_Tick = HAL_GetTick();
    }
}

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
#include "rtthread.h"

#define MODBUS_HANDLE_THREAD_STACK          512
#define MODBUS_HANDLE_THREAD_PRIORITY       RT_THREAD_PRIORITY_MAX - 4

#define MODBUS_AUTOUPLOAD_THREAD_STACK      512
#define MODBUS_AUTOUPLOAD_THREAD_PRIORITY   RT_THREAD_PRIORITY_MAX - 3

static ModBusBaseParam_TypeDef     ModBusBase_Param;     //ModBus����Ļ��������ṹ
static ModBus_Device_Param         ModBus_Device;       //ModBus�����豸�Ĳ����ṹ��


/**@brief       ModBus�����߳�,���ڴ���ModBus��Ϣ֡
* @param[in]    parameter : �̵߳Ĳ���,���ﲻʹ��
* @return       ����ִ�н��
* - None
*/
void modbus_handle_thread_entry(void *parameter)
{                
    while(1)
    {
        //ModBus����
        if(Sensor_USART_Get_RX_Updata_Flag())
        {
            ModbusHandle(&ModBusBase_Param, &ModBus_Device);
            Sensor_USART_Clear_RX_Updata_Flag();
        } 
    }
}

/**@brief       ModBus�Զ��ϴ������߳�,���ڴ���ModBus�Զ��ϴ��¼�
* @param[in]    parameter : �̵߳Ĳ���,���ﲻʹ��
* @return       ����ִ�н��
* - None
*/
void modbus_autoupload_thread_entry(void *parameter)
{
    while(1)
    {
        //����Զ��ϴ�ֵ��0,��ʹ���Զ��ϴ�
        if(ModBusBase_Param.AutoUpload != 0)
        {
            //Modbus֡�Զ��ϴ�
            ModbusAutoUpload(&ModBusBase_Param, &ModBus_Device);        
            rt_thread_mdelay(ModBusBase_Param.AutoUpload * AUTOUPLOAD_CYCLE);
        }
        else
        {
            rt_thread_mdelay(1000);
        }
    }
}

/**@brief       ModBus������ʼ��,����ModBus�����̺߳�ModBus�Զ��ϴ������̲߳�����
* @return       ����ִ�н��
* - int����ֵ(RT_EOK)
* @note         ������ʹ��RT-Thread���Զ���ʼ�����INIT_COMPONENT_EXPORT
* ����ִ��,ϵͳ��λ���Զ���ʼ��
*/
static int modbus_init(void)
{
    rt_device_t                 adc_device;
    rt_device_t                 pcap_device;
    struct rt_adc_device_obj    *adc_device_obj;
    struct rt_pcap_device_obj   *pcap_device_obj;
    rt_thread_t                 modbus_handle_thread;
    rt_thread_t                 modbus_autoupload_thread;
    
    adc_device = rt_device_find(ADC_DEVICE_NAME);
    if(adc_device != RT_NULL)
    {
        adc_device_obj = (struct rt_adc_device_obj *)adc_device->user_data;
        rt_device_open(adc_device, RT_DEVICE_OFLAG_RDONLY);
    }
    pcap_device = rt_device_find(PCAP_DEVICE_NAME);
    if(pcap_device != RT_NULL)
    {
        pcap_device_obj = (struct rt_pcap_device_obj *)pcap_device->user_data;
        rt_device_open(pcap_device, RT_DEVICE_OFLAG_RDWR);
    }        
    
    if(adc_device != RT_NULL)
    {
        ModBus_Device.ADC_TemperParam         = &adc_device_obj->ADC_TemperParam;
        ModBus_Device.ADC_TemperOut           = &adc_device_obj->ADC_TemperOut;
    }
    if(pcap_device != RT_NULL)
    {
        ModBus_Device.DataFilter              = &pcap_device_obj->DataFilter;
        ModBus_Device.PCap_DataConvert        = &pcap_device_obj->PCap_DataConvert; 
        ModBus_Device.PCap_DataConvert_Out    = &pcap_device_obj->PCap_DataConvert_Out;
    }
    
    ModBus_Init(&ModBusBase_Param);                      //ModBus��ʼ��(�������ڳ�ʼ��)    
    
    ModBusBase_Param.TX_Lock
        = (rt_sem_t)rt_object_find(USART_TX_LOCK_NAME, RT_Object_Class_Semaphore);
    
    modbus_handle_thread = rt_thread_create("mb_hand",
                                            modbus_handle_thread_entry,
                                            RT_NULL,
                                            MODBUS_HANDLE_THREAD_STACK,
                                            MODBUS_HANDLE_THREAD_PRIORITY,
                                            20);
    RT_ASSERT(modbus_handle_thread != RT_NULL);
    rt_thread_startup(modbus_handle_thread);
    
    modbus_autoupload_thread = rt_thread_create("mb_up",
                                            modbus_autoupload_thread_entry,
                                            RT_NULL,
                                            MODBUS_AUTOUPLOAD_THREAD_STACK,
                                            MODBUS_AUTOUPLOAD_THREAD_PRIORITY,
                                            20);
    RT_ASSERT(modbus_autoupload_thread != RT_NULL);
    rt_thread_startup(modbus_autoupload_thread);
    
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(modbus_init);

#endif

