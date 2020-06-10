#ifndef __SPI_BSP_H
#define __SPI_BSP_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "main.h"
#include "VariaType.h"

#define USER_SPI                        SPI1
#define USER_SPI_ALTERNATE              GPIO_AF0_SPI1

/* USER CODE BEGIN Private defines */
#define USER_SPI_SCK_CLK_ENABLE()       __HAL_RCC_SPI1_CLK_ENABLE()
#define USER_SPI_SCK_CLK_DISABLE()      __HAL_RCC_SPI1_CLK_DISABLE()

#define USER_SPI_SCK_PIN_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define USER_SPI_MISO_PIN_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define USER_SPI_MOSI_PIN_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define USER_SPI_SCK_PIN                GPIO_PIN_3
#define USER_SPI_MISO_PIN               GPIO_PIN_4
#define USER_SPI_MOSI_PIN               GPIO_PIN_5
#define USER_SPI_SCK_PIN_GPIOX          GPIOB
#define USER_SPI_MISO_PIN_GPIOX         GPIOB
#define USER_SPI_MOSI_PIN_GPIOX         GPIOB

#define USER_SPI_TIMEOUT                3000


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
void BSP_SPI_Init(void);

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
uint8_t BSP_SPI_Transmit(uint8_t *pData, uint16_t Size);

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
uint8_t BSP_SPI_Receive(uint8_t *pData, uint16_t Size);

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
uint8_t BSP_SPI_TransmitReceive(uint8_t *TxData, uint8_t *RxData, uint16_t Size);


#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
