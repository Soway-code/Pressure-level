/**@file        usart_app.c
* @brief        ����Ӧ��
* @details      ���ڳ�ʼ�������������շ����жϴ���
* @author       ���
* @date         2020-08-05
* @version      V1.0.1
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/08/05  <td>1.0.1    <td>���    <td>���ո��±�־���� 
* Sensor_USART_Get_RX_Updata_Flag ��Ҫ����ȴ�ʱ�� time ��������ɱ�־
* ���� Sensor_USART_Get_TX_Cplt_Flag �����˻�ȡ�ź�������
* </table>
*
**********************************************************************************
*/

#include "usart_app.h"


static uint16_t RX_Len = 0;                     ///< ���ڽ������ݳ���
static uint16_t Used_RX_Len = 0;                ///< ���ڽ��ջ��汻ռ�ó���
static uint8_t RX_OverRun_Flag = 0;             ///< ���ڽ��ջ��������־
static uint8_t TX_Cplt_Flag = 1;                ///< ���ڷ�����ɱ�־
static uint8_t RX_Updata_Flag = 0;              ///< ���ڽ��ո��±�־
static uint8_t USART_Receive_Buf[RECEIVE_SIZE]; ///< ���ڽ��ջ���


#ifdef  USART_USING_485

/**@brief       ��ʼ��485оƬ�ķ��ͽ��տ��ƹܽ�
* @return       ����ִ�н��
* - None
* @note         ��ͨ���궨��USART_USING_485������ر��������
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
#endif // USART_USING_485

/**@brief       ���ݲ����ʺ���żУ�������ʼ������������
* @param[in]    baudrate_code : �����ʴ���; 
* @param[in]    parity_code : ��żУ�����ʹ���;
* @return       ����ִ�н��
* - None
*/
void Sensor_USART_Init(uint8_t baudrate_code, uint8_t parity_code)
{
    uint32_t baudrate;
    uint32_t parity;
#ifdef  USING_UART_TIMEOUT
    uint32_t timeout;
#endif // USING_UART_TIMEOUT
    
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
#endif // USART_USING_485
    
#ifdef  USING_UART_TIMEOUT
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
#endif // USING_UART_TIMEOUT
    
#ifdef  USING_CHARMATCH
    BSP_UART_CharMatch_Config(DEFAULT_UART_MATCHCHAR);
#endif // USING_CHARMATCH
    BSP_UART_Receive_DMA(USART_Receive_Buf, RECEIVE_SIZE);
}

/**@brief       ��ȡʵ�ʽ��յ�����
* @param[out]   ReceData : ����ʵ�ʽ������ݵĻ���ָ��
* @return       ����ִ�н��
* - ��õ����ݳ���
* @note         ����ʹ����DMAѭ��ģʽ,���뾭�����������ܱ�֤�������ݵ�����
*/
uint16_t Sensor_USART_GetReceData(uint8_t *ReceData)
{
    if(Used_RX_Len > RECEIVE_SIZE || RX_Len > RECEIVE_SIZE)
    {
        return 0;
    }
    
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
    return RX_Len;
}

/**@brief       ��ȡ����������һ�ν��յ������ݳ���
* @return       ����ִ�н��
* - һ�ν��յ������ݳ���
*/
uint16_t Sensor_USART_Get_RX_Len(void)
{
    return RX_Len;
}

/**@brief       ��ȡ���������ڽ��յ����ݺ󣬴��ڽ��ջ��汻ռ�ó���
* @return       ����ִ�н��
* - ���ڽ��ջ��汻ռ�ó���
*/
uint16_t Sensor_USART_Get_RXBuf_Used_Len(void)
{
    return Used_RX_Len;
}

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
#include <rtthread.h>

static struct rt_semaphore usart_rx_lock;
static struct rt_semaphore usart_tx_lock;

