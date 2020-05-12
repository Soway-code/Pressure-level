/****************************************************************************************************************************************************************
** 版权:          2017-2027,深圳市信为科技发展有限公司
** 文件名:        SoftSpi.h
** 作者:          庄明群
** 版本:          V1.0.0
** 日期:          2017-05-09
** 描述:          软件模拟SPI通信
** 功能:          SPI驱动
*****************************************************************************************************************************************************************
** 修改者:        No
** 版本:          No
** 修改内容:      No
** 日期:          No
****************************************************************************************************************************************************************/

#ifndef __SOFTSPI_H
#define __SOFTSPI_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

//SPI片选
#define SPI_NSS_PIN                                      GPIO_PIN_15
//SPI片选端口
#define SPI_NSS_PORT                                     GPIOA

//SPI MOSI引脚
#define SPI_MOSI_PIN                                     GPIO_PIN_5
//SPI MOSI端口
#define SPI_MOSI_PORT                                    GPIOB

//SPI SCK引脚
#define SPI_SCK_PIN                                      GPIO_PIN_3
//SPI SCK端口
#define SPI_SCK_PORT                                     GPIOB

//SPI MISO引脚
#define SPI_MISO_PIN                                     GPIO_PIN_4
//SPI MISO端口
#define SPI_MISO_PORT                                    GPIOB

//SPI使能
#define SPI_ENABLE                                       HAL_GPIO_WritePin(SPI_NSS_PORT, SPI_NSS_PIN, GPIO_PIN_RESET)
//SPI禁止
#define SPI_DISABLE                                      HAL_GPIO_WritePin(SPI_NSS_PORT, SPI_NSS_PIN, GPIO_PIN_SET)

//SPI MOSI置高
#define SPI_MOSI_SET                                     HAL_GPIO_WritePin(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_PIN_SET)
//SPI MOSI置低
#define SPI_MOSI_CLR                                     HAL_GPIO_WritePin(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_PIN_RESET)

//SPI SCK置高
#define SPI_SCK_SET                                      HAL_GPIO_WritePin(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_PIN_SET)
//SPI SCK置低
#define SPI_SCK_CLR                                      HAL_GPIO_WritePin(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_PIN_RESET)
//SPI MISO读取电平
#define SPI_MISO_STA                                     HAL_GPIO_ReadPin(SPI_MISO_PORT, SPI_MISO_PIN)

//****************************************************************************************************************************************************************
// 名称               : SPI_Init()
// 创建日期           : 2017-05-09
// 作者               : 庄明群
// 功能               : SPI 通信接口初始化
// 输入参数           : 无
// 输出参数           : 无
// 返回结果           : 无
// 注意和说明         : 
// 修改内容           :
//****************************************************************************************************************************************************************
void SPI_Init(void);

//****************************************************************************************************************************************************************
// 名称               : SPI_Bit_Set()
// 创建日期           : 2017-05-09
// 作者               : 庄明群
// 功能               : SPI通信电平置位
// 输入参数           : 无
// 输出参数           : 无
// 返回结果           : 无
// 注意和说明         : 
// 修改内容           :
//****************************************************************************************************************************************************************

void SPI_Bit_Set(void);

//****************************************************************************************************************************************************************
// 名称               : SPI_Bit_Clr()
// 创建日期           : 2017-05-09
// 作者               : 庄明群
// 功能               : SPI通信电平置低
// 输入参数           : 无
// 输出参数           : 无
// 返回结果           : 无
// 注意和说明         : 
// 修改内容           :
//****************************************************************************************************************************************************************

void SPI_Bit_Clr(void);

//****************************************************************************************************************************************************************
// 名称               : Delay_us()
// 创建日期           : 2017-05-09
// 作者               : 庄明群
// 功能               : SPI通信微秒延时
// 输入参数           : us(延时参数)
// 输出参数           : 无
// 返回结果           : 无
// 注意和说明         : 
// 修改内容           :
//****************************************************************************************************************************************************************

void Delay_us(uint32_t us);

//****************************************************************************************************************************************************************
// 名称               : SPI_Receive_8Bit()
// 创建日期           : 2017-05-09
// 作者               : 庄明群
// 功能               : SPI通信接收一字节数据
// 输入参数           : 无
// 输出参数           : 无
// 返回结果           : 接收字节
// 注意和说明         : 
// 修改内容           :
//****************************************************************************************************************************************************************

uint8_t SPI_Receive_8Bit(void);

//****************************************************************************************************************************************************************
// 名称               : SPI_Receive_24Bit()
// 创建日期           : 2017-05-09
// 作者               : 庄明群
// 功能               : SPI通信接收3字节数据
// 输入参数           : 无
// 输出参数           : 无
// 返回结果           : 32位数据
// 注意和说明         : 
// 修改内容           :
//****************************************************************************************************************************************************************

uint32_t SPI_Receive_24Bit(void);

//****************************************************************************************************************************************************************
// 名称               : SPI_Send_8Bit()
// 创建日期           : 2017-05-09
// 作者               : 庄明群
// 功能               : SPI通信发送一字节数据
// 输入参数           : sBit8(待发送字节)
// 输出参数           : 无
// 返回结果           : 无
// 注意和说明         : 
// 修改内容           :
//****************************************************************************************************************************************************************

void SPI_Send_8Bit(uint8_t sBit8);

//****************************************************************************************************************************************************************
// 名称               : SPI_Send_16Bit()
// 创建日期           : 2017-05-09
// 作者               : 庄明群
// 功能               : SPI通信发送2字节数据
// 输入参数           : sBit16(待发送2字节)
// 输出参数           : 无
// 返回结果           : 无
// 注意和说明         : 
// 修改内容           :
//****************************************************************************************************************************************************************

void SPI_Send_16Bit(uint16_t sBit16);

//****************************************************************************************************************************************************************
// 名称               : SPI_Send_32Bit()
// 创建日期           : 2017-05-09
// 作者               : 庄明群
// 功能               : SPI通信发送4字节数据
// 输入参数           : sBit32(待发送4字节)
// 输出参数           : 无
// 返回结果           : 无
// 注意和说明         : 
// 修改内容           :
//****************************************************************************************************************************************************************

void SPI_Send_32Bit(uint32_t sBit32);

#ifdef __cplusplus
}
#endif
#endif
