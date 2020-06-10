/****************************************************************************************************************************************************************
** 版权:          2020-2030,深圳市信为科技发展有限公司
** 文件名:        usart_app.c
** 作者:          杨春林
** 版本:          V1.0.0
** 日期:          2020-04-29
** 描述:          串口通信
** 功能:          串口初始化、串口数据收发、中断处理
*****************************************************************************************************************************************************************
** 修改者:        No
** 版本:          No
** 修改内容:      No
** 日期:          No
****************************************************************************************************************************************************************/

#include "usart_app.h"


static uint16_t RX_Len = 0;
static uint16_t Used_RX_Len = 0;
static uint8_t RX_OverRun_Flag = 0;
static uint8_t TX_Cplt_Flag = 1;
static uint8_t RX_Updata_Flag = 0;
static uint8_t USART_Receive_Buf[RECEIVE_SIZE];     //串口接收缓存


#ifdef  USART_USING_485

/**
* 名称       : _485_RE_DE_GPIO_Init()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 初始化485芯片的输入输出控制管脚
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 可通过宏定义USART_USING_485开启或关闭这个函数
* 修改内容   :
*/
static void _485_RE_DE_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  _485_RE_DE_PIN_CLK_ENABLE();

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = _485_RE_DE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(_485_RE_DE_GPIO_PORT, &GPIO_InitStruct);
  
  RX_ON;
}
#endif

/**
* 名称       : Sensor_USART_Init()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 根据波特率和奇偶校验参数初始化传感器串口
* 输入参数   : baudrate_code,波特率代码; parity_code,奇偶校验类型代码;
*              Receive_Data,接收缓存指针; Receive_Data_Size,接收缓存大小;
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Sensor_USART_Init(uint8_t baudrate_code, uint8_t parity_code)
{
    uint32_t baudrate;
    uint32_t parity;
    uint32_t timeout;
    
    switch(baudrate_code)
    {
        case USART_BAUDRATE_4800_CODE : baudrate = 4800;
        break;
        
        case USART_BAUDRATE_9600_CODE : baudrate = 9600;
        break;
        
        case USART_BAUDRATE_19200_CODE : baudrate = 19200;
        break;
        
        case USART_BAUDRATE_38400_CODE : baudrate = 38400;
        break;
        
        case USART_BAUDRATE_57600_CODE : baudrate = 57600;
        break;
        
        case USART_BAUDRATE_115200_CODE : baudrate = 115200;
        break;
        
        default : baudrate = DEFAULT_UART_BAUD;
        break;
    }
    switch(parity_code)
    {
        case USART_PARITY_NONE_CODE : parity = UART_PARITY_NONE;
        break;
        
        case USART_PARITY_ODD_CODE : parity = UART_PARITY_ODD;
        break;
        
        case USART_PARITY_EVEN_CODE : parity = UART_PARITY_EVEN;
        break;
        
        default : parity = UART_PARITY_NONE;
        break;
    }
    
    BSP_USART_UART_Init(baudrate, parity);
#ifdef  USART_USING_485
    _485_RE_DE_GPIO_Init();
#endif
    if(parity_code == USART_PARITY_NONE_CODE)
    {
        timeout = DEFAULT_UART_TIMEOUT * 10;
    }
    else
    {
        timeout = DEFAULT_UART_TIMEOUT * 11;
    }
    timeout = (timeout % 10) ? ((timeout / 10) + 1) : timeout / 10;
    BSP_UART_ReceiverTimeout_Config(timeout);
    BSP_UART_Receive_DMA(USART_Receive_Buf, RECEIVE_SIZE);
}

/**
* 名称       : Sensor_USART_GetReceData()
* 创建日期   : 2020-05-15
* 作者       : 杨春林
* 功能       : 获取实际接收的数据(由于使用了DMA循环模式,必须经过本函数才能保证接收数据的完整)
* 输入参数   : 无
* 输出参数   : ReceData:保存实际接收数据的缓存指针
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Sensor_USART_GetReceData(uint8_t *ReceData)
{
    if(Used_RX_Len > RECEIVE_SIZE || RX_Len > RECEIVE_SIZE)
        return;
    
    if(Used_RX_Len > RX_Len)
    {
        memcpy( ReceData, USART_Receive_Buf 
                + Used_RX_Len
                - RX_Len, 
                RX_Len);
    }
    else
    {
        memcpy( ReceData, USART_Receive_Buf 
                + (RECEIVE_SIZE - RX_Len 
                + Used_RX_Len), 
                RX_Len - Used_RX_Len);
        memcpy( ReceData + RX_Len - Used_RX_Len, 
                USART_Receive_Buf, 
                Used_RX_Len);
    }
}

/**
* 名称       : Sensor_USART_Get_RX_Len()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取传感器串口一次接收到的数据长度
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 一次接收到的数据长度
* 注意和说明 : 
* 修改内容   :
*/
uint16_t Sensor_USART_Get_RX_Len(void)
{
    return RX_Len;
}

