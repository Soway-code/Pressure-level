/**@file        usart_bsp.c
* @brief        ��������
* @details      ͨ�ô��ڳ�ʼ��, Ϊ���������շ��ṩAPI����
* @author       ���
* @date         2020-04-28
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/04/28  <td>1.0.0    <td>���    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#include "usart_bsp.h"

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
#include "board.h"
#endif


/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart;        ///< ���ڴ������
static DMA_HandleTypeDef hdma_usart_rx; ///< ���ڽ���DMA�������
static DMA_HandleTypeDef hdma_usart_tx; ///< ���ڷ���DMA�������


/**@brief       ���ڳ�ʼ��
* @param[in]    baudrate : ������; 
* @param[in]    parity : ��żУ��;
* @return       ����ִ�н��
* - None
*/
void BSP_USART_UART_Init(uint32_t baudrate, uint32_t parity)
{
    huart.Instance = USARTx;
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

/**@brief       ���ڷ���ʼ��
* @return       ����ִ�н��
* - None
*/
void BSP_USART_UART_DeInit(void)
{
    if (HAL_UART_DeInit(&huart) != HAL_OK)
    {
        Error_Handler();
    }
}

/**@brief       ����ʱ�ӡ��ײ�ܽų�ʼ��(��HAL_UART_Init�ص�����)
* @param[in]    uartHandle : ���ڴ������
* @return       ����ִ�н��
* - None
* @note         ���ڵ�ʱ�ӡ��ܽš��жϷ��������͵���usart_bsp.h�ļ����涨��
*/
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    DMAx_CLK_ENABLE();    
    
    USARTx_CLK_ENABLE();    
  
    USARTx_TX_GPIO_CLK_ENABLE();
    USARTx_RX_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = USARTx_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = USARTx_TX_AF;
    HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
      
    GPIO_InitStruct.Pin = USARTx_RX_PIN;
    GPIO_InitStruct.Alternate = USARTx_RX_AF;
    HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart_rx.Instance = USARTx_RX_DMA_STREAM;
#if defined(STM32L0)
    hdma_usart_rx.Init.Request = USARTx_RX_DMA_REQUEST;
#endif
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
    hdma_usart_tx.Instance = USARTx_TX_DMA_STREAM;
#if defined(STM32L0)
    hdma_usart_tx.Init.Request = USARTx_TX_DMA_REQUEST;
#endif
    hdma_usart_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart_tx.Init.Mode = DMA_NORMAL;
    hdma_usart_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_usart_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart_tx);

    /* USART1 interrupt Init */    
    HAL_NVIC_SetPriority(USARTx_DMA_TX_RX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USARTx_DMA_TX_RX_IRQn);
    
    HAL_NVIC_SetPriority(USARTx_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USARTx_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
}

/**@brief       ����ʱ�ӡ��ײ�ܽŷ���ʼ��(��HAL_UART_DeInit�ص�����)
* @param[in]    uartHandle : ���ڴ������
* @return       ����ִ�н��
* - None
* @note         ���ڵ�ʱ�ӡ��ܽš��жϷ��������͵���usart_bsp.h�ļ����涨��
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

    USARTx_CLK_DISABLE();
  
    HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
    HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USARTx_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */

} 

/**@brief       ����DMA����
* @param[in]    pData : DMA���ͻ���ָ��;
* @param[in]    Size : �����С
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t BSP_UART_Transmit_DMA(uint8_t *pData, uint16_t Size)
{
    if(HAL_UART_Transmit_DMA(&huart, pData, Size) != HAL_OK)
    {
        return OP_FAILED;
    }
    return OP_SUCCESS;
}

/**@brief       ����DMA����
* @param[in]    pData : DMA���ջ���ָ��;
* @param[in]    Size : �����С
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t BSP_UART_Receive_DMA(uint8_t *pData, uint16_t Size)
{
    if(HAL_UART_Receive_DMA(&huart, pData, Size) != HAL_OK)
    {
        return OP_FAILED;
    }
    return OP_SUCCESS;
}

/**@brief       �������ý��ճ�ʱ�ж�
* @param[in]    TimeoutValue : ��ʱʱ�䣬��λ������λ��;
* @return       ����ִ�н��
* - None
* @note         ʾ����1����ʼλ��8λ���ݣ�1��ֹͣλ�Ĵ������ã�
* ��10λΪ1�������ֽڣ�������4�������ֽڵĳ�ʱʱ�䣬
* TimeoutValue = 40
*/
void BSP_UART_ReceiverTimeout_Config(uint32_t TimeoutValue)
{
    LL_USART_DisableRxTimeout(huart.Instance);
    LL_USART_DisableIT_RTO(huart.Instance);
    
    LL_USART_SetRxTimeout(huart.Instance, TimeoutValue);
    
    LL_USART_EnableRxTimeout(huart.Instance);
    LL_USART_EnableIT_RTO(huart.Instance);
}

