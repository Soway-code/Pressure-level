/**@file        spi_bsp.h
* @details      spi_bsp.c��ͷ�ļ�,������Ӳ��SPI��API����
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

#ifndef __SPI_BSP_H
#define __SPI_BSP_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "main.h"
#include "VariaType.h"

#define SPIx                            SPI1
#define SPIx_CLK_ENABLE()               __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()

#define SPIx_CLK_DISABLE()              __HAL_RCC_SPI1_CLK_DISABLE()

/* Definition for SPIx Pins */
#define SPIx_SCK_PIN                    GPIO_PIN_3
#define SPIx_SCK_GPIO_PORT              GPIOB
#define SPIx_SCK_AF                     GPIO_AF0_SPI1
#define SPIx_MISO_PIN                   GPIO_PIN_4
#define SPIx_MISO_GPIO_PORT             GPIOB
#define SPIx_MISO_AF                    GPIO_AF0_SPI1
#define SPIx_MOSI_PIN                   GPIO_PIN_5
#define SPIx_MOSI_GPIO_PORT             GPIOB
#define SPIx_MOSI_AF                    GPIO_AF0_SPI1

#define USER_SPI_TIMEOUT                3000


/**@brief       SPI��ʼ��
* @return       ����ִ�н��
* - None
*/
void BSP_SPI_Init(void);

/**@brief       SPI��������
* @param[in]    pData : ���ͻ���ָ��;
* @param[in]    Size : �����С
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t BSP_SPI_Transmit(uint8_t *pData, uint16_t Size);

/**@brief       SPI��������
* @param[in]    pData : ���ջ���ָ��;
* @param[in]    Size : �����С
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t BSP_SPI_Receive(uint8_t *pData, uint16_t Size);

/**@brief       SPI���Ͳ���������
* @param[in]    TxData : ���ͻ���ָ��;
* @param[out]   RxData : ���ջ���ָ��;
* @param[in]    Size : ���ͺͽ��ջ���Ĵ�С
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t BSP_SPI_TransmitReceive(uint8_t *TxData, uint8_t *RxData, uint16_t Size);


#ifdef __cplusplus
}
#endif
#endif // __SPI_BSP_H
