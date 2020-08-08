/**@file        Modbus.c
* @brief        Modbus 通信协议
* @details      标定功能、采集数据交互
* @author       庄明群
* @date         2020-08-05
* @version      V2.1.0
* @copyright    2020-2030,深圳市信为科技发展有限公司
**********************************************************************************
* @par 修改日志:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2020/08/05  <td>2.1.0    <td>庄明群  <td>杨春林      <td>删除了自动上传线程, 
* 使用 ModBus 处理线程处理消息帧和自动上传
* </table>
*
**********************************************************************************
*/

#include "ModBus.h"

#if defined(USING_MODBUS_RTU)
#include "ModBus_RTU.h"
#elif defined(USING_MODBUS_ASCII)
#include "ModBus_ASCII.h"
#endif // defined(USING_MODBUS_RTU) or defined(USING_MODBUS_ASCII)


static uint8_t ModBus_Receive_Buf[RECEIVE_SIZE];    ///< ModBus接收缓存(ModBus解析处理时,使用这个缓存)
static uint8_t ModBus_Send_Buf[SEND_SIZE];          ///< ModBus发送缓存

#ifdef __IN_MEMORY_APP_H

/**@brief       ModBus初始化
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体
* @return       函数执行结果
* - None
*/
void ModBus_Init(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
/* 使用soway上位机升级程序(Boot程序), BOOT_PROGRAM在main.h中定义 */
#ifndef BOOT_PROGRAM
    uint8_t Read_Data;
    
    InMemory_Read_MultiBytes(DEVICE_ADDR, &Read_Data, 1);    //设备地址
    ModBusBaseParam->Device_Addr = Read_Data;
    
    InMemory_Read_MultiBytes(BAUDRATE, &Read_Data, 1);       //串口波特率
    ModBusBaseParam->BaudRate = Read_Data;
    
    InMemory_Read_MultiBytes(PARITY, &Read_Data, 1);         //串口奇偶校验
    ModBusBaseParam->Parity = Read_Data;
    
    InMemory_Read_MultiBytes(AUTO_UPLOAD, &Read_Data, 1);    //自动上传周期
    ModBusBaseParam->AutoUpload = Read_Data;
    
    InMemory_Read_MultiBytes(OUTPUTMODE, &Read_Data, 1);     //输出模式
    ModBusBaseParam->Output_Mode = Read_Data;    
    
    InMemory_Read_MultiBytes(FREEZE, &Read_Data, 1);         //设备冻结标志
    ModBusBaseParam->Freeze = Read_Data;
    
    ModBusBaseParam->InRomWrEn = IN_MEMORY_WR_DISABLE;          //写内部存储器使能标志
#else
    if(InMemory_Read_OneByte(ADDR_DEVICEADDR) == 0)
    {
        InMemory_Write_OneByte(ADDR_DEVICEADDR, 0xFF);
    }
    ModBusBaseParam->Device_Addr    = InMemory_Read_OneByte(ADDR_DEVICEADDR);
    ModBusBaseParam->ProgErase      = InMemory_Read_OneByte(ADDR_ERASEFLAG);
    ModBusBaseParam->BaudRate       = USART_BAUDRATE_9600_CODE;
    ModBusBaseParam->Parity         = USART_PARITY_NONE_CODE;
#endif // BOOT_PROGRAM
        
    ModBusBaseParam->ModBus_CallBack = NULL;                //ModBus回调函数
    
    ModBusBaseParam->ModBus_TX_RX.Receive_Buf = ModBus_Receive_Buf;         //ModBus接收数据缓存
    ModBusBaseParam->ModBus_TX_RX.Send_Buf = ModBus_Send_Buf;               //ModBus发送数据缓存
    ModBusBaseParam->ModBus_TX_RX.Send_Len = 0;                             //ModBus发送数据缓存长度
    
    Sensor_USART_Init(ModBusBaseParam->BaudRate, ModBusBaseParam->Parity);
}
#else

/**@brief       ModBus初始化
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体
* @param[in]    Param : ModBus初始化的数据内容
* @param[in]    Param_Size : Param的数据长度
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
* @note         Param的数据内容(按字节顺序排列): 0,设备地址; 
* 1,波特率代码; 2奇偶校验代码; 3自动上传周期; 4,输出模式;
* 5,设备冻结标志;
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
#endif // __IN_MEMORY_APP_H

/**@brief       发送数据
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @param[in]    Msg : 消息首地址;
* @param[in]    MsgLen : 消息帧长度;
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
*/
uint8_t Send_Data(ModBusBaseParam_TypeDef *ModBusBaseParam, uint8_t *pMsg, uint16_t len)
{
#ifdef  USART_USING_485
    //发送使能
    TX_ON;
#endif // USART_USING_485

    while(Sensor_USART_Get_TX_Cplt_Flag() == 0);
    Sensor_USART_Clear_TX_Cplt_Flag();

    //DMA发送
    return BSP_UART_Transmit_DMA(pMsg, len);        
}

