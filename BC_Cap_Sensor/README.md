# README 

## 简介

本文档为 电容传感器的 主要硬件外设 和 程序(BSP) 说明。

主要内容如下：

- 目前支持的主控芯片
- 驱动程序介绍

通过阅读本文档可以大概地了解电容程序的运行流程

## 支持的主控芯片

目前，本程序工程支持的主控芯片为：
ST公司的
STM32F0系列
STM32L0系列
本公司目前在电容传感器上使用主控芯片为两种，分别为 STM32F072CB, STM32L072CB

更多详细信息请参考 ST 的 官方网站(https://www.st.com/)。

## 驱动程序

本 BSP 目前对外设的支持情况如下：

| **片上外设**      | **支持情况** | **备注**                              |
| :----------------- | :----------: | :------------------------------------- |
| UART              |     支持     | USART1                            |
| SPI               |     支持     | SPI1                              |
| ADC | 支持 |  |
| DAC | 支持 |  |
| RTC               |   支持   | 支持内部高速时钟 |
| FLASH | 支持 |  |
| IWG               |   支持   |                               |

其中UART、SPI、ADC、DAC、FLASH(STM32L0系列为内部EEPROM)、IWG都已封装成模块，
底层配置与上层应用是分开的。

UART： STM32F0系列 和 STM32L0系列 有针对ModBus的2个事件处理，这2个事件
分别为接收数据超时事件，接收到特定字符事件，可通过配置UART，将这2种事件分别
作为 RTU 和 ASCII 协议的数据帧接收完成标志。例如：使用RTU协议时，配置数据接
收超时时间为 3.5 个字符(的传输)时间作为接收完成标志；使用ASCII协议时，配置
接收到换行符 '/n'作为接收完成标志。数据的发送和接收都使用DMA方式。

SPI： 封装了初始化、数据读写的API函数，使用时，直接调用这些API函数，数据的发送
和接收都使用普通方式。

ADC： 可配置ADC采集通道，通道有：通道0、1、2、3、6、7、8、9、内部温度传感器通道，
数据传输使用DMA方式。

DAC： 可配置DAC转换通道，通道有：通道1、2，数据传输使用普通方式。

FLASH(或EEPROM)：封装了读、写和擦除的API函数，使用时，直接调用这些API函数，可
实现 1 字节或多字节的读写的功能，以及指定某个区域进行擦除。

IWG： 封装了初始化和喂狗(看门狗定时器重装载)的API函数，使用时，直接调用这些API
函数，可实现初始化独立看门狗和喂狗的功能。

重要的配置宏： 
- IN_EEPROM_BASE_ADDRESS----(操作内部EEPROM基地址)
- IN_EEPROM_START-----------(EEPROM起始地址)
- IN_EEPROM_END-------------(EEPROM结束地址)
- IN_FLASH_BASE_ADDRESS-----(操作内部FLASH基地址)
- IN_FLASH_START------------(FLASH起始地址)
- IN_FLASH_END--------------(FLASH结束地址)
- USING_UART_TIMEOUT--------(使用接收超时)
- USING_CHARMATCH-----------(使用字符匹配)
- USART_USING_485-----------(串口485收发控制管脚使能)
- DEFAULT_UART_TIMEOUT------(默认超时时间，一般, 10表示1个字节时间)
- DEFAULT_UART_MATCHCHAR----(默认匹配的字符)
- USING_MODBUS_RTU----------(使用ModBus RTU协议, 还需要定义USING_UART_TIMEOUT)
- USING_MODBUS_ASCII--------(使用ModBus ASCII协议, 还需要定义USING_CHARMATCH)
- SUBCODE_IS_DEVADDR--------(定义ModBus子码为设备地址，不定义则默认为0)
- AUTOUPLOAD_CYCLE----------(自动上传周期,单位 ms)

## 注意事项

- UART 的 ModBus 的2个事件处理功能在STM32F0、STM32L0系列上支持，查阅手册发现，
目前，STM32F1、STM32F4系列没有这2个功能。

## 联系人信息

维护人: 杨春林

-  QQ：371921187
-  邮箱：371921187@qq.com