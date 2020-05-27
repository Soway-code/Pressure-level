/****************************************************************************************************************************************************************
** 版权:          2020-2030,深圳市信为科技发展有限公司
** 文件名:        usart_bsp.c
** 作者:          杨春林
** 版本:          V1.0.0
** 日期:          2020-04-28
** 描述:          通用串口配置
** 功能:          通用串口初始化, 为串口数据收发提供底层函数
*****************************************************************************************************************************************************************
** 修改者:        No
** 版本:          No
** 修改内容:      No
** 日期:          No
****************************************************************************************************************************************************************/

#include "usart_bsp.h"

/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS
#include "board.h"
#endif


/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

static UART_HandleTypeDef huart;
static DMA_HandleTypeDef hdma_usart_rx;
static DMA_HandleTypeDef hdma_usart_tx;


/**
* 名称       : BSP_USART_UART_Init()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口初始化
* 输入参数   : baudrate,波特率；parity,奇偶校验
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void BSP_USART_UART_Init(uint32_t baudrate, uint32_t parity)
{
    huart.Instance = USER_USART;
    if(parity == UART_PARITY_NONE)
    {
        huart.Init.WordLength = UART_WORDLENGTH_8B;
    }
    else if(parity == UART_PARITY_ODD 
        || parity == UART_PARITY_EVEN)
    {
        huart.Init.WordLength = UART_WORDLENGTH_9B;
    }
    huart.Init.StopBits = UART_STOPBITS_1;
    huart.Init.Mode = UART_MODE_TX_RX;
    huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart.Init.OverSampling = UART_OVERSAMPLING_16;
    huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;    
    huart.Init.BaudRate = baudrate;
    huart.Init.Parity = parity;

    if (HAL_UART_Init(&huart) != HAL_OK)
    {
        Error_Handler();
    }
    
}

/**
* 名称       : HAL_UART_MspInit()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口时钟、底层管脚初始化（由HAL_UART_Init回调调用）
* 输入参数   : uartHandle，串口处理对象
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 串口的时钟、管脚、中断服务函数类型等在usart_bsp.h文件里面定义
* 修改内容   :
*/
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();
  
    USER_USART_TX_PIN_CLK_ENABLE();
    USER_USART_RX_PIN_CLK_ENABLE();

    GPIO_InitStruct.Pin = USER_USART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = USER_USART_ALTERNATE;
    HAL_GPIO_Init(USER_USART_TX_PIN_GPIOX, &GPIO_InitStruct);
      
    GPIO_InitStruct.Pin = USER_USART_RX_PIN;
    HAL_GPIO_Init(USER_USART_RX_PIN_GPIOX, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart_rx.Instance = DMA1_Channel3;
    hdma_usart_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart_rx.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_usart_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart_rx);

    /* USART1_TX Init */
    hdma_usart_tx.Instance = DMA1_Channel2;
    hdma_usart_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart_tx.Init.Mode = DMA_NORMAL;
    hdma_usart_tx.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_usart_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
    
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

/**
* 名称       : HAL_UART_MspDeInit()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口时钟、底层管脚反初始化（由HAL_UART_DeInit回调调用）
* 输入参数   : uartHandle，串口处理对象指针
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 串口的时钟、管脚、中断服务函数类型等在usart_bsp.h文件里面定义
* 修改内容   :
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_DISABLE();
  
    HAL_GPIO_DeInit(USER_USART_TX_PIN_GPIOX, USER_USART_TX_PIN);
    HAL_GPIO_DeInit(USER_USART_RX_PIN_GPIOX, USER_USART_RX_PIN);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/**
* 名称       : BSP_UART_Transmit_DMA()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口开始DMA发送
* 输入参数   : pData：DMA发送缓存指针;
*              Size：缓存大小
* 输出参数   : 无
* 返回结果   : OP_SUCCESS(成功),OP_FAILED(失败)
* 注意和说明 : 
* 修改内容   :
*/
uint8_t BSP_UART_Transmit_DMA(uint8_t *pData, uint16_t Size)
{
    if(HAL_UART_Transmit_DMA(&huart, pData, Size) != HAL_OK)
    {
        return OP_FAILED;
    }
    return OP_SUCCESS;
}