/**@brief       Modbus消息帧处理函数
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @param[in]    arg : 用户自定义的参数,例如设备参数
* @return       函数执行结果
* - None
*/
void ModbusHandle(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    uint8_t recv_status;
        
#if defined(USING_MODBUS_RTU)
    //获取一帧ModBus RTU数据
    recv_status = MODBUS_RTU_RecvData(ModBusBaseParam->ModBus_TX_RX.Receive_Buf, 
                                    &ModBusBaseParam->ModBus_TX_RX.Receive_Len);
#elif defined(USING_MODBUS_ASCII)
    //获取一帧ModBus ASCII数据
    recv_status = MODBUS_ASCII_RecvData(ModBusBaseParam->ModBus_TX_RX.Receive_Buf, 
                                    &ModBusBaseParam->ModBus_TX_RX.Receive_Len);
#endif // defined(USING_MODBUS_RTU) or defined(USING_MODBUS_ASCII)
    //校验错误，地址错误都不处理消息帧
    if( recv_status != ERR_NONE 
        || ((ModBusBaseParam->Device_Addr != ModBusBaseParam->ModBus_TX_RX.Receive_Buf[0]) 
        && (BROADCASTADDR != ModBusBaseParam->ModBus_TX_RX.Receive_Buf[0])))
    {     
        return;
    }
    switch(ModBusBaseParam->ModBus_TX_RX.Receive_Buf[1])    //功能码处理
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
    //发送一帧ModBus RTU数据
    MODBUS_RTU_SendData(ModBusBaseParam, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Buf, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Len,
                        CHECK_ADDRESS);
#elif defined(USING_MODBUS_ASCII)
    //发送一帧ModBus ASCII数据
    MODBUS_ASCII_SendData(ModBusBaseParam, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Buf, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Len,
                        CHECK_ADDRESS);
#endif // defined(USING_MODBUS_RTU) or defined(USING_MODBUS_ASCII)
    if(ModBusBaseParam->ModBus_CallBack != NULL)
    {        
        ModBusBaseParam->ModBus_CallBack(ModBusBaseParam);
        ModBusBaseParam->ModBus_CallBack = NULL;
    }    
}

