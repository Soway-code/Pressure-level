/**@file        spi_bsp.c
* @brief        Ӳ��SPI����
* @details      Ӳ��SPI��ʼ��, ΪSPI�����շ��ṩAPI����
* @author       ���
* @date         2020-05-20
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/05/20  <td>1.0.0    <td>���    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#include "spi_bsp.h"

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
#include "board.h"
#endif

static SPI_HandleTypeDef hspi1;     ///< SPI�������


/**@brief       SPI��ʼ��
* @return       ����ִ�н��
* - None
*/
void BSP_SPI_Init(void)
{

  hspi1.Instance = SPIx;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
#if defined(STM32L0)
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
#else
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
#endif
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
#if defined(STM32F0)
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
#endif
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

/**@brief       SPIʱ�ӡ��ײ�ܽų�ʼ��(��HAL_ADC_Init�ص�����)
* @param[in]    spiHandle, SPI�������
* @return       ����ִ�н��
* - None
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    SPIx_CLK_ENABLE();
  
    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_GPIO_CLK_ENABLE();
    SPIx_MOSI_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = SPIx_SCK_AF;
    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = SPIx_MISO_PIN;
    GPIO_InitStruct.Alternate = SPIx_MISO_AF;
    HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
    HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);
}

/**@brief       SPIʱ�ӡ��ײ�ܽŷ���ʼ��(��HAL_ADC_DeInit�ص�����)
* @param[in]    spiHandle, SPI�������
* @return       ����ִ�н��
* - None
*/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
    SPIx_CLK_DISABLE();
     
    HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
    HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
    HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);
} 

/**@brief       SPI��������
* @param[in]    pData : ���ͻ���ָ��;
* @param[in]    Size : �����С
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t BSP_SPI_Transmit(uint8_t *pData, uint16_t Size)
{
    if(HAL_SPI_Transmit(&hspi1, pData, Size, USER_SPI_TIMEOUT) != HAL_OK)
    {
        return OP_FAILED;
    }
    return OP_SUCCESS;
}

/**@brief       SPI��������
* @param[in]    pData : ���ջ���ָ��;
* @param[in]    Size : �����С
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t BSP_SPI_Receive(uint8_t *pData, uint16_t Size)
{
    if(HAL_SPI_Receive(&hspi1, pData, Size, USER_SPI_TIMEOUT) != HAL_OK)
    {
        return OP_FAILED;
    }
    return OP_SUCCESS;
}

/**@brief       SPI���Ͳ���������
* @param[in]    TxData : ���ͻ���ָ��;
* @param[out]   RxData : ���ջ���ָ��;
* @param[in]    Size : ���ͺͽ��ջ���Ĵ�С
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t BSP_SPI_TransmitReceive(uint8_t *TxData, uint8_t *RxData, uint16_t Size)
{
    if(HAL_SPI_TransmitReceive(&hspi1, TxData, RxData, Size, USER_SPI_TIMEOUT) != HAL_OK)
    {
        return OP_FAILED;
    }
    return OP_SUCCESS;
}

