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
static uint8_t TX_Cplt_Flag = 0;
static uint8_t RX_Updata_Flag = 0;
static uint8_t BaudRate_Code = 1;
static uint8_t Parity_Code = 0;


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
void Sensor_USART_Init(uint8_t baudrate_code, uint8_t parity_code, 
                    uint8_t *Receive_Data, uint16_t Receive_Data_Size)
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
    if(baudrate_code == USART_BAUDRATE_4800_CODE || baudrate_code == USART_BAUDRATE_9600_CODE
        || baudrate_code == USART_BAUDRATE_19200_CODE || baudrate_code == USART_BAUDRATE_38400_CODE
        || baudrate_code == USART_BAUDRATE_57600_CODE || baudrate_code == USART_BAUDRATE_115200_CODE)
    {
        BaudRate_Code = baudrate_code;
    }
    else
    {
        BaudRate_Code = USART_BAUDRATE_9600_CODE;
    }
    if(parity_code == USART_PARITY_NONE_CODE || parity_code == USART_PARITY_ODD_CODE
        || parity_code == USART_PARITY_EVEN_CODE)
    {
        Parity_Code = parity_code;
    }
    else
    {
        Parity_Code = USART_PARITY_NONE_CODE;
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
    BSP_UART_Receive_DMA(Receive_Data, Receive_Data_Size);
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

/**
* 名称       : Sensor_USART_Get_BaudRate_Code()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取传感器串口的波特率代码
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 串口的波特率代码
* 注意和说明 : 
* 修改内容   :
*/
uint8_t Sensor_USART_Get_BaudRate_Code(void)
{
    return BaudRate_Code;
}

/**
* 名称       : Sensor_USART_Get_Parity_Code()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取传感器串口的奇偶校验代码
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 串口的奇偶校验代码
* 注意和说明 : 
* 修改内容   :
*/
uint8_t Sensor_USART_Get_Parity_Code(void)
{
    return Parity_Code;
}

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
* 注意和说明 : 该函数是个弱定义函数，可在其他文件中重定义，且是自定义的一个函数，在HAL库中不存在的
* 修改内容   :
*/
void HAL_UART_RxTimoCallback(UART_HandleTypeDef *huart)
{
    uint16_t remain_len;
    
    remain_len = huart->RxXferSize - __HAL_DMA_GET_COUNTER(huart->hdmarx);
    if(RX_OverRun_Flag)
    {
        RX_Len = remain_len + huart->RxXferSize - Used_RX_Len;
        Used_RX_Len = remain_len;
        RX_OverRun_Flag = 0;
    }
    else
    {
        RX_Len = remain_len - Used_RX_Len;    
        Used_RX_Len += RX_Len;
    }
    
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
    TX_Cplt_Flag = 1;
}
