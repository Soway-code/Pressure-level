/**@file        Picocap_app.c
* @brief        PCapоƬӦ�ü������ݴ���
* @details      ��ȡPCapоƬ,�Զ��������ݽ����˲�,����ת��
* @author       ���
* @date         2020-04-30
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/04/30  <td>1.0.0    <td>���    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#include "Picocap_app.h"
#include "VariaType.h"
#include "TypeConvert.h"



/**@brief       �л��˲�ˮƽ����
* @param[in]    FiltFactor : �˲�ϵ��;
* @param[in]    FilterParam : ָ���˲������ṹ��;
* @return       ����ִ�н��
* - None
* @note         �˲�ˮƽ��9�������ִӵ͵��߶�Ӧ�˲���ȴӵ͵���
*/
void SwitchCurFilter(uint8_t FiltFactor, DataFilterParam *FilterParam)
{  
    switch(FiltFactor)
    {
    case 0:
        FilterParam->FilterBufMax   = 1;
        FilterParam->FilterCycle    = 1;
    break;

    case 1:
        FilterParam->FilterBufMax   = 1;
        FilterParam->FilterCycle    = 12;
    break;

    case 2:
        FilterParam->FilterBufMax   = 2;
        FilterParam->FilterCycle    = 12;
    break;

    case 3:
        FilterParam->FilterBufMax   = 3;
        FilterParam->FilterCycle    = 12;
    break;

    case 4:
        FilterParam->FilterBufMax   = 4;
        FilterParam->FilterCycle    = 15;
    break;

    case 5:
        FilterParam->FilterBufMax   = 5;
        FilterParam->FilterCycle    = 24;
    break;

    case 6:
        FilterParam->FilterBufMax   = 6;
        FilterParam->FilterCycle    = 30;
    break;

    case 7:
        FilterParam->FilterBufMax   = 8;
        FilterParam->FilterCycle    = 30;
    break;

    case 8:
        FilterParam->FilterBufMax   = 8;
        FilterParam->FilterCycle    = 60;
    break;

    case 9:
        FilterParam->FilterBufMax   = 10;
        FilterParam->FilterCycle    = 96;
    break;

    default:
        FilterParam->FilterBufMax   = 1;
        FilterParam->FilterCycle    = 12;
    break;
    }

    FilterParam->FilterStart = 0;
}

#ifdef __IN_MEMORY_APP_H

/**@brief       ��ʼ�������˲���Ҫ�Ĳ����ṹ
* @param[in]    Filterfactor_CountMax : �˲�ϵ�����ۼ���������ֵ,�˲�ϵ���ڸ�8λ,
* �ۼ���������ֵ�ڵ�8λ;
* @param[out]   FilterParam : �����˲���Ҫ�Ĳ����ṹָ��;
* @return       ����ִ�н��
* - None
* @note         Ҫʹ�ñ�����,Ҫ����In_Memory_app.c��In_Memory_app.h��
* In_Flash.c��In_Flash.h�ļ�(STM32L0ϵ�������In_EEPROM.c��In_EEPROM.h�ļ�)
*/
void DataFilterParam_Init(DataFilterParam *FilterParam, uint16_t Filterfactor_CountMax)
{
    uint8_t Read_Data[4];
    
    InMemory_Read_MultiBytes(FILTER, Read_Data, 1);      //�˲�ϵ��
    FilterParam->FilterFactor   = Read_Data[0];
    SwitchCurFilter(Read_Data[0], FilterParam);         //�л��˲�ˮƽ
    FilterParam->InputCountMax = Filterfactor_CountMax; 
    InMemory_Read_MultiBytes(CAPMAX, Read_Data, 4);      //��������ֵ
    FilterParam->InputRangeMax = HexToUlong(Read_Data);
    InMemory_Read_MultiBytes(CAPMIN, Read_Data, 4);      //�������ֵ
    FilterParam->InputRangeMin = HexToUlong(Read_Data);
}
#else