/**@brief       ���������ַ�ƥ���ж�
* @param[in]    Value : ƥ����ַ�;
* @return       ����ִ�н��
* - None
*/
void BSP_UART_CharMatch_Config(uint8_t Value)
{
    uint32_t USART_Status;
    
    assert_param(IS_UART_ADDRESSLENGTH_DETECT(WakeUpSelection.AddressLength));
    
    USART_Status = LL_USART_IsEnabled(huart.Instance);
    if(USART_Status)
    {
        LL_USART_Disable(huart.Instance);
    }
    LL_USART_DisableIT_CM(huart.Instance);
  
    /* ������������ַ����Ⱥ�ƥ����ַ�����������Ϊ8λ���ȵ��ַ� */
    LL_USART_ConfigNodeAddress(huart.Instance, LL_USART_ADDRESS_DETECT_7B, Value);  
    
    LL_USART_EnableIT_CM(huart.Instance);
    if(USART_Status)
    {
        LL_USART_Enable(huart.Instance);
    }
}

/**@brief       ���ڽ��ճ�ʱ�жϻص�����
* @param[in]    huart : ���ڴ������;
* @return       ����ִ�н��
* - None
* @note         �ú����Ǹ������庯�������������ļ����ض��壬�����Զ����һ����������HAL���в����ڵ�
*/
__weak void HAL_UART_RxTimoCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}

/**@brief       �����ַ�ƥ���жϻص�����
* @param[in]    huart : ���ڴ������;
* @return       ����ִ�н��
* - None
* @note         �ú����Ǹ������庯�������������ļ����ض��壬�����Զ����һ����������HAL���в����ڵ�
*/
__weak void HAL_UART_CMatchCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}

/**@brief       ����DMA�жϷ�����
* @return       ����ִ�н��
* - None
* @note         USARTx_DMA_TX_RX_IRQHandler��usart_bsp.h�ж���
*/
void USARTx_DMA_TX_RX_IRQHandler(void)
{
/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
    rt_interrupt_enter();
#endif
    
    HAL_DMA_IRQHandler(&hdma_usart_tx);
    HAL_DMA_IRQHandler(&hdma_usart_rx);

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
    rt_interrupt_leave();
#endif
}

/**@brief       �����жϷ�����
* @return       ����ִ�н��
* - None
* @note         USARTx_IRQHandler��usart_bsp.h�ж���
*/
void USARTx_IRQHandler(void)
{
/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
    rt_interrupt_enter();
#endif
    
    if(LL_USART_IsActiveFlag_RTO(huart.Instance))
    {
        LL_USART_ClearFlag_RTO(huart.Instance);
        HAL_UART_RxTimoCallback(&huart);
    }
    
    if(__HAL_UART_GET_IT(&huart, UART_IT_CM))
    {
        __HAL_UART_CLEAR_FLAG(&huart, UART_CLEAR_CMF);    
        HAL_UART_CMatchCallback(&huart);
    }
    
    if(__HAL_UART_GET_IT(&huart, UART_IT_TC) && __HAL_UART_GET_IT_SOURCE(&huart, UART_IT_TC))
    {
        __HAL_UART_DISABLE_IT(&huart, UART_IT_TC);
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
    
/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
    rt_interrupt_leave();
#endif
}

