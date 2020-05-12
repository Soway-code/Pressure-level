#ifndef __ADC_APP_H
#define __ADC_APP_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "adc_bsp.h"
#include "In_Flash_app.h"
#include "VariaType.h"

#define VREF30ADDR          ((uint32_t) 0x1FFFF7B8)             //30摄氏度ADC参考值地址
#define VREF110ADDR         ((uint32_t) 0x1FFFF7C2)             //110摄氏度ADC参考值地址

#define AD_GROUP_MAX        10                                  //每个通道采集数量，用户可以自定义
#define AD_CHANNEL_MAX      AD_CHANNEL_TOTAL                       //使能ADC通道数，用户可以自定义
#define AD_SEQBUFF_MAX      (AD_GROUP_MAX * AD_CHANNEL_MAX)     //ADC序列缓存大小

/* ADC温度处理需要的参数结构 */
typedef struct {
    float Temper_K1;                                            //温度1修正系数K1
    float Temper_B1;                                            //温度1修正系数B1
    float Temper_K2;                                            //温度2修正系数K2
    float Temper_B2;                                            //温度2修正系数B2
    uint16_t TempDAMin;                                         //温度DA值零点
    uint16_t TempDAMax;                                         //温度DA值满量程
    uint16_t TempDARange;                                       //温度DA值量程
}ADC_TemperParam_TypeDef;

/* ADC温度处理输出数据的结构 */
typedef struct {
    uint16_t TemperInAirAD;                                     //环境温度AD值
    uint16_t TemperInLiquidAD;                                  //液体温度AD值
    float TemperInAir;                                          //环境温度
    float TemperInLiquid;                                       //液体温度
}ADC_TemperOut_TypeDef;

#ifdef __IN_FLASH_APP_H
/**
* 名称       : Sensor_ADC_TemperParam_Init()
* 创建日期   : 2020-05-06
* 作者       : 杨春林
* 功能       : 初始化温度转换需要的参数结构
* 输入参数   : 无
* 输出参数   : ADC_TemperParam,温度转换需要的参数结构指针;
* 返回结果   : 无
* 注意和说明 : 要使用本函数,要加入In_Flash_app.c、In_Flash_app.h、
*              In_Flash.ch、In_Flash.h和文件
* 修改内容   :
*/
void Sensor_ADC_TemperParam_Init(ADC_TemperParam_TypeDef *ADC_TemperParam);
#else
/*
* 名称       : Sensor_ADC_TemperParam_Init()
* 创建日期   : 2020-05-06
* 作者       : 杨春林
* 功能       : 初始化温度转换需要的参数结构
* 输入参数   : Param, 要写入ADC_TemperParam的参数数组指针;
*              Param_Size, Param的大小;
* 输出参数   : ADC_TemperParam,温度转换需要的参数结构指针;
* 返回结果   : OP_SUCCESS(成功)，OP_FAILED(失败)
* 注意和说明 : Param的数据内容(按字节顺序排列,大端模式): 0~1,温度1修正系数K1; 
*              2~3,温度1修正系数B1; 4~5,温度2修正系数K2; 6~7,温度2修正系数B2;
*              8~9,温度DA转换零点值; 10~11,温度DA转换满点值;
* 修改内容   :
*/
uint8_t Sensor_ADC_TemperParam_Init(ADC_TemperParam_TypeDef *ADC_TemperParam, uint8_t *Param, 
                            uint16_t Param_Size);
#endif

/**
* 名称       : Sensor_ADC_Init()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : 初始化ADC，求出stm32芯片内部温度传感器的温度变化斜率，启动ADC的DMA传输
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Sensor_ADC_Init(void);

/**
* 名称       : Sensor_ADC_Data_Handler()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : ADC数据处理,把ADC的缓存数据全部保存到另一个数组里,保存完后置位ADC数据更新标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Sensor_ADC_Data_Handler(void);

/**
* 名称       : Sensor_ADC_GetChn_Value()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : 用户获取ADC通道上平均滤波后的数据
* 输入参数   : Channel_Num:通道号,指定获取的通道
* 输出参数   : 无
* 返回结果   : 平均滤波后的结果
* 注意和说明 : 
* 修改内容   :
*/
uint16_t Sensor_ADC_GetChn_Value(AD_CHANNEL_NUM AD_Channel_Num);

/**
* 名称       : Sensor_ADC_Get_TemperData()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : 获取ADC温度数据
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 温度值
* 注意和说明 : 温度计算可参考STM32F072数据手册
* 修改内容   :
*/
float Sensor_ADC_Get_TemperData(void);

/**
* 名称       : Sensor_ADC_Get_Updata_Flag()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : 获取ADC数据更新标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : ADC更新标志
* 注意和说明 : 
* 修改内容   :
*/
uint8_t Sensor_ADC_Get_Updata_Flag(void);
    
/**
* 名称       : Sensor_ADC_Clean_Updata_Flag()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : 清除ADC更新标志
* 输入参数   : 无
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Sensor_ADC_Clean_Updata_Flag(void);

#ifdef __cplusplus
}
#endif
#endif