/**@brief       ��ʼ�������˲���Ҫ�Ĳ����ṹ
* @param[in]    Filterfactor_CountMax : �˲�ϵ�����ۼ���������ֵ,�˲�ϵ���ڸ�8λ,
* �ۼ���������ֵ�ڵ�8λ;
* @param[in]    InputRangeMax : ���뷶Χ���ֵ;
* @param[in]    InputRangeMin : ���뷶Χ��Сֵ;
* @param[out]   FilterParam : �����˲���Ҫ�Ĳ����ṹָ��;
* @return       ����ִ�н��
* - None
*/
void DataFilterParam_Init(DataFilterParam *FilterParam, uint16_t Filterfactor_CountMax,
                            uint32_t InputRangeMax, uint32_t InputRangeMin)
{
    SwitchCurFilter(Filterfactor_CountMax >> 8, FilterParam);
    FilterParam->FilterFactor   = Filterfactor_CountMax >> 8;
    FilterParam->InputCountMax = Filterfactor_CountMax & 0xFF;
    FilterParam->InputRangeMax = InputRangeMax;
    FilterParam->InputRangeMin = InputRangeMin;
}
#endif

#ifdef __IN_MEMORY_APP_H

/**@brief       ��ʼ������ת����Ҫ�Ĳ����ṹ
* @param[out]   DataConvert_Param : ����ת����Ҫ�Ĳ����ṹָ��;
* @return       ����ִ�н��
* - None
* @note         Ҫʹ�ñ�����,Ҫ����In_Memory_app.c��In_Memory_app.h��
* In_Flash.c��In_Flash.h�ļ�(STM32L0ϵ�������In_EEPROM.c��In_EEPROM.h�ļ�)
*/
void DataConvertParam_Init(PCap_DataConvert_Param *DataConvert_Param)
{
    uint8_t Read_Data[4];
    
    InMemory_Read_MultiBytes(COMPENSATE, Read_Data, 1);      //����ʹ�ܱ�־
    DataConvert_Param->CompenEn = Read_Data[0];
    
    InMemory_Read_MultiBytes(HEIGHTRANGE, Read_Data, 2);     //Һ��߶ȷ�Χ
    DataConvert_Param->HeightRange = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InMemory_Read_MultiBytes(CAPMIN, Read_Data, 4);          //�������ֵ
    DataConvert_Param->CapMin = HexToUlong(Read_Data);
    
    InMemory_Read_MultiBytes(CAPMAX, Read_Data, 4);          //��������ֵ
    DataConvert_Param->CapMax = HexToUlong(Read_Data);
    
    InMemory_Read_MultiBytes(CAPMINBAK, Read_Data, 4);       //������㱸��ֵ
    DataConvert_Param->CapMinBak = HexToUlong(Read_Data);
    
    InMemory_Read_MultiBytes(CAPMAXBAK, Read_Data, 4);       //�������㱸��ֵ
    DataConvert_Param->CapMaxBak = HexToUlong(Read_Data);
    
    InMemory_Read_MultiBytes(CAPADMIN, Read_Data, 2);        //����AD���ֵ
    DataConvert_Param->CapADMin = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InMemory_Read_MultiBytes(CAPADLOW, Read_Data, 2);        //����AD�¿̶�ֵ
    DataConvert_Param->CapADLow = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InMemory_Read_MultiBytes(CAPADHIGH, Read_Data, 2);       //����AD�Ͽ̶�ֵ
    DataConvert_Param->CapADHigh = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InMemory_Read_MultiBytes(CAPADMAX, Read_Data, 2);        //����AD����ֵ
    DataConvert_Param->CapADMax = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InMemory_Read_MultiBytes(CAPDAMIN, Read_Data, 2);        //����DA���ֵ
    DataConvert_Param->CapDAMin = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InMemory_Read_MultiBytes(CAPDALOW, Read_Data, 2);        //����DA�¿̶�ֵ
    DataConvert_Param->CapDALow = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InMemory_Read_MultiBytes(CAPDAHIGH, Read_Data, 2);       //����DA�Ͽ̶�ֵ
    DataConvert_Param->CapDAHigh = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InMemory_Read_MultiBytes(CAPDAMAX, Read_Data, 2);        //����DA����ֵ
    DataConvert_Param->CapDAMax = (uint16_t)(Read_Data[0] << 8) | Read_Data[1];
    
    InMemory_Read_MultiBytes(CORRECT_K, Read_Data, 2);       //��������ϵ��K
    DataConvert_Param->Correct_K = (float)((Read_Data[0] << 8) | Read_Data[1]) / 100.0;
    
    InMemory_Read_MultiBytes(CORRECT_B, Read_Data, 2);       //��������ϵ��B
    DataConvert_Param->Correct_B = (float)((Read_Data[0] << 8) | Read_Data[1]) - 100.0;
    
    DataConvert_Param->CapDA_ClibEn = DAOUTCLIB_DISABLE;    //����DA�궨ʧ��
}
#else