/**
* ����       : stm32_usart_rx_lock_init()
* ��������   : 2020-05-18
* ����       : ���
* ����       : �����ź�����ʼ��
* �������   : ��
* �������   : ��
* ���ؽ��   : int����ֵ
* ע���˵�� : ������ʹ��RT-Thread���Զ���ʼ�����INIT_DEVICE_EXPORT
               ����ִ��,ϵͳ��λ���Զ���ʼ��
* �޸�����   :
*/
/**@brief       �����ź�����ʼ��
* @return       int����ֵ
* - RT_ERROR(�ź�����ʼ��ʧ��)
* - RT_EOK(�ź�����ʼ���ɹ�)
*/
static int stm32_usart_rx_lock_init(void)
{
    if( rt_sem_init(&usart_rx_lock, USART_RX_LOCK_NAME, 0, RT_IPC_FLAG_FIFO) != RT_EOK
        || rt_sem_init(&usart_tx_lock, USART_TX_LOCK_NAME, 1, RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        return RT_ERROR;
    }
    
    return RT_EOK;        
}
INIT_DEVICE_EXPORT(stm32_usart_rx_lock_init);

#endif // USING_RT_THREAD_OS

/**@brief       ��ȡ���ո��±�־
* @param[in]    time : ���ʹ�� rt-thread ����ϵͳ�������ź����ȴ�ʱ��, 
* ��λΪϵͳʱ�ӽ���
* @return       ����ִ�н��
* - ���ո��±�־
*/
uint8_t Sensor_USART_Get_RX_Updata_Flag(
#ifdef USING_RT_THREAD_OS
int32_t time
#else
void
#endif // USING_RT_THREAD_OS
)
{
/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
    rt_sem_take(&usart_rx_lock, time);
#endif // USING_RT_THREAD_OS
    return RX_Updata_Flag;
}

/**@brief       ��ȡ������ɱ�־
* @return       ����ִ�н��
* - ������ɱ�־
*/
uint8_t Sensor_USART_Get_TX_Cplt_Flag(void)
{  
/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
    rt_sem_take(&usart_tx_lock, RT_WAITING_FOREVER);    //��ȡ�ź���
#endif // USING_RT_THREAD_OS
    return TX_Cplt_Flag;
}

/**@brief       ������ո��±�־
* @return       ����ִ�н��
* - None
*/
void Sensor_USART_Clear_RX_Updata_Flag(void)
{
    RX_Updata_Flag = 0;
}

/**@brief       ���������ɱ�־
* @return       ����ִ�н��
* - None
*/
void Sensor_USART_Clear_TX_Cplt_Flag(void)
{
    TX_Cplt_Flag = 0;
}

#ifdef  USING_UART_TIMEOUT
/**@brief       ���ڽ��ճ�ʱ�жϻص�����
* @param[out]   huart : ���ڴ������ָ��
* @return       ����ִ�н��
* - None
* @note         �������Զ����һ����������HAL���в����ڵ�
*/
void HAL_UART_RxTimoCallback(UART_HandleTypeDef *huart)
{
    uint16_t rx_buf_used;
    
    /* ��ȡ��ǰDMA����ռ�ó��ȣ���λ��byte */
    rx_buf_used = huart->RxXferSize - __HAL_DMA_GET_COUNTER(huart->hdmarx);
    if(RX_OverRun_Flag)         //���������־
    {
        /* ���㵱ǰ���յ����ݳ��� */
        RX_Len = rx_buf_used + huart->RxXferSize - Used_RX_Len;        
        RX_OverRun_Flag = 0;    //���������־����
    }
    else
    {
        /* ���㵱ǰ���յ����ݳ��� */
        RX_Len = rx_buf_used - Used_RX_Len;    
    }
    /* ��ǰDMA����ռ�ó��� */
    Used_RX_Len = rx_buf_used;
    
/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */    
#ifdef USING_RT_THREAD_OS
    rt_sem_release(&usart_rx_lock);     //�ͷ��ź���
#endif // USING_RT_THREAD_OS
    RX_Updata_Flag = 1;         //���ո��±�־��1
}
#endif // USING_UART_TIMEOUT

#ifdef  USING_CHARMATCH
/**@brief       �����ַ�ƥ���жϻص�����
* @param[out]   huart : ���ڴ������ָ��
* @return       ����ִ�н��
* - None
* @note         �������Զ����һ����������HAL���в����ڵ�
*/
void HAL_UART_CMatchCallback(UART_HandleTypeDef *huart)
{
    uint16_t rx_buf_used;
    
    /* ��ȡ��ǰDMA����ռ�ó��ȣ���λ��byte */
    rx_buf_used = huart->RxXferSize - __HAL_DMA_GET_COUNTER(huart->hdmarx);
    if(RX_OverRun_Flag)         //���������־
    {
        /* ���㵱ǰ���յ����ݳ��� */
        RX_Len = rx_buf_used + huart->RxXferSize - Used_RX_Len;        
        RX_OverRun_Flag = 0;    //���������־����
    }
    else
    {
        /* ���㵱ǰ���յ����ݳ��� */
        RX_Len = rx_buf_used - Used_RX_Len;    
    }
    /* ��ǰDMA����ռ�ó��� */
    Used_RX_Len = rx_buf_used;
    
/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */    
#ifdef USING_RT_THREAD_OS
    rt_sem_release(&usart_rx_lock);     //�ͷ��ź���
#endif // USING_RT_THREAD_OS
    RX_Updata_Flag = 1;         //���ո��±�־��1
}
#endif // USING_CHARMATCH

/**@brief       ���ڽ�������жϻص�����,���ﴮ�ڽ�����ɱ�ʾ���յ��������˻������
* @param[out]   huart : ���ڴ������ָ��
* @return       ����ִ�н��
* - None
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    RX_OverRun_Flag = 1;
}  

/**@brief       ���ڷ�������жϻص�����
* @param[out]   huart : ���ڴ������ָ��
* @return       ����ִ�н��
* - None
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
#ifdef  USART_USING_485
    RX_ON;
#endif // USART_USING_485
/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */    
#ifdef USING_RT_THREAD_OS
    rt_sem_release(&usart_tx_lock);     //�ͷ��ź���
#endif // USING_RT_THREAD_OS
    TX_Cplt_Flag = 1;
}