/**
* 名称       : BSP_UART_Receive_DMA()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口开始DMA接收
* 输入参数   : pData：DMA接收缓存指针;
*              Size：缓存大小
* 输出参数   : 无
* 返回结果   : OP_SUCCESS(成功),OP_FAILED(失败)
* 注意和说明 : 
* 修改内容   :
*/
uint8_t BSP_UART_Receive_DMA(uint8_t *pData, uint16_t Size)
{
    if(HAL_UART_Receive_DMA(&huart, pData, Size) != HAL_OK)
    {
        return OP_FAILED;
    }
    return OP_SUCCESS;
}

/**
* 名称       : BSP_UART_ReceiverTimeout_Config()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口配置接收超时中断
* 输入参数   : TimeoutValue：超时时间，单位波特率位数;
*              例如：1个起始位，8位数据，1个停止位的串口配置，
*              则10位为1个数据字节，若设置4个数据字节的超时时间，
*              TimeoutValue = 40
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void BSP_UART_ReceiverTimeout_Config(uint32_t TimeoutValue)
{
    HAL_UART_DisableReceiverTimeout(&huart);
    __HAL_UART_DISABLE_IT(&huart, UART_IT_RTO);
    
    HAL_UART_ReceiverTimeout_Config(&huart, TimeoutValue);
    
    HAL_UART_EnableReceiverTimeout(&huart);
    __HAL_UART_ENABLE_IT(&huart, UART_IT_RTO);
}

/**
* 名称       : HAL_UART_RxTimoCallback()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口接收超时中断回调函数
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 该函数是个弱定义函数，可在其他文件中重定义，且是自定义的一个函数，在HAL库中不存在的
* 修改内容   :
*/
__weak void HAL_UART_RxTimoCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}

/**
* 名称       : DMA1_Channel2_3_IRQHandler()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : DMA1通道2和3的中断服务函数
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void DMA1_Channel2_3_IRQHandler(void)
{

  HAL_DMA_IRQHandler(&hdma_usart_tx);
  HAL_DMA_IRQHandler(&hdma_usart_rx);

}

/**
* 名称       : USER_USART_IRQHANDLER()
* 创建日期   : 2020-04-28
* 作者       : 杨春林
* 功能       : 串口中断服务函数
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : USER_USART_IRQHANDLER在usart_bsp.h文件里面定义
* 修改内容   :
*/
void USER_USART_IRQHANDLER(void)
{

    if(__HAL_UART_GET_IT(&huart, UART_IT_RTO))
    {
        __HAL_UART_CLEAR_FLAG(&huart, UART_CLEAR_RTOF);    
        HAL_UART_RxTimoCallback(&huart);
    }
    
    if(__HAL_UART_GET_IT(&huart, UART_IT_TC) && __HAL_UART_GET_IT_SOURCE(&huart, UART_IT_TC))
    {
        CLEAR_BIT(huart.Instance->CR1, USART_CR1_TCIE);
        huart.gState = HAL_UART_STATE_READY;
        HAL_UART_TxCpltCallback(&huart);
    }
    
    if(__HAL_UART_GET_IT(&huart, UART_IT_PE))
    {
        __HAL_UART_CLEAR_FLAG(&huart, UART_CLEAR_PEF);    
    }
    
    if(__HAL_UART_GET_IT(&huart, UART_IT_FE))
    {
        __HAL_UART_CLEAR_FLAG(&huart, UART_CLEAR_FEF);    
    }
    
    if(__HAL_UART_GET_IT(&huart, UART_IT_NE))
    {
        __HAL_UART_CLEAR_FLAG(&huart, UART_CLEAR_NEF);    
    }
    
    if(__HAL_UART_GET_IT(&huart, UART_IT_ORE))
    {
        __HAL_UART_CLEAR_FLAG(&huart, UART_CLEAR_OREF);    
    }
}

