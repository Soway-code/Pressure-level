/****************************************************************************************************************************************************************
** 版权:          2020-2030,深圳市信为科技发展有限公司
** 文件名:        adc_app.c
** 作者:          杨春林
** 版本:          V1.0.0
** 日期:          2020-05-06
** 描述:          ADC模块的应用
** 功能:          ADC DMA 功能配置 ADC温度数据获取与处理
*****************************************************************************************************************************************************************
** 修改者:        No
** 版本:          No
** 修改内容:      No
** 日期:          No
****************************************************************************************************************************************************************/

#include "adc_app.h"

static uint16_t Temp30_Vref_Adc;                                //30摄氏度ADC基准值
static uint16_t Temp110_Vref_Adc;                               //110摄氏度ADC基准值
static float TempVarySlope;                                     //温度变化斜率
static uint16_t gADC_GetData[AD_GROUP_MAX][AD_CHANNEL_MAX];     //ADC的DMA数据缓存区
static uint16_t uADC_GetResult[AD_GROUP_MAX][AD_CHANNEL_MAX];   //ADC结果缓存,用于数据处理

static uint8_t ADC_Cplt_Flag = 0;                               //ADC完成标志
static uint8_t ADC_Updata_Flag = 0;                             //ADC数据更新标志


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
void Sensor_ADC_TemperParam_Init(ADC_TemperParam_TypeDef *ADC_TemperParam)
{
    uint8_t Read_Data[4];
    
    InFlash_Read_MultiBytes(TEMPER_K1, Read_Data, 2);
    ADC_TemperParam->Temper_K1 = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InFlash_Read_MultiBytes(TEMPER_B1, Read_Data, 2);
    ADC_TemperParam->Temper_B1 = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InFlash_Read_MultiBytes(TEMPER_K2, Read_Data, 2);
    ADC_TemperParam->Temper_K2 = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InFlash_Read_MultiBytes(TEMPER_B2, Read_Data, 2);
    ADC_TemperParam->Temper_B2 = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InFlash_Read_MultiBytes(TEMPDAMIN, Read_Data, 2);
    ADC_TemperParam->TempDAMin = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InFlash_Read_MultiBytes(TEMPDAMAX, Read_Data, 2);
    ADC_TemperParam->TempDAMax = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];   
}

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
* 注意和说明 : Param的数据内容(按字节顺序排列): 0~1,温度1修正系数K1; 
*              2~3,温度1修正系数B1; 4~5,温度2修正系数K2; 6~7,温度2修正系数B2;
*              8~9,温度DA转换零点值; 10~11,温度DA转换满点值;
* 修改内容   :
*/
uint8_t Sensor_ADC_TemperParam_Init(ADC_TemperParam_TypeDef *ADC_TemperParam, uint8_t *Param, 
                            uint16_t Param_Size)
{
    uint8_t cnt = 0;

    if(Param != NULL && Param_Size != 0)
    {
        ADC_TemperParam->Temper_K1 = (uint16_t)(Param[cnt] << 8) | Param[cnt + 1];   
        cnt += 2;
        ADC_TemperParam->Temper_B1 = (uint16_t)(Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        ADC_TemperParam->Temper_K2 = (uint16_t)(Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        ADC_TemperParam->Temper_B2 = (uint16_t)(Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        ADC_TemperParam->TempDAMin = (uint16_t)(Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        ADC_TemperParam->TempDAMax = (uint16_t)(Param[cnt] << 8) | Param[cnt + 1]; 
        
        if(cnt > Param_Size)
        {
            return OP_FAILED;
        }
        return OP_SUCCESS;
    }
    return OP_FAILED;
}
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
void Sensor_ADC_Init(void)
{    
    //ADC驱动初始化
    BSP_ADC_Init();
    //读取30摄氏度ADC基准值
    Temp30_Vref_Adc = *(__IO uint16_t *)VREF30ADDR;
    //读取110摄氏度ADC基准值
    Temp110_Vref_Adc = *(__IO uint16_t *)VREF110ADDR;
    //温度变化斜率计算
    TempVarySlope = 80.0 / (float)(Temp110_Vref_Adc - Temp30_Vref_Adc);
    //开始ADC DMA传输
    BSP_ADC_Start_DMA((uint32_t *)&gADC_GetData[0][0],  AD_SEQBUFF_MAX);
}

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
void Sensor_ADC_Data_Handler(void)
{
    uint8_t Ad_group;
    uint8_t Ad_Channel;
    static uint16_t DMA_TC_Cnt = 0;
    //ADC传输完成才会处理数据
    if(ADC_Cplt_Flag)
    {
        ADC_Cplt_Flag = 0;
        DMA_TC_Cnt++;

        if(AD_SEQBUFF_MAX <= DMA_TC_Cnt)
        {
            //获取全部ADC通道的数据
            for(Ad_Channel = 0; Ad_Channel < AD_CHANNEL_MAX; Ad_Channel++)
            {
                for(Ad_group = 0; Ad_group < AD_GROUP_MAX; Ad_group++)
                {
                    uADC_GetResult[Ad_group][Ad_Channel] = gADC_GetData[Ad_group][Ad_Channel];
                }
            }
            ADC_Updata_Flag = UPDATA_OK;
            DMA_TC_Cnt = 0;
        }
    }
}

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
uint16_t Sensor_ADC_GetChn_Value(AD_CHANNEL_NUM AD_Channel_Num)
{
    uint8_t cn;
    uint32_t AD_Sum = 0;

    if(AD_Channel_Num >= AD_CHANNEL_MAX)
        return 0;
    
    for(cn = 0; cn < AD_GROUP_MAX; cn++)
    {
        //对通道的所有数据累加
        AD_Sum += uADC_GetResult[cn][AD_Channel_Num];
    }
    //计算平均值
    return AD_Sum / AD_GROUP_MAX;
}

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
float Sensor_ADC_Get_TemperData(void)
{
    uint8_t cn;
    float Result;
    uint32_t AD_Sum = 0;
    uint16_t AD_Average;

    for(cn = 0; cn < AD_GROUP_MAX; cn++)
    {
        //对温度通道的所有数据累加
        AD_Sum += uADC_GetResult[cn][AD_CHANNEL_TEMPSENSOR];
    }
    //计算平均值
    AD_Average = AD_Sum / AD_GROUP_MAX;
    //根据温度变化斜率得到实际温度
    Result = TempVarySlope * (AD_Average - Temp30_Vref_Adc) + 30;
    
    return Result;
}

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
uint8_t Sensor_ADC_Get_Updata_Flag(void)
{
    return ADC_Updata_Flag;
}

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
void Sensor_ADC_Clean_Updata_Flag(void)
{
    ADC_Updata_Flag = UPDATA_NONE;
}

/**
* 名称       : HAL_ADC_ConvCpltCallback()
* 创建日期   : 2020-04-26
* 作者       : 杨春林
* 功能       : ADC转换完成回调，这里仅置位ADC完成标志
* 输入参数   : ADC处理对象指针
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    ADC_Cplt_Flag = 1;
}
