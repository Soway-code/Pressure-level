#ifndef __PICOCAP_APP_H
#define __PICOCAP_APP_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "Picocap.h"
#include "In_Flash_app.h"


/* PCap采集周期，单位 ms */
#define PCAP_COLLECT_CYCLE              100

#define DATA_BUF_MAX                    10

#define DATA_2ND_FILTER_BUF_MAX         10

#define DATA_1ST_FILTER_BUF_MAX         96

#define PCAP_DAC_MIN_VALUE              0

#define PCAP_DAC_MAX_VALUE              4095

#define PCAP_ADC_MIN_VALUE              0

#define PCAP_ADC_MAX_VALUE              65535

/* 数据滤波需要的参数结构 */
typedef struct {    
    uint8_t FilterStart;                //滤波开始标志
    uint8_t InputCountMax;              //累计输入的最大值，上限为DATA_BUF_MAX
    uint8_t FilterFactor;               //滤波系数
    uint8_t FilterBufMax;               //滤波最大缓存
    uint16_t FilterCycle;               //滤波周期
    uint32_t InputRangeMax;             //输入范围最大值
    uint32_t InputRangeMin;             //输入范围最小值 
}DataFilterParam;
    
/* PCap做数据转换需要的参数结构 */
typedef struct {
    uint8_t CompenEn;                   //补偿使能
    uint16_t HeightRange;               //高度量程
    uint16_t CapADMin;                  //电容AD值零点
    uint16_t CapADLow;                  //电容AD值下刻度
    uint16_t CapADHigh;                 //电容AD值上刻度
    uint16_t CapADMax;                  //电容AD值满量程
    uint16_t CapDAMin;                  //电容DA值零点
    uint16_t CapDALow;                  //电容DA值下刻度
    uint16_t CapDAHigh;                 //电容DA值上刻度
    uint16_t CapDAMax;                  //电容DA值满量程
    uint32_t CapMin;                    //电容零点
    uint32_t CapMax;                    //电容满量程
    uint32_t CapMinBak;                 //电容零点
    uint32_t CapMaxBak;                 //电容满量程
    uint8_t CapDA_ClibEn;               //电容DA标定使能
    float Correct_K;                    //电容修正系数K
    float Correct_B;                    //电容修正系数B
}PCap_DataConvert_Param;

/* PCap转换后输出数据的结构 */
typedef struct {
    uint16_t LiquidHeightAD;            //液位高度AD值
    float LiquidHeight;                 //液位高度
    uint16_t PCapDA_ResultValue;        //电容DA原始输出值
    uint16_t PCapDA_OutValue;           //电容DA转换输出值
    uint32_t PCap_ResultValue;          //PCap原始值
}PCap_DataConvert_Out_Param;

/* 使用RT-Thread操作系统,USING_RT_THREAD_OS在main.h中定义 */
#ifdef USING_RT_THREAD_OS
#include <rtthread.h>

#define PCAP_DEVICE_NAME        "pcap"

struct rt_pcap_device_obj {
    struct rt_device            dev;
    DataFilterParam             DataFilter;
    PCap_DataConvert_Param      PCap_DataConvert;
    PCap_DataConvert_Out_Param  PCap_DataConvert_Out;
};
#endif


/**
* 名称       : SwitchCurFilter()
* 创建日期   : 2020-04-30
* 作者       : 杨春林
* 功能       : 切换滤波水平函数
* 输入参数   : FiltFactor(滤波系数)
* 输出参数   : FilterParam,指定滤波参数结构体;
* 返回结果   : 无
* 注意和说明 : 滤波水平分9级，数字从低到高对应滤波深度从低到高
* 修改内容   :
*/
void SwitchCurFilter(uint8_t FiltFactor, DataFilterParam *FilterParam);