/**
* 名称       : Sensor_USART_Get_RXBuf_Used_Len()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取传感器串口接收到数据后，从接收缓存头部到数据尾部的长度
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 从接收缓存头部到数据尾部的长度
* 注意和说明 : 
* 修改内容   :
*/
uint16_t Sensor_USART_Get_RXBuf_Used_Len(void)
{
    return Used_RX_Len;
}

/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS
#include <rtthread.h>

#define USART_SEM_NAME      "usart"

static struct rt_semaphore usart_sem;
static struct rt_semaphore usart_tx_lock;

/**
* 名称       : stm32_usart_sem_init()
* 创建日期   : 2020-05-18
* 作者       : 杨春林
* 功能       : 串口信号量初始化
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : int整型值
* 注意和说明 : 本函数使用RT-Thread的自动初始化组件INIT_DEVICE_EXPORT
               调用执行,系统复位后自动初始化
* 修改内容   :
*/
static int stm32_usart_sem_init(void)
{
    if( rt_sem_init(&usart_sem, USART_SEM_NAME, 0, RT_IPC_FLAG_FIFO) != RT_EOK
        || rt_sem_init(&usart_tx_lock, USART_TX_LOCK_NAME, 1, RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        return RT_ERROR;
    }
    
    return RT_EOK;        
}
INIT_DEVICE_EXPORT(stm32_usart_sem_init);

#endif

/**
* 名称       : Sensor_USART_Get_RX_Updata_Flag()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取接收更新标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 接收更新标志
* 注意和说明 : 
* 修改内容   :
*/
uint8_t Sensor_USART_Get_RX_Updata_Flag(void)
{
/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS
    rt_sem_take(&usart_sem, RT_WAITING_FOREVER);    //获取信号量
#endif
    return RX_Updata_Flag;
}

/**
* 名称       : Sensor_USART_Get_TX_Cplt_Flag()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取发送完成标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 发送完成标志
* 注意和说明 : 
* 修改内容   :
*/
uint8_t Sensor_USART_Get_TX_Cplt_Flag(void)
{  
    return TX_Cplt_Flag;
}

/**
* 名称       : Sensor_USART_Clear_RX_Updata_Flag()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 清除接收更新标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Sensor_USART_Clear_RX_Updata_Flag(void)
{
    RX_Updata_Flag = 0;
}

/**
* 名称       : Sensor_USART_Clear_TX_Cplt_Flag()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 清除发送完成标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Sensor_USART_Clear_TX_Cplt_Flag(void)
{
    TX_Cplt_Flag = 0;
}

/**
* 名称       : HAL_UART_RxTimoCallback()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口接收超时中断回调函数
* 输入参数   : huart,串口处理对象指针
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 本函数自定义的一个函数，在HAL库中不存在的
* 修改内容   :
*/
void HAL_UART_RxTimoCallback(UART_HandleTypeDef *huart)
{
    uint16_t rx_buf_used;
    
    rx_buf_used = huart->RxXferSize - __HAL_DMA_GET_COUNTER(huart->hdmarx);
    if(RX_OverRun_Flag)
    {
        RX_Len = rx_buf_used + huart->RxXferSize - Used_RX_Len;        
        RX_OverRun_Flag = 0;
    }
    else
    {
        RX_Len = rx_buf_used - Used_RX_Len;    
    }
    Used_RX_Len = rx_buf_used;
    
/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */    
#ifdef USING_RT_THREAD_OS
    rt_sem_release(&usart_sem);     //释放信号量
#endif
    RX_Updata_Flag = 1;
}

/**
* 名称       : HAL_UART_RxCpltCallback()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 串口接收完成中断回调函数
* 输入参数   : huart,串口处理对象指针
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    RX_OverRun_Flag = 1;
}  

/**
* 名称       : HAL_UART_TxCpltCallback()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 串口发送完成中断回调函数
* 输入参数   : huart,串口处理对象指针
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
#ifdef  USART_USING_485
    RX_ON;
#endif
/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */    
#ifdef USING_RT_THREAD_OS
    rt_sem_release(&usart_tx_lock);     //释放信号量
#endif
    TX_Cplt_Flag = 1;
}