/**@brief       Modbus 03功能码消息帧处理
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @param[in]    arg : 用户自定义的参数,例如设备参数
* @return       函数执行结果
* - None
* @note         这里使用弱定义,实际函数在ModBus_Conf.c里面定义
*/
__weak void ModbusFunc03(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 04功能码消息帧处理
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @param[in]    arg : 用户自定义的参数,例如设备参数
* @return       函数执行结果
* - None
* @note         这里使用弱定义,实际函数在ModBus_Conf.c里面定义
*/
__weak void ModbusFunc04(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 05功能码消息帧处理
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @param[in]    arg : 用户自定义的参数,例如设备参数
* @return       函数执行结果
* - None
* @note         这里使用弱定义,实际函数在ModBus_Conf.c里面定义
*/
__weak void ModbusFunc05(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 10功能码消息帧处理
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @param[in]    arg : 用户自定义的参数,例如设备参数
* @return       函数执行结果
* - None
* @note         这里使用弱定义,实际函数在ModBus_Conf.c里面定义
*/
__weak void ModbusFunc10(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 25功能码消息帧处理
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @param[in]    arg : 用户自定义的参数,例如设备参数
* @return       函数执行结果
* - None
* @note         这里使用弱定义,实际函数在ModBus_Conf.c里面定义
*/
__weak void ModbusFunc25(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 26功能码消息帧处理
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @param[in]    arg : 用户自定义的参数,例如设备参数
* @return       函数执行结果
* - None
* @note         这里使用弱定义,实际函数在ModBus_Conf.c里面定义
*/
__weak void ModbusFunc26(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 27功能码消息帧处理
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @param[in]    arg : 用户自定义的参数,例如设备参数
* @return       函数执行结果
* - None
* @note         这里使用弱定义,实际函数在ModBus_Conf.c里面定义
*/
__weak void ModbusFunc27(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/**@brief       Modbus 2A功能码消息帧处理
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @return       函数执行结果
* - None
* @note         这里使用弱定义,实际函数在ModBus_Conf.c里面定义
*/
__weak void ModbusFunc2A(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
    UNUSED(ModBusBaseParam);
}

/**@brief       Modbus 2B功能码消息帧处理
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @return       函数执行结果
* - None
* @note         这里使用弱定义,实际函数在ModBus_Conf.c里面定义
*/
__weak void ModbusFunc2B(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
    UNUSED(ModBusBaseParam);
}

/**@brief       Modbus 41功能码消息帧处理
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @return       函数执行结果
* - None
* @note         这里使用弱定义,实际函数在ModBus_Conf.c里面定义
*/
__weak void ModbusFunc41(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
    UNUSED(ModBusBaseParam);
}

/**@brief       Modbus 消息帧自动上传处理
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @param[in]    arg : 用户自定义的参数,例如设备参数
* @return       函数执行结果
* - None
* @note         这里使用弱定义,实际函数在ModBus_Conf.c里面定义
*/
__weak void ModbusAutoUpload(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    UNUSED(ModBusBaseParam);
    UNUSED(arg);
}

/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS
#include "rtthread.h"

#define MODBUS_HANDLE_THREAD_STACK          512
#define MODBUS_HANDLE_THREAD_PRIORITY       RT_THREAD_PRIORITY_MAX - 4

static ModBusBaseParam_TypeDef     ModBusBase_Param;     //ModBus处理的基本参数结构
static ModBus_Device_Param         ModBus_Device;       //ModBus管理设备的参数结构体


/**@brief       ModBus处理线程,用于处理ModBus消息帧
* @param[in]    parameter : 线程的参数,这里不使用
* @return       函数执行结果
* - None
*/
void modbus_handle_thread_entry(void *parameter)
{                
    while(1)
    {
        // 判断串口是否有数据接收并输入等待时间(单位为 ms)，将输入的等待时间转换成系统时钟节拍
        if(Sensor_USART_Get_RX_Updata_Flag(
            rt_tick_from_millisecond(
            (ModBusBase_Param.AutoUpload ? ModBusBase_Param.AutoUpload : 1) // 判断自动上传周期是否为0，不为0则使用
            * 1000)))                                                       // 自动上传周期 * 1000 ms ，否则使用 1000 ms
        {           
            // Modbus 消息帧处理
            ModbusHandle(&ModBusBase_Param, &ModBus_Device);            
            Sensor_USART_Clear_RX_Updata_Flag();                        
        } 
        else if(ModBusBase_Param.AutoUpload != 0)
        {
            // ModBus 帧自动上传
            ModbusAutoUpload(&ModBusBase_Param, &ModBus_Device); 
        }
    }
}

/**@brief       ModBus参数初始化,创建ModBus处理线程和ModBus自动上传处理线程并启动
* @return       函数执行结果
* - int整型值(RT_EOK)
* @note         本函数使用RT-Thread的自动初始化组件INIT_COMPONENT_EXPORT
* 调用执行,系统复位后自动初始化
*/
static int modbus_init(void)
{
    rt_device_t                 adc_device;                 // ADC设备
    rt_device_t                 pcap_device;                // Pcap设备
    struct rt_adc_device_obj    *adc_device_obj;            // ADC设备对象(包含设备处理参数和输出值)
    struct rt_pcap_device_obj   *pcap_device_obj;           // Pcap设备对象(包含设备处理参数和输出值)
    rt_thread_t                 modbus_handle_thread;       // ModBus 处理线程句柄
    
/************************************** 查找Pcap设备并打开(启动) ***************************************/
    // 查找ADC设备
    adc_device = rt_device_find(ADC_DEVICE_NAME);           
    if(adc_device != RT_NULL)
    {   
        // 获取ADC设备对象信息
        adc_device_obj = (struct rt_adc_device_obj *)adc_device->user_data; 
        // 打开(启动)ADC设备
        rt_device_open(adc_device, RT_DEVICE_OFLAG_RDONLY); 
    }
/*******************************************************************************************************/
    
    
/************************************** 查找Pcap设备并打开(启动) ***************************************/
    // 查找Pcap设备
    pcap_device = rt_device_find(PCAP_DEVICE_NAME);         
    if(pcap_device != RT_NULL)
    {
        // 获取Pcap设备对象信息
        pcap_device_obj = (struct rt_pcap_device_obj *)pcap_device->user_data;  
        // 打开(启动)Pcap设备
        rt_device_open(pcap_device, RT_DEVICE_OFLAG_RDWR);  
    }  
/*******************************************************************************************************/
      
    
/******************************************* 设备参数初始化 ********************************************/
    /* 为 ModBus 添加 ADC 和 Pcap 设备对象 */
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
/*******************************************************************************************************/
    
    
/******************************************** ModBus初始化 *********************************************/
    // ModBus初始化(包括串口初始化)    
    ModBus_Init(&ModBusBase_Param);                      
    // 查找串口发送锁
    ModBusBase_Param.TX_Lock                            
        = (rt_sem_t)rt_object_find(USART_TX_LOCK_NAME, RT_Object_Class_Semaphore);    
    
    /* 创建 ModBus 处理线程 */
    modbus_handle_thread = rt_thread_create("mb_hand",
                                            modbus_handle_thread_entry,
                                            RT_NULL,
                                            MODBUS_HANDLE_THREAD_STACK,
                                            MODBUS_HANDLE_THREAD_PRIORITY,
                                            20);
    RT_ASSERT(modbus_handle_thread != RT_NULL);
    // 启动 ModBus 处理线程
    rt_thread_startup(modbus_handle_thread);        
/*******************************************************************************************************/
    
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(modbus_init);

#endif // USING_RT_THREAD_OS

