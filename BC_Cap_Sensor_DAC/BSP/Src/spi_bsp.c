/****************************************************************************************************************************************************************
** 版权:          2020-2030,深圳市信为科技发展有限公司
** 文件名:        spi_bsp.c
** 作者:          杨春林
** 版本:          V1.0.0
** 日期:          2020-04-28
** 描述:          硬件SPI配置
** 功能:          硬件SPI初始化, 为SPI数据收发提供底层函数
*****************************************************************************************************************************************************************
** 修改者:        No
** 版本:          No
** 修改内容:      No
** 日期:          No
****************************************************************************************************************************************************************/

#include "spi_bsp.h"

/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS
#include "board.h"
#endif

static SPI_HandleTypeDef hspi1;

/**
* 名称       : BSP_SPI_Init()
* 创建日期   : 2020-05-20
* 作者       : 杨春林
* 功能       : SPI初始化
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void BSP_SPI_Init(void)
{

  hspi1.Instance = USER_SPI;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    USER_SPI_SCK_CLK_ENABLE();
  
    USER_SPI_SCK_PIN_CLK_ENABLE();
    USER_SPI_MISO_PIN_CLK_ENABLE();
    USER_SPI_MOSI_PIN_CLK_ENABLE();

    GPIO_InitStruct.Pin = USER_SPI_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(USER_SPI_SCK_PIN_GPIOX, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = USER_SPI_MISO_PIN;
    HAL_GPIO_Init(USER_SPI_MISO_PIN_GPIOX, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = USER_SPI_MOSI_PIN;
    HAL_GPIO_Init(USER_SPI_MOSI_PIN_GPIOX, &GPIO_InitStruct);
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
    USER_SPI_SCK_CLK_DISABLE();
     
    HAL_GPIO_DeInit(USER_SPI_SCK_PIN_GPIOX, USER_SPI_SCK_PIN);
    HAL_GPIO_DeInit(USER_SPI_MISO_PIN_GPIOX, USER_SPI_MISO_PIN);
    HAL_GPIO_DeInit(USER_SPI_MOSI_PIN_GPIOX, USER_SPI_MOSI_PIN);
} 

/**
* 名称       : BSP_SPI_Transmit()
* 创建日期   : 2020-05-20
* 作者       : 杨春林
* 功能       : SPI发送数据
* 输入参数   : pData：发送缓存指针;
*              Size：缓存大小
* 输出参数   : 无
* 返回结果   : OP_SUCCESS(成功),OP_FAILED(失败)
* 注意和说明 : 
* 修改内容   :
*/
uint8_t BSP_SPI_Transmit(uint8_t *pData, uint16_t Size)
{
    if(HAL_SPI_Transmit(&hspi1, pData, Size, USER_SPI_TIMEOUT) != HAL_OK)
    {
        return OP_FAILED;
    }
    return OP_SUCCESS;
}

/**
* 名称       : BSP_SPI_Receive()
* 创建日期   : 2020-05-20
* 作者       : 杨春林
* 功能       : SPI接收数据
* 输入参数   : pData：接收缓存指针;
*              Size：缓存大小
* 输出参数   : 无
* 返回结果   : OP_SUCCESS(成功),OP_FAILED(失败)
* 注意和说明 : 
* 修改内容   :
*/
uint8_t BSP_SPI_Receive(uint8_t *pData, uint16_t Size)
{
    if(HAL_SPI_Receive(&hspi1, pData, Size, USER_SPI_TIMEOUT) != HAL_OK)
    {
        return OP_FAILED;
    }
    return OP_SUCCESS;
}

/**
* 名称       : BSP_SPI_Receive()
* 创建日期   : 2020-05-20
* 作者       : 杨春林
* 功能       : SPI发送并接收数据
* 输入参数   : TxData：发送缓存指针; RxData: 接收缓存指针
*              Size：缓存大小
* 输出参数   : 无
* 返回结果   : OP_SUCCESS(成功),OP_FAILED(失败)
* 注意和说明 : 
* 修改内容   :
*/
uint8_t BSP_SPI_TransmitReceive(uint8_t *TxData, uint8_t *RxData, uint16_t Size)
{
    if(HAL_SPI_TransmitReceive(&hspi1, TxData, RxData, Size, USER_SPI_TIMEOUT) != HAL_OK)
    {
        return OP_FAILED;
    }
    return OP_SUCCESS;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