#ifdef __IN_FLASH_APP_H
/**
* 名称       : DataFilterParam_Init()
* 创建日期   : 2020-05-05
* 作者       : 杨春林
* 功能       : 初始化数据滤波需要的参数结构
* 输入参数   : Filterfactor_CountMax, 滤波系数和累计输入的最大值,滤波系数在高8位,
*              累计输入的最大值在低8位;
* 输出参数   : FilterParam,数据滤波需要的参数结构指针;
* 返回结果   : 无
* 注意和说明 : 要使用本函数,要加入In_Flash_app.c、In_Flash_app.h、
*              In_Flash.ch、In_Flash.h和文件
* 修改内容   :
*/
void DataFilterParam_Init(DataFilterParam *FilterParam, uint16_t Filterfactor_CountMax);
#else
/**
* 名称       : DataFilterParam_Init()
* 创建日期   : 2020-05-05
* 作者       : 杨春林
* 功能       : 初始化数据滤波需要的参数结构
* 输入参数   : Filterfactor_CountMax, 滤波系数和累计输入的最大值,滤波系数在高8位,
*              累计输入的最大值在低8位;
*              InputRangeMax, 输入范围最大值;
*              InputRangeMin, 输入范围最小值;
* 输出参数   : FilterParam,数据滤波需要的参数结构指针;
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   : 
*/
void DataFilterParam_Init(DataFilterParam *FilterParam, uint16_t Filterfactor_CountMax,
                            uint32_t InputRangeMax, uint32_t InputRangeMin);
#endif

#ifdef __IN_FLASH_APP_H
/**
* 名称       : DataConvertParam_Init()
* 创建日期   : 2020-05-05
* 作者       : 杨春林
* 功能       : 初始化数据转换需要的参数结构
* 输入参数   : 无
* 输出参数   : DataConvert_Param,数据转换需要的参数结构指针;
* 返回结果   : 无
* 注意和说明 : 要使用本函数,要加入In_Flash_app.c、In_Flash_app.h、
*              In_Flash.ch、In_Flash.h和文件
* 修改内容   :
*/
void DataConvertParam_Init(PCap_DataConvert_Param *DataConvert_Param);
#else
/**
* 名称       : DataConvertParam_Init()
* 创建日期   : 2020-05-05
* 作者       : 杨春林
* 功能       : 初始化数据转换需要的参数结构
* 输入参数   : CompenEn, 补偿使能;
*              Param, 要写入DataConvert_Param的参数数组指针;
*              Param_Size, Param的大小;
* 输出参数   : DataConvert_Param,数据转换需要的参数结构指针;
* 返回结果   : OP_SUCCESS(成功)，OP_FAILED(失败)
* 注意和说明 : Param的数据分布(按字节分布): 0~1,位移高度; 2~5,测量电容零点;
*              6~9,测量电容满点; 10~13,测量电容零点; 14~17,测量电容满点; 
*              18~19,电容AD值零点; 20~21,电容AD值下刻度; 22~23,电容AD值上刻度; 
*              24~25,电容AD值满量程; 26~27,电容DA值零点; 28~29,电容DA值下刻度; 
*              30~31,电容DA值上刻度; 32~33,电容DA值满量程; 34~35,电容修正系数K; 
*              36~37,电容修正系数B;
* 修改内容   :
*/
uint8_t DataConvertParam_Init(PCap_DataConvert_Param *DataConvert_Param, uint8_t CompenEn,
                            uint8_t *Param, uint16_t Param_Size);
#endif

/**
* 名称       : Sensor_PCap_GetResult()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 获取PCap原始采集值
* 输入参数   : reg_addr,结果寄存器的地址;
* 输出参数   : PCap_Result,保存PCap的输出结果
* 返回结果   : 操作成功/失败
* 注意和说明 : 
* 修改内容   : 
*/
uint8_t Sensor_PCap_GetResult(uint8_t reg_addr, uint32_t *PCap_Result);

/**
* 名称       : Sensor_DataFilter()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 数据按指定滤波参数进行滤波
* 输入参数   : FilterParam,指定滤波参数结构体;InputValue,输入的数据
* 输出参数   : OutputValue,滤波后的值
* 返回结果   : 操作成功/失败
* 注意和说明 : 使用本函数前,先将FilterParam初始化(即对它的每一个成员赋合适的值),否则不能正常使用
* 修改内容   : 
*/
uint8_t Sensor_DataFilter(DataFilterParam *FilterParam, uint32_t InputValue, uint32_t *OutputValue);

/**
* 名称       : Sensor_PCap_DataConvert()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 数据按指定控制参数进行数据转换
* 输入参数   : DataConvert_Param,指定控制参数结构体;InputValue,输入的数据
* 输出参数   : DataConvert_Out,输出数据的参数结构体
* 返回结果   : 无
* 注意和说明 : 使用本函数前,先将DataConvert_Param初始化(即对它的每一个成员赋合适的值),否则不能正常使用
* 修改内容   : 
*/
void Sensor_PCap_DataConvert(PCap_DataConvert_Param *DataConvert_Param, 
                                uint32_t InputValue, 
                                PCap_DataConvert_Out_Param *DataConvert_Out);

#ifdef __cplusplus
}
#endif
#endif