/**@brief       ��ʼ������ת����Ҫ�Ĳ����ṹ
* @param[in]    CompenEn : ����ʹ��;
* @param[in]    Param : Ҫд��DataConvert_Param�Ĳ�������ָ��;
* @param[in]    Param_Size : Param�Ĵ�С;
* @param[out]   DataConvert_Param : ����ת����Ҫ�Ĳ����ṹָ��;
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         Param�����ݷֲ�(���ֽڷֲ�): 0~1,λ�Ƹ߶�; 2~5,�����������;
* 6~9,������������; 10~13,�����������; 14~17,������������; 18~19,����ADֵ���; 
* 20~21,����ADֵ�¿̶�; 22~23,����ADֵ�Ͽ̶�; 24~25,����ADֵ������; 
* 26~27,����DAֵ���; 28~29,����DAֵ�¿̶�; 30~31,����DAֵ�Ͽ̶�; 
* 32~33,����DAֵ������; 34~35,��������ϵ��K; 36~37,��������ϵ��B;
*/
uint8_t DataConvertParam_Init(PCap_DataConvert_Param *DataConvert_Param, uint8_t CompenEn,
                            uint8_t *Param, uint16_t Param_Size)
{
    uint8_t cnt = 0;
    
    DataConvert_Param->CompenEn = CompenEn;
    if(Param != NULL && Param_Size != 0)
    {
        DataConvert_Param->HeightRange = (Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;    
        DataConvert_Param->CapMin = HexToUlong(&Param[cnt]);
        cnt += 4;
        DataConvert_Param->CapMax = HexToUlong(&Param[cnt]);
        cnt += 4;
        DataConvert_Param->CapADMin = (Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        DataConvert_Param->CapADLow = (Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        DataConvert_Param->CapADHigh = (Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        DataConvert_Param->CapADMax = (Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        DataConvert_Param->CapDAMin = (Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        DataConvert_Param->CapDALow = (Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        DataConvert_Param->CapDAHigh = (Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;
        DataConvert_Param->CapDAMax = (Param[cnt] << 8) | Param[cnt + 1];
        cnt += 2;    
        DataConvert_Param->Correct_K = (float)((Param[cnt] << 8) | Param[cnt + 1]) / 100.0;
        cnt += 2;
        DataConvert_Param->Correct_B = (float)((Param[cnt] << 8) | Param[cnt + 1]) / 100.0;
        
        DataConvert_Param->CapDA_ClibEn = DAOUTCLIB_DISABLE;    //����DA�궨ʧ��
        
        if(cnt > Param_Size)
        {
            return OP_FAILED;
        }
        return OP_SUCCESS;
    }       
    return OP_FAILED;
}
#endif

/**@brief       ��ȡPCapԭʼ�ɼ�ֵ
* @param[in]    reg_addr : ����Ĵ����ĵ�ַ;
* @param[out]   PCap_Result : ����PCap��������;
* @param[in]    Read_Cnt : ��ȡ�ĵ��ݸ���;
* @return       ����ִ�н��
* - OP_SUCCESS(�����ɹ�)
* - OP_FAILED(����ʧ��)
*/
uint8_t Sensor_PCap_GetResult(uint8_t reg_addr, uint32_t *PCap_Result, uint8_t Read_Cnt)
{
    uint32_t PCap_Status;
    uint8_t Result = OP_FAILED;    
    uint8_t i;
    
    //��ȡPCap״̬
    PCap_Status = PCap_Res_Sta();
    if(PCap_Status & PCAP_RUNBIT)
    {
        for(i = 0; i < Read_Cnt; i++)
        {
            PCap_Result[i] = PCap_Res_Data(reg_addr + i);           
        }
        Result = OP_SUCCESS;
    }
    PCap_Measure();
            
    return Result;
}

/**@brief       ���ݰ�ָ���˲����������˲�
* @param[in]    FilterParam : ָ���˲������ṹ��;
* @param[in]    InputValue : ���������;
* @param[out]   OutputValue : ������ݵ�ָ��;
* @return       ����ִ�н��
* - OP_SUCCESS(�����ɹ�)
* - OP_FAILED(����ʧ��)
* @note         ʹ�ñ�����ǰ,�Ƚ�FilterParam��ʼ��(��������ÿһ����Ա�����ʵ�ֵ),����������ʹ��
*/
uint8_t Sensor_DataFilter(DataFilterParam *FilterParam, uint32_t InputValue, uint32_t *OutputValue)
{
    static uint32_t Data_1st_FilterBuf[DATA_1ST_FILTER_BUF_MAX];        //һ���˲�����
    static uint32_t Data_1st_FilterBufBak[DATA_1ST_FILTER_BUF_MAX];     //һ���˲����汸��
    static uint32_t Data_2nd_FilterBuf[DATA_2ND_FILTER_BUF_MAX];        //�����˲�����
    static uint32_t Data_2nd_FilterBufBak[DATA_2ND_FILTER_BUF_MAX];     //�����˲����汸��
    static uint32_t DataResBuf[DATA_BUF_MAX];                           //����ԭʼ����ֵ�Ļ���
    static uint8_t DataResBuf_Count;
    uint8_t i;
    uint32_t Data_Avg1;
    uint32_t Data_Avg2;
    uint32_t Data_Avg3;
    uint64_t Data_FilterSum;                                            //�˲������ۼƺ�
    
    if(InputValue > FilterParam->InputRangeMax)
    {
        InputValue = FilterParam->InputRangeMax;
    }
    if(InputValue < FilterParam->InputRangeMin)
    {
        InputValue = FilterParam->InputRangeMin;
    }
    
    if(FilterParam->FilterStart == 0)
    {
        DataResBuf_Count = 0;
        for(i = 0; i < DATA_1ST_FILTER_BUF_MAX; i++)
        {
            Data_1st_FilterBuf[i] = InputValue;
        }
        for(i = 0; i < DATA_2ND_FILTER_BUF_MAX; i++)
        {
            Data_2nd_FilterBuf[i] = InputValue;
        }
        FilterParam->FilterStart = 1;
        
        return OP_FAILED;
    }
    
    DataResBuf[DataResBuf_Count++] = InputValue;
    if(DataResBuf_Count < DATA_BUF_MAX)
    {
        return OP_FAILED;
    }
    else
    {
        DataResBuf_Count = 0;
    }
    
    Data_Avg1 = GetDelExtremeAndAverage(DataResBuf, DATA_BUF_MAX, 2, 5);
    
    if(FilterParam->FilterCycle < 2)
    {
        *OutputValue = Data_Avg1;
        return OP_SUCCESS;
    }
    else
    {
        for(i = 0; i < FilterParam->FilterCycle - 1; i++)
        {
            Data_1st_FilterBuf[i] = Data_1st_FilterBuf[i + 1];
        }
        Data_1st_FilterBuf[i] = Data_Avg1;
        
        if(FilterParam->FilterCycle >= 3)
        {
            for(i = 0; i < FilterParam->FilterCycle; i++)
            {
                Data_1st_FilterBufBak[i] = Data_1st_FilterBuf[i];
            }
            Data_Avg2 = GetDelExtremeAndAverage(Data_1st_FilterBufBak, 
                                            FilterParam->FilterCycle, 
                                            FilterParam->FilterCycle / 3, 
                                            FilterParam->FilterCycle / 3);
        }
        else
        {
            Data_FilterSum = 0;                                                               
            for(i = 0; i < FilterParam->FilterCycle; i++)
            {
                Data_FilterSum += Data_1st_FilterBuf[i];
            }           
            Data_Avg2 = Data_FilterSum / FilterParam->FilterCycle;        //�����˲������ƽ��ֵ
        }
        
        if(FilterParam->FilterBufMax < 2)            
        {
            *OutputValue = Data_Avg2;
            return OP_SUCCESS;
        }
        else
        {
            for(i = 0; i < FilterParam->FilterBufMax - 1; i++)
            {
                Data_2nd_FilterBuf[i] = Data_2nd_FilterBuf[i + 1];
            }
            Data_2nd_FilterBuf[i] = Data_Avg2;
            
            if(FilterParam->FilterBufMax >= 3)
            {
                for(i = 0; i < FilterParam->FilterBufMax; i++)
                {
                    Data_2nd_FilterBufBak[i] = Data_2nd_FilterBuf[i];
                }
                Data_Avg3 = GetDelExtremeAndAverage(Data_2nd_FilterBufBak, 
                                                FilterParam->FilterBufMax, 
                                                FilterParam->FilterBufMax / 3, 
                                                FilterParam->FilterBufMax / 3);
            }
            else
            {
                Data_FilterSum = 0;                                                               
                for(i = 0; i < FilterParam->FilterBufMax; i++)
                {
                    Data_FilterSum += Data_2nd_FilterBuf[i];
                }           
                Data_Avg3 = Data_FilterSum / FilterParam->FilterBufMax;        //�����˲������ƽ��ֵ
            }
            
            *OutputValue = Data_Avg3;
            return OP_SUCCESS;
        }       
    }    
}

/**@brief       ���ݰ�ָ�����Ʋ�����������ת��
* @param[in]    DataConvert_Param : ָ�����Ʋ����ṹ��;
* @param[in]    InputValue : ���������;
* @param[out]   DataConvert_Out : ������ݵĲ����ṹ��ָ��
* @return       ����ִ�н��
* - None
* @note         ʹ�ñ�����ǰ,�Ƚ�DataConvert_Param��ʼ��(��������ÿһ����Ա�����ʵ�ֵ),����������ʹ��
*/
void Sensor_PCap_DataConvert(PCap_DataConvert_Param *DataConvert_Param, 
                                uint32_t InputValue, 
                                PCap_DataConvert_Out_Param *DataConvert_Out)
{
    uint16_t LiquidHeightAD;
    uint32_t PCap_Result;
    float LiquidHeightRate;
    float Rate;
    //����ʹ��,����K,Bֵ
    if(DataConvert_Param->CompenEn == COMPENSATE_ENABLE)        
    {
        PCap_Result = (uint32_t)(InputValue * DataConvert_Param->Correct_K 
                                + (DataConvert_Param->Correct_B - 100));
    }
    else
    {
        PCap_Result = InputValue;
    }
    //������ݱ���
    LiquidHeightRate = (float)(PCap_Result - DataConvert_Param->CapMin) 
                                / (float)(DataConvert_Param->CapMax - DataConvert_Param->CapMin);
    
    if(1.0 < LiquidHeightRate)
    {
        LiquidHeightRate = 1.0;
    }
    else if (0.0 > LiquidHeightRate)
    {
        LiquidHeightRate = 0.0;
    }
    //ת����0--65535��Χ�ڵ�ADֵ
    LiquidHeightAD = (uint16_t)(LiquidHeightRate * PCAP_ADC_MAX_VALUE);
    //ת����Һλ�߶�
    DataConvert_Out->LiquidHeight = (float)(LiquidHeightRate 
                                            * DataConvert_Param->HeightRange);
    DataConvert_Out->PCapDA_ResultValue = (uint16_t)(LiquidHeightRate * (PCAP_DAC_MAX_VALUE 
                                                    - PCAP_DAC_MIN_VALUE)) 
                                                    + PCAP_DAC_MIN_VALUE;
    //���û��4���ֵ����ֱ�Ӽ���DA���ֵ
    if((0x0000 == DataConvert_Param->CapDALow) || (0x0000 == DataConvert_Param->CapDAHigh))
    {
        DataConvert_Out->PCapDA_OutValue = (uint16_t)(LiquidHeightRate * (DataConvert_Param->CapDAMax 
                                                    - DataConvert_Param->CapDAMin)) 
                                                    + DataConvert_Param->CapDAMin;
    }
    else
    {                 
        //ADֵ�����AD����
        if(LiquidHeightAD < DataConvert_Param->CapADMin)  
        {
            DataConvert_Out->PCapDA_OutValue = DataConvert_Param->CapDAMin;
        }
        //ADֵ�����AD���¿̶�AD֮��
        else if((LiquidHeightAD < DataConvert_Param->CapADLow) 
            && (LiquidHeightAD) >= DataConvert_Param->CapADMin)
        {
            Rate = (float)(LiquidHeightAD - DataConvert_Param->CapADMin)
                / (float)(DataConvert_Param->CapADLow - DataConvert_Param->CapADMin);
            DataConvert_Out->PCapDA_OutValue = DataConvert_Param->CapDALow - DataConvert_Param->CapDAMin;
            DataConvert_Out->PCapDA_OutValue = (uint16_t)(Rate * DataConvert_Out->PCapDA_OutValue) 
                                            + DataConvert_Param->CapDAMin;
        }
        //ADֵ���¿̶�ADֵ���Ͽ̶�ADֵ֮��
        else if((LiquidHeightAD < DataConvert_Param->CapADHigh) 
            && (LiquidHeightAD >= DataConvert_Param->CapADLow))
        {
            Rate = (float)(LiquidHeightAD - DataConvert_Param->CapADLow) 
                / (float)(DataConvert_Param->CapADHigh - DataConvert_Param->CapADLow);
            DataConvert_Out->PCapDA_OutValue = DataConvert_Param->CapDAHigh - DataConvert_Param->CapDALow;
            DataConvert_Out->PCapDA_OutValue = (uint16_t)(Rate * DataConvert_Out->PCapDA_OutValue) 
                                            + DataConvert_Param->CapDALow;
        }
        //ADֵ���Ͽ̶�ADֵ��������ADֵ֮��
        else if((LiquidHeightAD < DataConvert_Param->CapADMax) 
            && (LiquidHeightAD >= DataConvert_Param->CapADHigh))
        {
            Rate = (float)(LiquidHeightAD - DataConvert_Param->CapADHigh) 
                / (float)(DataConvert_Param->CapADMax - DataConvert_Param->CapADHigh);
            DataConvert_Out->PCapDA_OutValue = DataConvert_Param->CapDAMax - DataConvert_Param->CapDAHigh;
            DataConvert_Out->PCapDA_OutValue = (uint16_t)(Rate * DataConvert_Out->PCapDA_OutValue) 
                                                    + DataConvert_Param->CapDAHigh;
        }                                            
        //ADֵ������������
        else                                                       
        {
            DataConvert_Out->PCapDA_OutValue = DataConvert_Param->CapDAMax;
        }
    } 
    DataConvert_Out->LiquidHeightAD = LiquidHeightAD;
}

/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
#include <board.h>

static struct rt_pcap_device_obj pcap_device_obj;       //Pcap�豸����

/**@brief       Pcap�豸��ʼ��,��������rt_device_registerע���,��
* rt_device_init��rt_device_open����
* @param[in]    dev : �豸���
* @return       ����ִ�н��
* - RT_EOK : �豸��ʼ���ɹ�; 
*/
static rt_err_t pcap_init(rt_device_t dev)
{    
    PCap_Init();
    
    return RT_EOK;
}

/**@brief       Pcap�豸��,��������rt_device_registerע���,��
* rt_device_open����
* @param[in]    dev : �豸���
* @param[in]    oflag : �豸����ģʽ��־
* @return       ����ִ�н��
* - RT_EOK : �豸��ʼ���ɹ�; 
* @note         ���ﲻʹ���豸��,��ֻ����һ��RT_EOK
*/
static rt_err_t  pcap_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

/**@brief       Pcap�豸�ر�,��������rt_device_registerע���,��
* rt_device_close����
* @param[in]    dev : �豸���
* @return       ����ִ�н��
* - RT_EOK:�豸�رճɹ�; 
* @note         ���ﲻʹ���豸�ر�,��ֻ����һ��RT_EOK
*/
static rt_err_t  pcap_close(rt_device_t dev)
{
    return RT_EOK;
}

/**@brief       Pcap�豸������,��������rt_device_registerע���,��
* rt_device_read����
* @param[in]    dev : �豸���
* @param[in]    pos : ��ȡ����ƫ����;
* @param[out]   buffer : �ڴ滺����ָ�룬��ȡ�����ݽ��ᱻ�����ڻ�������;
* @param[in]    size : ��ȡ���ݵĴ�С
* @return       ����ִ�н��
* - size : �������ݵ�ʵ�ʴ�С��������ַ��豸�����ش�С���ֽ�Ϊ
* ��λ������ǿ��豸�����صĴ�С�Կ�Ϊ��λ;
* - 0 : ��Ҫ��ȡ��ǰ�̵߳� errno ���жϴ���״̬
*/
static rt_size_t pcap_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    if(Sensor_PCap_GetResult(pos, buffer, size) != OP_SUCCESS)
    {
        return 0;
    }    
    
    return size;
}

/**@brief       Pcap�豸����,��������rt_device_registerע���,��
* rt_device_control����
* @param[in]    dev : �豸���
* @param[in]    cmd : ��������֣��������ͨ�����豸�����������;
* @param[in]    arg : ���ƵĲ���
* @return       ����ִ�н��
* - RT_EOK:����ִ�гɹ�;
* @note         ���ﲻʹ���豸����,��ֻ����һ��RT_EOK
*/
static rt_err_t  pcap_control(rt_device_t dev, int cmd, void *args)
{
    return RT_EOK;
}

/**@brief       cap�豸������ʼ��,ע��Pcap�豸
* @return       ����ִ�н��
* - int����ֵ(RT_EOK)
* @note         ������ʹ��RT-Thread���Զ���ʼ�����INIT_DEVICE_EXPORT
* ����ִ��,ϵͳ��λ���Զ���ʼ��
*/
static int pcap_device_init(void)
{
    pcap_device_obj.dev.type     = RT_Device_Class_Miscellaneous;
    pcap_device_obj.dev.init     = pcap_init;
    pcap_device_obj.dev.open     = pcap_open;
    pcap_device_obj.dev.close    = pcap_close;
    pcap_device_obj.dev.read     = pcap_read;
    pcap_device_obj.dev.write    = RT_NULL;
    pcap_device_obj.dev.control  = pcap_control;    
    
    pcap_device_obj.dev.user_data = &pcap_device_obj;
    
    DataFilterParam_Init(&pcap_device_obj.DataFilter, DATA_BUF_MAX);   //�˲�������ʼ��
    DataConvertParam_Init(&pcap_device_obj.PCap_DataConvert); 
    
    rt_device_register(&pcap_device_obj.dev, PCAP_DEVICE_NAME, 
                        RT_DEVICE_FLAG_RDWR
                        | RT_DEVICE_FLAG_STANDALONE);
    
    return RT_EOK;
}
INIT_DEVICE_EXPORT(pcap_device_init);

#endif

