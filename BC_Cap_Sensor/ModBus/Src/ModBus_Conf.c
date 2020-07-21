/**@file        Modbus_Conf.c
* @brief        Modbus �����봦������
* @details      ����λ�����͵�ָ����н�������Ӧ,���й�������Զ��ϴ��ĳ�����붼��д�ڱ��ļ�
* @author       ׯ��Ⱥ
* @date         2020-07-20
* @version      V2.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2020/07/20  <td>2.0.0    <td>ׯ��Ⱥ  <td>���      <td>����ӵĳ������(ָ��ִ�в���)
* </table>
*
**********************************************************************************
*/

#include "ModBus_Conf.h"
#if defined(USING_MODBUS_RTU)
#include "ModBus_RTU.h"
#elif defined(USING_MODBUS_ASCII)
#include "ModBus_ASCII.h"
#endif


#ifdef __PICOCAP_APP_H

/* ʹ��soway��λ����������(Boot����), BOOT_PROGRAM��main.h�ж��� */
#ifndef BOOT_PROGRAM
static uint32_t Calib_CapMin;                   ///< �궨�������ֵ
static uint32_t Calib_CapMax;                   ///< �궨��������ֵ
static uint16_t Calib_CapDAMin;                 ///< �궨����DA���ֵ
static uint16_t Calib_CapDALow;                 ///< �궨����DA�¿̶�
static uint16_t Calib_CapDAHigh;                ///< �궨����DA�Ͽ̶�
static uint16_t Calib_CapDAMax;                 ///< �궨����DA����ֵ
static uint16_t Calib_TempDAMin;                ///< �궨�¶�DA���ֵ
static uint16_t Calib_TempDAMax;                ///< �궨�¶�DA������
static uint16_t Calib_ADMin;                    ///< �궨����AD���ֵ
static uint16_t Calib_ADLow;                    ///< �궨����AD�¿̶�
static uint16_t Calib_ADHigh;                   ///< �궨����AD�Ͽ̶�
static uint16_t Calib_ADMax;                    ///< �궨����AD������
static uint8_t CapDA_ClibFlag = CALIB_CLEAR;    

//Modbus �������³�ʼ���ص�����
static int MB_USART_ReInit(ModBusBaseParam_TypeDef *ModBusBaseParam);

//Modbus �豸����ص�����
static int MB_Freeze(ModBusBaseParam_TypeDef *ModBusBaseParam);

//ϵͳ��λ�ص�����
static int MB_System_Reset(ModBusBaseParam_TypeDef *ModBusBaseParam);

//�޵�ַ��鷢������
static int MB_SendData_NoCheck(ModBusBaseParam_TypeDef *ModBusBaseParam);


/**@brief       Modbus ��Ԥ����,�ӽ��յ�ԭʼ��������ȡ����ϢID�ͼĴ�������,���豸��ַ�͹�������뷢�ͻ�����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[out]   ReadAddr : ��ȡ��ַ(��ϢID)
* @param[out]   RegNum : �Ĵ�������
* @return       ����ִ�н��
* - None
* @note         �Ĵ�����ַ����Խ��
*/
static void ModBus_ReadPreHandle(ModBusBaseParam_TypeDef *ModBusBaseParam, uint16_t *ReadAddr, uint16_t *RegNum)
{
    if(ReadAddr != NULL)
    {
        //�Ĵ�����ַ���ֽ�
        *ReadAddr = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[2];
        *ReadAddr <<= 8;
        //�Ĵ�����ַ���ֽ�
        *ReadAddr |= ModBusBaseParam->ModBus_TX_RX.Receive_Buf[3];
    }

    if(RegNum != NULL)
    {
        //�Ĵ������ݸ��ֽ�
        *RegNum = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[4];
        *RegNum <<= 8;
        //�Ĵ����������ֽ�
        *RegNum |= ModBusBaseParam->ModBus_TX_RX.Receive_Buf[5];
    }
    
    //�����豸��ַ��������
    memcpy( ModBusBaseParam->ModBus_TX_RX.Send_Buf, 
            ModBusBaseParam->ModBus_TX_RX.Receive_Buf,
            2);
    ModBusBaseParam->ModBus_TX_RX.Send_Len = 2;
}

/**@brief       Modbus дԤ����,�ӽ��յ�ԭʼ��������ȡ����ϢID,���豸��ַ��
* �����롢��ϢID����Ϣ���ȴ��뷢�ͻ�����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[out]   WriteAddr : д���ַ(��ϢID)
* @param[out]   RegNum : �Ĵ�������
* @return       ����ִ�н��
* - None
* @note         �Ĵ�����ַ����Խ��
*/
static void ModBus_WritePreHandle(ModBusBaseParam_TypeDef *ModBusBaseParam, uint16_t *WriteAddr, uint16_t *RegNum)
{
    if(WriteAddr != NULL)
    {
        //�Ĵ�����ַ���ֽ�
        *WriteAddr = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[2];
        *WriteAddr <<= 8;
        //�Ĵ�����ַ���ֽ�
        *WriteAddr |= ModBusBaseParam->ModBus_TX_RX.Receive_Buf[3];
    }

    if(RegNum != NULL)
    {
        //�Ĵ������ݸ��ֽ�
        *RegNum = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[4];
        *RegNum <<= 8;
        //�Ĵ����������ֽ�
        *RegNum |= ModBusBaseParam->ModBus_TX_RX.Receive_Buf[5];
    }

    //����Ӧ���ݴ��뷢�ͻ���
    memcpy( ModBusBaseParam->ModBus_TX_RX.Send_Buf, 
            ModBusBaseParam->ModBus_TX_RX.Receive_Buf,
            6);

    ModBusBaseParam->ModBus_TX_RX.Send_Len = 6;
}

/**@brief       Modbus 03��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc03(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    uint16_t Nr;
    //�Ĵ�������
    uint16_t DataBuf;
    //�Ĵ�����ַ
    uint16_t ReadAddr;
    //�Ĵ�������
    uint16_t RegNum;
    //�豸����
    ModBus_Device_Param *Device_Param;
        
    Device_Param = (ModBus_Device_Param *)arg;
    //��Ԥ����
    ModBus_ReadPreHandle(ModBusBaseParam, &ReadAddr, &RegNum);
    //���ʵ�ַ������Ч��Χ��
    if( 
#if defined(SUBCODE_IS_DEVADDR)
        (ReadAddr >> 8) != ModBusBaseParam->Device_Addr ||
#endif
        (ReadAddr & 0xFF) < HOLDING_REG_REGION1_BGEIN 
        || ((ReadAddr & 0xFF) + RegNum) > (HOLDING_REG_REGION1_END + 1)
        || RegNum == 0)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        //���ʵ�ַ��Ч
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_ADDR_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }

#if defined(SUBCODE_IS_DEVADDR)
    ReadAddr &= 0x00FF;
#endif
    ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++]
        = RegNum * 2;
    for(Nr = 0; Nr < RegNum; Nr++, ReadAddr++)
    {
        switch(ReadAddr)
        {
        case 0x0030:
            //�豸��ַ
            DataBuf = ModBusBaseParam->Device_Addr;
            if(BROADCASTADDR == ModBusBaseParam->ModBus_TX_RX.Receive_Buf[0])
            {
                ModBusBaseParam->ModBus_CallBack = MB_SendData_NoCheck;
            }
        break;
                
        case 0x0031:
            //������
            DataBuf = ModBusBaseParam->BaudRate;
        break;
                
        case 0x0032:
            //��żУ��
            DataBuf = ModBusBaseParam->Parity;
        break;

        case 0x0033:
            //��
            DataBuf = 0;
        break;

        case 0x0034:
            //����ʹ��
            DataBuf = Device_Param->PCap_DataConvert->CompenEn;
        break;

        case 0x0035:
            //�˲�ϵ��
            DataBuf = Device_Param->DataFilter->FilterFactor;
        break;

        case 0x0036:
            //�Զ��ϴ�����
            DataBuf = ModBusBaseParam->AutoUpload;
        break;

        case 0x0037:
            //��������ϵ��K
            DataBuf = (uint16_t)(Device_Param->PCap_DataConvert->Correct_K * 100);
        break;

        case 0x0038:
            //��������ϵ��B
            DataBuf = (uint16_t)Device_Param->PCap_DataConvert->Correct_B + 100;
        break;

        case 0x0039:
            //�����ʽ ����
            DataBuf = ModBusBaseParam->Output_Mode;
        break;

        case 0x003F:
            //���� ����
            DataBuf = Device_Param->PCap_DataConvert->HeightRange;
        break;

        case 0x0040:
            //��
            DataBuf = 0;
        break;

        case 0x0041:
            //��
            DataBuf = 0;
        break;

        default:
            DataBuf = 0;
        break;
        }
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] 
            = (uint8_t)(DataBuf >> 8);
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] 
            = (uint8_t)DataBuf;       
    }
}

/**@brief       Modbus 04��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc04(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    //��������
    float fbuf;
    uint16_t Nr;
    //�Ĵ�����ַ
    uint16_t ReadAddr;
    //�Ĵ�������
    uint16_t RegNum;
    //�Ĵ�������
    uint32_t DataBuf;
    //�豸����
    ModBus_Device_Param *Device_Param;
        
    Device_Param = (ModBus_Device_Param *)arg;
    //��Ԥ����
    ModBus_ReadPreHandle(ModBusBaseParam, &ReadAddr, &RegNum);
    //�Ĵ�����ַ��Ч
    if( 
#if defined(SUBCODE_IS_DEVADDR)
        (ReadAddr >> 8) != ModBusBaseParam->Device_Addr ||
#endif
        ((ReadAddr & 0xFF) + RegNum) > (INPUT_REG_REGION2_END + 2)
        || ((ReadAddr & 0xFF) & 0x01) != 0 || (RegNum & 0x01) != 0
        || RegNum == 0)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_ADDR_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }
    
#if defined(SUBCODE_IS_DEVADDR)
    ReadAddr &= 0x00FF;
#endif
    ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++]
        = RegNum * 2;
    for(Nr = 0; Nr < RegNum; Nr += 2, ReadAddr += 2)
    {
        switch(ReadAddr)
        {
        case 0x0000:
            //Һλ�߶�ADֵ
            fbuf = (float)Device_Param->PCap_DataConvert_Out->LiquidHeightAD;
            DataBuf = *(uint32_t *)&fbuf;
        break;
            
        case 0x0002:
            //�����¶�
            DataBuf = *(uint32_t *)&Device_Param->ADC_TemperOut->TemperInAir;
        break;
            
        case 0x0004:
            //Һ���¶� ����
            DataBuf = *(uint32_t *)&Device_Param->ADC_TemperOut->TemperInLiquid;
        break;

        case 0x0080:
            //PCapԭʼֵ
            fbuf = (float)Device_Param->PCap_DataConvert_Out->PCap_ResultValue;
            DataBuf = *(uint32_t *)&fbuf;
        break;

        default:
            DataBuf = 0;
        break;
        }

        ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] 
            = (DataBuf >> 24);
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] 
            = (DataBuf >> 16);
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] 
            = (DataBuf >> 8);
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] 
            = (uint8_t)DataBuf;        
    }
}

/**@brief       Modbus 05��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc05(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    //�Ĵ�����ַ
    uint16_t WriteAddr;
    //�Ĵ�������
    uint16_t DataBuf;
    static uint8_t CalibFlag = CALIB_CLEAR;
    //�豸����
    ModBus_Device_Param *Device_Param;
        
    Device_Param = (ModBus_Device_Param *)arg;
    //дԤ����
    ModBus_WritePreHandle(ModBusBaseParam, &WriteAddr, NULL);    
    //��ַ��Ч
    if(
#if defined(SUBCODE_IS_DEVADDR)
        ((WriteAddr >> 8) != ModBusBaseParam->Device_Addr ||
#else
        (
#endif
        (WriteAddr & 0xFF) < SINGLE_COIL_REGION1_BEGIN 
        || (WriteAddr & 0xFF) > SINGLE_COIL_REGION1_END)
        && (WriteAddr & 0xFF) != 0x004A)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_ADDR_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }

#if defined(SUBCODE_IS_DEVADDR)
    WriteAddr &= 0x00FF;
#endif
    //�������ݸ��ֽ�
    DataBuf = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[4];
    DataBuf <<= 8;
    //�������ݵ��ֽ�
    DataBuf |= ModBusBaseParam->ModBus_TX_RX.Receive_Buf[5];

    //����������Ч
    if((0x0000 != DataBuf)&&(0xFF00 != DataBuf))
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_VALU_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }
     
    switch(WriteAddr)
    {
    case 0x004A:                                //Ϊ�˸���Э�����
        //�ڲ��洢��ʹ�ܻ��ֹ
        if(0xFF00 == DataBuf)
        {
            ModBusBaseParam->InRomWrEn = IN_MEMORY_WR_ENABLE;
        }
        else if(0x0000 == DataBuf)
        {
            ModBusBaseParam->InRomWrEn = IN_MEMORY_WR_DISABLE;
        }
    break;      
      
    case 0x0050:
        if(ModBusBaseParam->InRomWrEn == IN_MEMORY_WR_ENABLE)
        {
            //���ݱ궨
            if(0xFF00 == DataBuf)       //�궨������
            {
                Calib_CapMax = Device_Param->PCap_DataConvert_Out->PCap_ResultValue;
                CalibFlag |= CALIB_CAPMAX_FLAG;
            }
            else if(0x0000 == DataBuf)  //�궨���
            {
                Calib_CapMin = Device_Param->PCap_DataConvert_Out->PCap_ResultValue;
                CalibFlag |= CALIB_CAPMIN_FLAG;
            }
            //���ݱ궨��־λ��Ч��д��궨����  
            if(CALIB_CAPEOC_FLAG == CalibFlag)
            {
                MB_Cap_Calibration(arg);
                CalibFlag = CALIB_CLEAR;
            }
        }
        else
        {
            goto __rom_error;
        }
    break;

    case 0x0051:
        if(ModBusBaseParam->InRomWrEn == IN_MEMORY_WR_ENABLE)
        {
            //�ָ����ݱ궨����ֵ
            if(0xFF00 == DataBuf)
            {
                Calib_CapMin = Device_Param->PCap_DataConvert->CapMinBak;
                Calib_CapMax = Device_Param->PCap_DataConvert->CapMaxBak;
                MB_Cap_Calibration(arg);
            }
        }
        else
        {
            goto __rom_error;
        }
    break;

    case 0x0052:
        //�豸�����ⶳ
        if(0xFF00 == DataBuf)
        {
            ModBusBaseParam->ModBus_CallBack = MB_Freeze;
        }
        else if(0x0000 == DataBuf)
        {
            ModBusBaseParam->Freeze = FREEZE_DISABLE;
        }
    break;

    case 0x0053:
        //�ڲ�Flashʹ�ܻ��ֹ
        if(0xFF00 == DataBuf)
        {
            ModBusBaseParam->InRomWrEn = IN_MEMORY_WR_ENABLE;
        }
        else if(0x0000 == DataBuf)
        {
            ModBusBaseParam->InRomWrEn = IN_MEMORY_WR_DISABLE;
        }
    break;
            
    default:
    break;
    }	
    return;
__rom_error:
    ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
    ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_DEVC_EXCEPTION;
    ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;    
}

/**@brief       Modbus 10��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc10(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    uint16_t Nr;
    //��������
    uint16_t Index;
    //�Ĵ�����ַ
    uint16_t WriteAddr;
    //�Ĵ�������
    uint16_t RegNum;
    //���ݳ���
    uint16_t DataLen;
    //16λ�����ݴ�
    uint16_t u16temp;
    //�豸����
    ModBus_Device_Param *Device_Param;
        
    Device_Param = (ModBus_Device_Param *)arg;
    
    //���ݳ���
    DataLen = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[6];
    //дԤ����
    ModBus_WritePreHandle(ModBusBaseParam, &WriteAddr, &RegNum);    
    //�Ĵ�����ַ��Ч
    if( 
#if defined(SUBCODE_IS_DEVADDR)
        (WriteAddr >> 8) != ModBusBaseParam->Device_Addr ||
#endif
        (WriteAddr & 0xFF) < MUL_REG_REGION1_BEGIN 
        || ((WriteAddr & 0xFF) + RegNum) > (MUL_REG_REGION1_END + 1)
        || (DataLen != (RegNum * 2))
        || RegNum == 0)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_ADDR_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }

#if defined(SUBCODE_IS_DEVADDR)
    WriteAddr &= 0x00FF;
#endif
    //�ڲ�ROM���ʽ�ֹ
    if(IN_MEMORY_WR_ENABLE != ModBusBaseParam->InRomWrEn)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_DEVC_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }

    Index = 0;
    for(Nr = 0; Nr < RegNum; Nr++, WriteAddr++)
    {
        u16temp = (uint16_t)(ModBusBaseParam->ModBus_TX_RX.Receive_Buf[7 + Index] << 8) 
            | ModBusBaseParam->ModBus_TX_RX.Receive_Buf[8 + Index];

        switch(WriteAddr)
        {
        case 0x0030:
            //�豸��ַ
            if(0 < u16temp && 0xF8 > u16temp)
            {
                ModBusBaseParam->Device_Addr = u16temp;
                InMemory_Write3T_MultiBytes(DEVICE_ADDR, &ModBusBaseParam->Device_Addr, 1);
            }
        break;
                
        case 0x0031:
            //������
            if(0x06 > u16temp)
            {
                ModBusBaseParam->BaudRate = u16temp;
                InMemory_Write3T_MultiBytes(BAUDRATE, &ModBusBaseParam->BaudRate, 1);
                ModBusBaseParam->ModBus_CallBack = MB_USART_ReInit;
            }
        break;
                
        case 0x0032:
            //��żУ��
            if(0x03 > u16temp)
            {
                ModBusBaseParam->Parity = u16temp;
                InMemory_Write3T_MultiBytes(PARITY, &ModBusBaseParam->Parity, 1);
                ModBusBaseParam->ModBus_CallBack = MB_USART_ReInit;
            }
        break;
          
        case 0x0033:
            //��
        break;
          
        case 0x0034:
            //����ʹ��
            if(2 > u16temp)
            {
                Device_Param->PCap_DataConvert->CompenEn = u16temp;
                InMemory_Write3T_MultiBytes(COMPENSATE, &Device_Param->PCap_DataConvert->CompenEn, 1);
            }
        break;
          
        case 0x0035:
            //�˲�ϵ��
            if(0x0A > u16temp)
            {
                Device_Param->DataFilter->FilterFactor = u16temp;
                InMemory_Write3T_MultiBytes(FILTER, &Device_Param->DataFilter->FilterFactor, 1);
                SwitchCurFilter(Device_Param->DataFilter->FilterFactor, Device_Param->DataFilter);
            }
        break;
          
        case 0x0036:
            //�Զ��ϴ�����
            ModBusBaseParam->AutoUpload = u16temp;
            InMemory_Write3T_MultiBytes(AUTO_UPLOAD, &ModBusBaseParam->AutoUpload, 1);
        break;
          
        case 0x0037:
            //��������ϵ��K
            if(0 < u16temp)
            {
                Device_Param->PCap_DataConvert->Correct_K = (float)u16temp / 100.0;
                InMemory_Write3T_MultiBytes(CORRECT_K, 
                                    (const uint8_t *)&ModBusBaseParam->ModBus_TX_RX.Receive_Buf[7 + Index], 2);
            }
        break;
          
        case 0x0038:
            //��������ϵ��B
            Device_Param->PCap_DataConvert->Correct_B = (float)u16temp;
            InMemory_Write3T_MultiBytes(CORRECT_B, 
                                    (const uint8_t *)&ModBusBaseParam->ModBus_TX_RX.Receive_Buf[7 + Index], 2);
        break;
          
        case 0x0039:
            //�����ʽ
            if(2 > u16temp)
            {
                ModBusBaseParam->Output_Mode = u16temp;
                InMemory_Write3T_MultiBytes(OUTPUTMODE, &ModBusBaseParam->Output_Mode, 1);
            }
        break;
          
        case 0x003F:
            //����
            if(u16temp > 0)
            {
                Device_Param->PCap_DataConvert->HeightRange = u16temp;
                InMemory_Write3T_MultiBytes(HEIGHTRANGE, 
                                        (const uint8_t *)&ModBusBaseParam->ModBus_TX_RX.Receive_Buf[7 + Index], 2);
            }
        break;
          
        case 0x0040:
            //��
        break;
          
        case 0x0041:
            //��
        break;
          
        default:
            
        break;
        }     
            
        Index += DataLen;        
    }
}

/**@brief       Modbus 25��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc25(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    //�Ĵ�����ַ
    uint16_t WriteAddr;
    //��������
    uint16_t DataBuf;
    //�豸����
    ModBus_Device_Param *Device_Param;    
    static uint8_t CapAD_ClibFlag = CALIB_CLEAR;
    static uint8_t CapAD_ClibEn = ADCLIB_DISABLE;
    static uint8_t TempDA_ClibFlag = CALIB_CLEAR;
    static uint8_t TempDA_ClibEn = DAOUTCLIB_DISABLE;
        
    Device_Param = (ModBus_Device_Param *)arg;
    
    if(ModBusBaseParam->InRomWrEn != IN_MEMORY_WR_ENABLE)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_DEVC_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }

    //дԤ����
    ModBus_WritePreHandle(ModBusBaseParam, &WriteAddr, NULL);
    //�Ĵ�����ַ��Ч
    if(
#if defined(SUBCODE_IS_DEVADDR)
        ((WriteAddr >> 8) != ModBusBaseParam->Device_Addr ||
#else
        (
#endif
        (WriteAddr & 0xFF) > _25_FNUC_REG_REGION1_END)
        && (WriteAddr & 0x00B0) != 0x00B0)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_ADDR_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }
    
#if defined(SUBCODE_IS_DEVADDR)
    WriteAddr &= 0x00FF;
#endif
    //�Ĵ������ݸ��ֽ�
    DataBuf = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[4];
    DataBuf <<= 8;
    //�Ĵ������ݵ��ֽ�
    DataBuf |= ModBusBaseParam->ModBus_TX_RX.Receive_Buf[5];

    //�Ĵ�������Ч
    if((0x0000 != DataBuf)&&(0xFF00 != DataBuf))
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_VALU_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }

    switch(WriteAddr & ~0x00B0)
    {
    case 0x0000:
    //��
    break;

    case 0x0001:
        //����DA�궨���¿̶���
        if(DAOUTCLIB_ENABLE == Device_Param->PCap_DataConvert->CapDA_ClibEn)
        {
            if(0xFF00 == DataBuf)
            {
                CapDA_ClibFlag |= CALIB_CAPDAHIH_FLAG;
                Calib_CapDAHigh = Device_Param->PCap_DataConvert_Out->PCapDA_ResultValue;
            }
            else if(0x0000 == DataBuf)
            {
                CapDA_ClibFlag |= CALIB_CAPDALOW_FLAG;
                Calib_CapDALow = Device_Param->PCap_DataConvert_Out->PCapDA_ResultValue;
            }
        }
    break;

    case 0x0002:
        //����DA�궨ʹ��
        if(0xFF00 == DataBuf)
        {
            Device_Param->PCap_DataConvert->CapDA_ClibEn = DAOUTCLIB_ENABLE;
        }
        else if(0x0000 == DataBuf)
        {
            Device_Param->PCap_DataConvert->CapDA_ClibEn = DAOUTCLIB_DISABLE;
        }

        if(DAOUTCLIB_DISABLE == Device_Param->PCap_DataConvert->CapDA_ClibEn 
            && (CapDA_ClibFlag & (CALIB_CAPDAMAX_FLAG | CALIB_CAPDAMIN_FLAG)) 
            == (CALIB_CAPDAMAX_FLAG | CALIB_CAPDAMIN_FLAG))
        {
            MB_CapDAOut_Calibration(arg);
            CapDA_ClibFlag = CALIB_CLEAR;
        }
    break;

    case 0x0003:
        //����DA�궨���������
        if(DAOUTCLIB_ENABLE == Device_Param->PCap_DataConvert->CapDA_ClibEn)
        {
            if(0xFF00 == DataBuf)
            {
                CapDA_ClibFlag |= CALIB_CAPDAMAX_FLAG;
                Calib_CapDAMax = Device_Param->PCap_DataConvert_Out->PCapDA_ResultValue;
            }
            else if(0x0000 == DataBuf)
            {
                CapDA_ClibFlag |= CALIB_CAPDAMIN_FLAG;
                Calib_CapDAMin = Device_Param->PCap_DataConvert_Out->PCapDA_ResultValue;
            }
        }
    break;

    case 0x0004:
        //�¶ȱ궨ʹ��
        if(0xFF00 == DataBuf)
        {
            TempDA_ClibEn = DAOUTCLIB_ENABLE;
        }
        else if(0x0000 == DataBuf)
        {
            TempDA_ClibEn = DAOUTCLIB_DISABLE;
        }

        if((DAOUTCLIB_DISABLE == TempDA_ClibEn) && (CALIB_TEMPDAEOC_FLAG == TempDA_ClibFlag))
        {
            MB_TempDAOut_Calibration(arg);
            TempDA_ClibFlag = CALIB_CLEAR;
        }
    break;

    case 0x0005:
        //�¶ȱ궨
        if(DAOUTCLIB_ENABLE == TempDA_ClibEn)
        {
            if(0xFF00 == DataBuf)
            {
                TempDA_ClibFlag |= CALIB_TEMPDAMAX_FLAG;
                Calib_TempDAMax = Device_Param->ADC_TemperOut->TemperInAirAD;
            }
            else if(0x0000 == DataBuf)
            {
                TempDA_ClibFlag |= CALIB_TEMPDAMIN_FLAG;
                Calib_TempDAMin = Device_Param->ADC_TemperOut->TemperInAirAD;
            }
        }
    break;

    case 0x0007:
        //����AD�궨ʹ��
        if(0xFF00 == DataBuf)
        {
            CapAD_ClibFlag = CALIB_CLEAR;
            CapAD_ClibEn = ADCLIB_ENABLE;
        }
        else if(0x0000 == DataBuf)
        {
            CapAD_ClibEn = ADCLIB_DISABLE;
        }

        if((ADCLIB_DISABLE == CapAD_ClibEn) && (CALIB_CAPADEOC_FLAG == CapAD_ClibFlag))
        {
            MB_CapAD_Calibration(arg);
            CapAD_ClibFlag = CALIB_CLEAR;
        }
    break;

    case 0x0008:
        //����AD�궨���¿̶���
        if(ADCLIB_ENABLE == CapAD_ClibEn)
        {
            if(0xFF00 == DataBuf)
            {
                Calib_ADHigh = Device_Param->PCap_DataConvert_Out->LiquidHeightAD;
                CapAD_ClibFlag |= CALIB_CAPADHIH_FLAG;
            }
            else if(0x0000 == DataBuf)
            {
                Calib_ADLow = Device_Param->PCap_DataConvert_Out->LiquidHeightAD;
                CapAD_ClibFlag |= CALIB_CAPADLOW_FLAG;
            }
        }
    break;

    case 0x0009:
        //����AD�궨���������
        if(ADCLIB_ENABLE == CapAD_ClibEn)
        {
            if(0xFF00 == DataBuf)
            {
                Calib_ADMax = Device_Param->PCap_DataConvert_Out->LiquidHeightAD;
                CapAD_ClibFlag |= CALIB_CAPADMAX_FLAG;
            }
            else if(0x0000 == DataBuf)
            {
                Calib_ADMin = Device_Param->PCap_DataConvert_Out->LiquidHeightAD;
                CapAD_ClibFlag |= CALIB_CAPADMIN_FLAG;
            }
        }
    break;

    default:
        
    break;
    }
}

/**@brief       Modbus 26��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc26(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    uint8_t i;
    //��������
    float fBuf;
    uint16_t Nr;
    //�Ĵ�����ַ
    uint16_t ReadAddr;
    //�Ĵ�������
    uint16_t RegNum;
    //�Ĵ�����������
    uint32_t DataBuf;
    //�豸����
    ModBus_Device_Param *Device_Param;
        
    Device_Param = (ModBus_Device_Param *)arg;
    //��Ԥ����
    ModBus_ReadPreHandle(ModBusBaseParam, &ReadAddr, &RegNum);
    //�Ĵ������ʵ�ַ��Ч
    if(
#if defined(SUBCODE_IS_DEVADDR)
        (ReadAddr >> 8) != ModBusBaseParam->Device_Addr ||
#endif
        (ReadAddr & 0xFF) < DEF_MUL_REG_REGION1_BEGIN 
        || ((ReadAddr & 0xFF) + RegNum) > (DEF_MUL_REG_REGION1_END + 2)
        || (RegNum & 0x01) != 0 || (ReadAddr & 0x01) != 0
        || RegNum == 0)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_ADDR_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }

#if defined(SUBCODE_IS_DEVADDR)
    ReadAddr &= 0x00FF;
#endif
    ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++]
        = RegNum * 2;
    for(Nr = 0; Nr < RegNum; Nr += 2, ReadAddr += 2)
    {
        switch(ReadAddr)
        {
        case 0x0080:
            fBuf = (float)Device_Param->PCap_DataConvert->CapMax;                                       //����������
            DataBuf = *(uint32_t *)&fBuf;
        break;

        case 0x0082:
            fBuf = (float)Device_Param->PCap_DataConvert->CapMin;                                       //�������
            DataBuf = *(uint32_t *)&fBuf;
        break;

        case 0x0084:     
            fBuf = (float)Device_Param->PCap_DataConvert->CapDAMax;                                     //DA����
            DataBuf = *(uint32_t *)&fBuf;
        break;
          
        case 0x0086:
            fBuf = (float)Device_Param->PCap_DataConvert->CapDAHigh;                                     //DA�Ͽ̶�
            DataBuf = *(uint32_t *)&fBuf;
        break;
          
        case 0x0088:
            fBuf = (float)Device_Param->PCap_DataConvert->CapDALow;                                     //DA�¿̶�
            DataBuf = *(uint32_t *)&fBuf;
        break;  
          
        case 0x008A:
            fBuf = (float)Device_Param->PCap_DataConvert->CapDAMin;                                     //DA���
            DataBuf = *(uint32_t *)&fBuf;
        break; 
          
        case 0x008C: 
            fBuf = (float)Device_Param->PCap_DataConvert->CapADMax;                                     //AD����
            DataBuf = *(uint32_t *)&fBuf;
        break;

        case 0x008E:
            fBuf = (float)Device_Param->PCap_DataConvert->CapADHigh;                                     //AD�Ͽ̶�
            DataBuf = *(uint32_t *)&fBuf;
        break;       
            
        case 0x0090:
            fBuf = (float)Device_Param->PCap_DataConvert->CapADLow;                                     //AD�¿̶�
            DataBuf = *(uint32_t *)&fBuf;
        break;  
          
        case 0x0092:
            fBuf = (float)Device_Param->PCap_DataConvert->CapADMin;                                      //AD���
            DataBuf = *(uint32_t *)&fBuf;
        break;       
          
        case 0x0094:  
            
        break;
          
        case 0x0096:  
            
        break;

        case 0x0098:  
            
        break;

        case 0x009A:  
            
        break;      
          
        case 0x009C:                                                                
            fBuf = Device_Param->ADC_TemperParam->Temper_K1;                                               //�����¶�����ϵ��K1   
            DataBuf = *(uint32_t *)&fBuf;
        break;
          
        case 0x009E:
            fBuf = Device_Param->ADC_TemperParam->Temper_B1;                                               //�����¶�����ϵ��B1
            DataBuf = *(uint32_t *)&fBuf;
        break;
          
        case 0x00A0:
            fBuf = Device_Param->ADC_TemperParam->Temper_K2;                                               //Һ���¶�����ϵ��K2
            DataBuf = *(uint32_t *)&fBuf;
        break;
          
        case 0x00A2:
            fBuf = Device_Param->ADC_TemperParam->Temper_B2;                                               //Һ���¶�����ϵ��B2
            DataBuf = *(uint32_t *)&fBuf;
        break;
          
        default:
            
        break;
        }

        for(i = 4; i > 0; i--)
        {
            ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] 
                = (uint8_t)(DataBuf >> ((i - 1)*8));
        }
    }
}

/**@brief       Modbus 27��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc27(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    //��������
    float fbuf;
    uint16_t Nr;
    //����
    uint16_t index;
    //�Ĵ�����ַ
    uint16_t WriteAddr;
    //�Ĵ�������
    uint16_t RegNum;
    //���ݳ���
    uint16_t DataLen;
    //�豸����
    ModBus_Device_Param *Device_Param;
    uint8_t Cur_Param[2];
          
    Device_Param = (ModBus_Device_Param *)arg;    
    //дԤ����
    ModBus_WritePreHandle(ModBusBaseParam, &WriteAddr, &RegNum);  
    //�ڲ�ROMδʹ�ܷ���ʧ��
    if(IN_MEMORY_WR_ENABLE != ModBusBaseParam->InRomWrEn)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_DEVC_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }

    //���ݳ���
    DataLen = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[6];
    //�Ĵ������ʵ�ַ��Ч
    if( 
#if defined(SUBCODE_IS_DEVADDR)
        (WriteAddr >> 8) != ModBusBaseParam->Device_Addr ||
#endif
        (WriteAddr & 0xFF) < DEF_MUL_REG_REGION1_BEGIN 
        || ((WriteAddr & 0xFF) + RegNum) > (DEF_MUL_REG_REGION1_END + 2)
        || (RegNum & 0x01) != 0 || (WriteAddr & 0x01) != 0
        || DataLen != (RegNum * 2)
        || RegNum == 0)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_ADDR_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }
    
#if defined(SUBCODE_IS_DEVADDR)
    WriteAddr &= 0x00FF;
#endif
    index = 0;
    for(Nr = 0; Nr < RegNum; Nr += 2, WriteAddr += 2)
    {
        fbuf = HexToFloat(&ModBusBaseParam->ModBus_TX_RX.Receive_Buf[7 + index]);
        switch(WriteAddr)
        {
        case 0x0080:            
            Device_Param->PCap_DataConvert->CapMax = (uint32_t)fbuf;
            Device_Param->DataFilter->InputRangeMax = (uint32_t)fbuf;            
            InMemory_Write3T_MultiBytes(CAPMAX, 
                                    &ModBusBaseParam->ModBus_TX_RX.Receive_Buf[7 + index], 4);
        break;
          
        case 0x0082:
            Device_Param->PCap_DataConvert->CapMin = (uint32_t)fbuf;
            Device_Param->DataFilter->InputRangeMin = (uint32_t)fbuf;
            InMemory_Write3T_MultiBytes(CAPMIN, 
                                    &ModBusBaseParam->ModBus_TX_RX.Receive_Buf[7 + index], 4);
        break;

        case 0x0084:
            if(fbuf <= PCAP_DAC_MAX_VALUE && fbuf >= PCAP_DAC_MIN_VALUE)
            {
                Device_Param->PCap_DataConvert->CapDAMax = (uint16_t)fbuf;              
                Cur_Param[0] = Device_Param->PCap_DataConvert->CapDAMax >> 8;
                Cur_Param[1] = Device_Param->PCap_DataConvert->CapDAMax & 0xFF;
                InMemory_Write3T_MultiBytes(CAPDAMAX, Cur_Param, 2);
            }            
        break;
               
        case 0x0086:
            if(fbuf <= PCAP_DAC_MAX_VALUE && fbuf >= PCAP_DAC_MIN_VALUE)
            {
                Device_Param->PCap_DataConvert->CapDAHigh = (uint16_t)fbuf;       
                Cur_Param[0] = Device_Param->PCap_DataConvert->CapDAHigh >> 8;
                Cur_Param[1] = Device_Param->PCap_DataConvert->CapDAHigh & 0xFF;                
                InMemory_Write3T_MultiBytes(CAPDAHIGH, Cur_Param, 2); 
            }
        break;
          
        case 0x0088:
            if(fbuf <= PCAP_DAC_MAX_VALUE && fbuf >= PCAP_DAC_MIN_VALUE)
            {
                Device_Param->PCap_DataConvert->CapDALow = (uint16_t)fbuf;          
                Cur_Param[0] = Device_Param->PCap_DataConvert->CapDALow >> 8;
                Cur_Param[1] = Device_Param->PCap_DataConvert->CapDALow & 0xFF;
                InMemory_Write3T_MultiBytes(CAPDALOW, Cur_Param, 2); 
            }      
        break;
              
        case 0x008A:
            if(fbuf <= PCAP_DAC_MAX_VALUE && fbuf >= PCAP_DAC_MIN_VALUE)
            {
                Device_Param->PCap_DataConvert->CapDAMin = (uint16_t)fbuf;     
                Cur_Param[0] = Device_Param->PCap_DataConvert->CapDAMin >> 8;
                Cur_Param[1] = Device_Param->PCap_DataConvert->CapDAMin & 0xFF;
                InMemory_Write3T_MultiBytes(CAPDAMIN, Cur_Param, 2);
            }     
        break; 
          
        case 0x008C:
            if(fbuf <= PCAP_ADC_MAX_VALUE && fbuf >= PCAP_ADC_MIN_VALUE)
            {
                Device_Param->PCap_DataConvert->CapADMax = (uint16_t)fbuf;    
                Cur_Param[0] = Device_Param->PCap_DataConvert->CapADMax >> 8;
                Cur_Param[1] = Device_Param->PCap_DataConvert->CapADMax & 0xFF;
                InMemory_Write3T_MultiBytes(CAPADMAX, Cur_Param, 2);      
            }            
        break; 
               
        case 0x008E:
            if(fbuf <= PCAP_ADC_MAX_VALUE && fbuf >= PCAP_ADC_MIN_VALUE)
            {
                Device_Param->PCap_DataConvert->CapADHigh = (uint16_t)fbuf;       
                Cur_Param[0] = Device_Param->PCap_DataConvert->CapADHigh >> 8;
                Cur_Param[1] = Device_Param->PCap_DataConvert->CapADHigh & 0xFF;
                InMemory_Write3T_MultiBytes(CAPADHIGH, Cur_Param, 2);       
            }                
        break; 
          
        case 0x0090:
            if(fbuf <= PCAP_ADC_MAX_VALUE && fbuf >= PCAP_ADC_MIN_VALUE)
            {
                Device_Param->PCap_DataConvert->CapADLow = (uint16_t)fbuf;        
                Cur_Param[0] = Device_Param->PCap_DataConvert->CapADLow >> 8;
                Cur_Param[1] = Device_Param->PCap_DataConvert->CapADLow & 0xFF;
                InMemory_Write3T_MultiBytes(CAPADLOW, Cur_Param, 2);           
            }
        break; 
               
        case 0x0092:
            if(fbuf <= PCAP_ADC_MAX_VALUE && fbuf >= PCAP_ADC_MIN_VALUE)
            {
                Device_Param->PCap_DataConvert->CapADMin = (uint16_t)fbuf;        
                Cur_Param[0] = Device_Param->PCap_DataConvert->CapADMin >> 8;
                Cur_Param[1] = Device_Param->PCap_DataConvert->CapADMin & 0xFF;
                InMemory_Write3T_MultiBytes(CAPADMIN, Cur_Param, 2);   
            }            
        break;       

        default:
            
        break;
        }
        index += DataLen;
    }
}

/**@brief       Modbus 2A��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @return       ����ִ�н��
* - None
*/
void ModbusFunc2A(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
    uint8_t i;
    uint8_t j;
    uint8_t objlen;
    uint16_t RegNum;
    uint16_t WriteAddr;

    //дԤ����
    ModBus_WritePreHandle(ModBusBaseParam, &WriteAddr, &RegNum);    
    //�ڲ�ROMδʹ�ܷ���ʧ��
    if(IN_MEMORY_WR_ENABLE != ModBusBaseParam->InRomWrEn)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_DEVC_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }
    if(
#if defined(SUBCODE_IS_DEVADDR)
        (WriteAddr >> 8) != ModBusBaseParam->Device_Addr ||
#endif
        (WriteAddr & 0xFF) < MUL_VERSION_INF_BEGIN 
        || ((WriteAddr & 0xFF) + RegNum) > (MUL_VERSION_INF_END + 1)
        || RegNum == 0)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_ADDR_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }
    
#if defined(SUBCODE_IS_DEVADDR)
    WriteAddr &= 0x00FF;
#endif

    j = 6;
    for(i = 0; i < RegNum; i++, WriteAddr++)
    {
        objlen = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[j] + 1;
        switch(WriteAddr)
        {
        //��������
        case 0x00E0:            
            if(0x2F < objlen)
            {
                j += objlen;
                break;
            }
            InMemory_Write_MultiBytes(ORGANIZATION, 
                (const uint8_t *)&ModBusBaseParam->ModBus_TX_RX.Receive_Buf[j], objlen);                                    
            j += objlen;
        break;
        //��Ʒ����    
        case 0x00E1:
            if(0x2F < objlen)
            {
                j += objlen;
                break;
            }
            InMemory_Write_MultiBytes(PRODUCTION, 
                (const uint8_t *)&ModBusBaseParam->ModBus_TX_RX.Receive_Buf[j], objlen);                               
            j += objlen;
        break;
        //Ӳ���汾    
        case 0x00E2:
            if(0x2F < objlen)
            {
                j += objlen;
                break;
            }
            InMemory_Write_MultiBytes(HARDWAREVER, 
                (const uint8_t *)&ModBusBaseParam->ModBus_TX_RX.Receive_Buf[j], objlen);                                  
            j += objlen;
        break;
        //����汾    
        case 0x00E3:
            if(0x2F < objlen)
            {
                j += objlen;
                break;
            }
            InMemory_Write_MultiBytes(SOFTWAREVER, 
                (const uint8_t *)&ModBusBaseParam->ModBus_TX_RX.Receive_Buf[j], objlen);                                  
            j += objlen;
        break;
        //�豸ID    
        case 0x00E4:
            if(0x2F < objlen)
            {
                j += objlen;
                break;
            }
            InMemory_Write_MultiBytes(DEVICENUM, 
                (const uint8_t *)&ModBusBaseParam->ModBus_TX_RX.Receive_Buf[j], objlen);                                     
            j += objlen;
        break;
        //�ͻ�����    
        case 0x00E5:
            if(0x2F < objlen)
            {
                j += objlen;
                break;
            }
            InMemory_Write_MultiBytes(CUSTOMER, 
                (const uint8_t *)&ModBusBaseParam->ModBus_TX_RX.Receive_Buf[j], objlen);                               
            j += objlen;
        break;
            
        default:
            
        break;
        }        
    }
}

/**@brief       Modbus 2B��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @return       ����ִ�н��
* - None
*/
void ModbusFunc2B(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
    uint8_t i;
    uint8_t objlen;
    uint16_t RegNum;
    uint16_t ReadAddr;

    //��Ԥ����
    ModBus_ReadPreHandle(ModBusBaseParam, &ReadAddr, &RegNum);    
    
    if(
#if defined(SUBCODE_IS_DEVADDR)
        (ReadAddr >> 8) != ModBusBaseParam->Device_Addr ||
#endif
        (ReadAddr & 0xFF) < MUL_VERSION_INF_BEGIN
        || ((ReadAddr & 0xFF) + RegNum) > (MUL_VERSION_INF_END + 1)
        || RegNum == 0)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_ADDR_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }
    
#if defined(SUBCODE_IS_DEVADDR)
    ReadAddr &= 0x00FF;
#endif
    memcpy(&ModBusBaseParam->ModBus_TX_RX.Send_Buf[2], &ModBusBaseParam->ModBus_TX_RX.Receive_Buf[4], 2);
    memcpy(&ModBusBaseParam->ModBus_TX_RX.Send_Buf[4], &ModBusBaseParam->ModBus_TX_RX.Receive_Buf[2], 2);
    ModBusBaseParam->ModBus_TX_RX.Send_Len = 6;
    for(i = 0; i < RegNum; i++, ReadAddr++)
    {
        switch(ReadAddr)
        {
        //��������  
        case 0x00E0:
            objlen = InMemory_Read_OneByte(ORGANIZATION);                                                       
            ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = objlen;
            if((objlen > (SEND_SIZE * 2 / 3))||(0 == objlen))
            {
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len - 1] = 1;
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = 0;
                break;
            }
            InMemory_Read_MultiBytes((ORGANIZATION + 1), 
                &ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len], objlen);
            ModBusBaseParam->ModBus_TX_RX.Send_Len += objlen;
        break;
        //��Ʒ����  
        case 0x00E1:
            objlen = InMemory_Read_OneByte(PRODUCTION);                                                  
            ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = objlen;
            if((objlen > (SEND_SIZE * 2 / 3))||(0 == objlen))
            {
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len - 1] = 1;
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = 0;
                break;
            }
            InMemory_Read_MultiBytes((PRODUCTION + 1), 
                &ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len], objlen);
            ModBusBaseParam->ModBus_TX_RX.Send_Len += objlen;
        break;
        //Ӳ���汾  
        case 0x00E2:
            objlen = InMemory_Read_OneByte(HARDWAREVER);                                                     
            ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = objlen;
            if((objlen > (SEND_SIZE * 2 / 3))||(0 == objlen))
            {
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len - 1] = 1;
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = 0;
                break;
            }
            InMemory_Read_MultiBytes((HARDWAREVER + 1), 
                &ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len], objlen);
            ModBusBaseParam->ModBus_TX_RX.Send_Len += objlen;
        break;
        //����汾 
        case 0x00E3:                        
            objlen = InMemory_Read_OneByte(SOFTWAREVER);
            ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = objlen;
            if((objlen > (SEND_SIZE * 2 / 3))||(0 == objlen))
            {
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len - 1] = 1;
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = 0;
                break;
            }
            InMemory_Read_MultiBytes((SOFTWAREVER + 1), 
                &ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len], objlen);
            ModBusBaseParam->ModBus_TX_RX.Send_Len += objlen;
        break;
        //�豸ID  
        case 0x00E4:
            objlen = InMemory_Read_OneByte(DEVICENUM);                                                        
            ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = objlen;
            if((objlen > (SEND_SIZE * 2 / 3))||(0 == objlen))
            {
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len - 1] = 1;
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = 0;
                break;
            }
            InMemory_Read_MultiBytes((DEVICENUM + 1), 
                &ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len], objlen);
            ModBusBaseParam->ModBus_TX_RX.Send_Len += objlen;
        break;
        //�ͻ�����  
        case 0x00E5:
            objlen = InMemory_Read_OneByte(CUSTOMER);                                                  
            ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = objlen;
            if((objlen > (SEND_SIZE * 2 / 3))||(0 == objlen))
            {
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len - 1] = 1;
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = 0;
                break;
            }
            InMemory_Read_MultiBytes((CUSTOMER + 1), 
                &ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len], objlen);
            ModBusBaseParam->ModBus_TX_RX.Send_Len += objlen;
        break;

        default:
            
        break;
        }        
    }
}
#endif // BOOT_PROGRAM

/* ʹ��soway��λ����������(Boot����), BOOT_PROGRAM��main.h�ж��� */
#ifdef BOOT_PROGRAM
#include "common.h"
#include "flash_if.h"
#endif

/**@brief       Modbus 41��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @return       ����ִ�н��
* - None
*/
void ModbusFunc41(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
/* ʹ��soway��λ����������(Boot����), BOOT_PROGRAM��main.h�ж��� */
#ifdef BOOT_PROGRAM
    
    uint16_t WriteAddr;                     //�Ĵ�����ַ    
    uint16_t DataLen;                       //���ݳ���
    uint16_t packetnum = 0;                 //�ܰ���
    uint16_t packetcnt;                     //�����
    uint16_t prt;                           //����
    uint32_t tpcksum;                       //��У���
    uint32_t *ramdata;                      //����ָ��
    static uint16_t PacketCnt;              // �����
    static uint16_t PacketNum;              // �ܰ���
    static uint32_t Flashadrdst;            // FLASH��ַ
    static uint32_t FileCheckSum;           // �����ļ�У���
    static uint32_t FileRunCheckSum;        // �����ļ�ʵʱУ���
    
    WriteAddr = (ModBusBaseParam->ModBus_TX_RX.Receive_Buf[2] << 8)
                | ModBusBaseParam->ModBus_TX_RX.Receive_Buf[3];
    DataLen = (ModBusBaseParam->ModBus_TX_RX.Receive_Buf[4] << 8)
                | ModBusBaseParam->ModBus_TX_RX.Receive_Buf[5];
    //�Ĵ�����ַ��Ч
    if( 
#if defined(SUBCODE_IS_DEVADDR)
        (WriteAddr >> 8) != ModBusBaseParam->Device_Addr ||
#endif
        (WriteAddr & 0xFF) < 1 
        || (WriteAddr & 0xFF) > 4
        || (DataLen != (ModBusBaseParam->ModBus_TX_RX.Receive_Len - 6)))
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] |= MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_ADDR_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
        return;
    }
    
    WriteAddr &= 0x00FF;
    
    //�洢��������Ϣ
    memcpy( ModBusBaseParam->ModBus_TX_RX.Send_Buf, 
            ModBusBaseParam->ModBus_TX_RX.Receive_Buf,
            4);
    ModBusBaseParam->ModBus_TX_RX.Send_Len = 4;

    ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = 0x00;
    ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = 0x01;

    switch(WriteAddr)
    {
        case 0x0001:                                                                                //��ʼ����      
            ModBusBaseParam->UpgradeWaitTime = -1;        
            if((0 != DataLen) && (2 != DataLen))
            {
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_REC_ERR;
                break;
            }
            else 
            {
                if(DataLen == 2)
                {
                    packetnum = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[6] * 256 
                                + ModBusBaseParam->ModBus_TX_RX.Receive_Buf[7];    //��ȡ�ܰ���
                }
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_ERR_NONE;
            }  
            PacketNum = 0;
            PacketCnt = 0;

            if(packetnum > 0)
            {           
                InFlash_Erase_Page(0, 4);
            }
            break;
          
        case 0x0002:                                                                                //���Դ����
            if(0 != DataLen)
            {
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_REC_ERR;
                break;
            }          
            FLASH_If_Init();                                                                          //FLASH����
            if(FLASH_If_GetWriteProtectionStatus() != FLASHIF_PROTECTION_NONE)
            {
                if(FLASH_If_WriteProtectionConfig(FLASHIF_WRP_DISABLE) == FLASHIF_OK)
                {
                    HAL_FLASH_OB_Launch();
                }
            }
            FLASH_If_Erase(APPLICATION_ADDRESS);
            Flashadrdst = APPLICATION_ADDRESS;
            ModBusBaseParam->ProgErase = ERASE_FLAG;
            ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_ERR_NONE;
            InMemory_Write_OneByte(ADDR_ERASEFLAG, ERASE_FLAG);
            break;

        case 0x0003:                //���������ļ�
            packetnum = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[6] * 256 + ModBusBaseParam->ModBus_TX_RX.Receive_Buf[7];    //��ȡ�ܰ���
            packetcnt = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[8] * 256 + ModBusBaseParam->ModBus_TX_RX.Receive_Buf[9];
            if((0 == PacketNum) && (1 < packetnum) && (0 == packetcnt))
            {
                FileCheckSum = 0;
                FileRunCheckSum = 0;
                PacketNum = packetnum;
                PacketCnt = packetcnt;
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_ERR_NONE;

                for(prt = 0; prt < 4; prt++)
                {
                    FileCheckSum <<= 8;
                    FileCheckSum += ModBusBaseParam->ModBus_TX_RX.Receive_Buf[10 + prt];
                }
            }
            else if((PacketNum == packetnum) && (1 < packetnum) 
                && (PacketCnt == (packetcnt - 1)) && (PacketNum > packetcnt))
            {
                tpcksum = 0;
                DataLen = DataLen - 4;

                for(prt = 0; prt < DataLen; prt++)
                {
                    tpcksum += ModBusBaseParam->ModBus_TX_RX.Receive_Buf[10 + prt];
                }

                Decoding(&ModBusBaseParam->ModBus_TX_RX.Receive_Buf[10], DataLen);

                for(prt = 0; prt < DataLen; prt++)
                {
                    ModBusBaseParam->ModBus_TX_RX.Receive_Buf[prt] = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[10 + prt];
                }
                ramdata = (uint32_t*)ModBusBaseParam->ModBus_TX_RX.Receive_Buf;

                if(FLASH_If_Write(Flashadrdst, ramdata, DataLen/4)  == 0)
                {
                    PacketCnt++;
                    ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_ERR_NONE;
                    FileRunCheckSum += tpcksum;
                    Flashadrdst += DataLen;
                }
                else
                {
                    ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_REC_ERR;
                }
            }
            else if((PacketNum == packetnum) && (1 < packetnum) 
                && (PacketCnt == packetcnt) && (PacketNum > packetcnt))
            {
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_ERR_NONE;
            }
            else
            {
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_REC_ERR;
            }
            break;

        case 0x0004:                                                                                  //ִ��Ӧ�ó���
            if((((FileRunCheckSum == FileCheckSum) && ((PacketCnt + 1) == PacketNum)) 
                || (0 == PacketNum))&&(0 == DataLen))
            {
                if(0 != PacketNum)
                {
                    ModBusBaseParam->UpgradeWaitTime = 0;
                    ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_ERR_NONE;
                }
                else if((ModBusBaseParam->ProgErase == ERASE_FLAG)
                    || (ModBusBaseParam->ProgErase == ERASE_FLAG_NONE))
                {
                    ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_REC_ERR;
                }
                else
                {
                    ModBusBaseParam->UpgradeWaitTime = 0;
                    ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_ERR_NONE;
                }
            }
            else
            {
                ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_LRC_ERR;
            }
            break;

        default:
            ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] = RESPONSE_REC_ERR;
            break;
    }
    
#else
    uint16_t temp[2];    
    
    temp[0] = ((uint16_t)ModBusBaseParam->ModBus_TX_RX.Receive_Buf[2] << 8) 
                | ModBusBaseParam->ModBus_TX_RX.Receive_Buf[3];
    temp[1] = ((uint16_t)ModBusBaseParam->ModBus_TX_RX.Receive_Buf[4] << 8) 
                | ModBusBaseParam->ModBus_TX_RX.Receive_Buf[5];
    if(temp[0] != 0x0001 || temp[1] != 0x0000)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[0] = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[0];
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] = ModBusBaseParam->ModBus_TX_RX.Receive_Buf[1] 
                                                    | MB_REQ_FAILURE;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = MB_VALU_EXCEPTION;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
    }
    else
    {
        memcpy(ModBusBaseParam->ModBus_TX_RX.Send_Buf, ModBusBaseParam->ModBus_TX_RX.Receive_Buf, 5);
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[5] = 0x01;
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[6] = 0x00;
        ModBusBaseParam->ModBus_TX_RX.Send_Len = 7;        
        ModBusBaseParam->ModBus_CallBack = MB_System_Reset;
    }
#endif
}

/* ʹ��soway��λ����������(Boot����), BOOT_PROGRAM��main.h�ж��� */
#ifndef BOOT_PROGRAM

/**@brief       Modbus ��Ϣ֡�Զ��ϴ�����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void ModbusAutoUpload(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg)
{
    float fbuf;
    uint8_t Nr;
    uint32_t DataBuf;
    //�豸����
    ModBus_Device_Param *Device_Param;
        
    Device_Param = (ModBus_Device_Param *)arg;

    ModBusBaseParam->ModBus_TX_RX.Send_Buf[0] = ModBusBaseParam->Device_Addr;
    ModBusBaseParam->ModBus_TX_RX.Send_Buf[1] = 0x04;
    ModBusBaseParam->ModBus_TX_RX.Send_Buf[2] = 0x08;

    ModBusBaseParam->ModBus_TX_RX.Send_Len = 3;
    fbuf = (float)Device_Param->PCap_DataConvert_Out->LiquidHeightAD;
    DataBuf = *(uint32_t *)&fbuf;
    for(Nr = 4; Nr > 0; Nr--)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] 
            = (uint8_t)(DataBuf >> ((Nr - 1)*8));
    }

    ModBusBaseParam->ModBus_TX_RX.Send_Len = 7;
    DataBuf = *(uint32_t *)&Device_Param->ADC_TemperOut->TemperInAir;
    for(Nr = 4; Nr > 0; Nr--)
    {
        ModBusBaseParam->ModBus_TX_RX.Send_Buf[ModBusBaseParam->ModBus_TX_RX.Send_Len++] 
            = (uint8_t)(DataBuf >> ((Nr - 1)*8));
    }
#if defined(USING_MODBUS_RTU)
    MODBUS_RTU_SendData(ModBusBaseParam, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Buf, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Len,
                        NO_CHECK_ADDRESS);
#elif defined(USING_MODBUS_ASCII)
    MODBUS_ASCII_SendData(ModBusBaseParam, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Buf, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Len,
                        NO_CHECK_ADDRESS);
#endif
}

/**@brief       ���ݱ궨
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void MB_Cap_Calibration(void *arg)
{
    uint8_t Cur_Param[8];
    //�豸����
    ModBus_Device_Param *Device_Param;
        
    Device_Param = (ModBus_Device_Param *)arg;
    
    if(Calib_CapMin < Calib_CapMax)
    {
        //���������뻺��
        Cur_Param[0] = (uint8_t)(Calib_CapMin >> 24);
        Cur_Param[1] = (uint8_t)(Calib_CapMin >> 16);
        Cur_Param[2] = (uint8_t)(Calib_CapMin >> 8);
        Cur_Param[3] = (uint8_t)Calib_CapMin;
        //����������뻺��
        Cur_Param[4] = (uint8_t)(Calib_CapMax >> 24);
        Cur_Param[5] = (uint8_t)(Calib_CapMax >> 16);
        Cur_Param[6] = (uint8_t)(Calib_CapMax >> 8);
        Cur_Param[7] = (uint8_t)Calib_CapMax;
        //�������
        Device_Param->PCap_DataConvert->CapMin = Calib_CapMin;
        //��������
        Device_Param->PCap_DataConvert->CapMax = Calib_CapMax;
        
        Device_Param->DataFilter->InputRangeMin = Calib_CapMin;
        Device_Param->DataFilter->InputRangeMax = Calib_CapMax;
        //���ݱ궨����д���ڲ�Flash
        InMemory_Write3T_MultiBytes(CAPMIN, Cur_Param, 8);
        //����������㱸��ֵ��ȱʡֵ��д��궨ֵ
        if(0xFFFFFFFF == Device_Param->PCap_DataConvert->CapMaxBak)
        {
            //���ݱ�������д���ڲ�Flash
            InMemory_Write3T_MultiBytes(CAPMINBAK, Cur_Param, 8);
            //������㱸��
            Device_Param->PCap_DataConvert->CapMinBak = Calib_CapMin;
            //�������㱸��
            Device_Param->PCap_DataConvert->CapMaxBak = Calib_CapMax;
        }
    }
    //Device_Param->PCap_DataConvert->CapRange = Device_Param->PCap_DataConvert->CapMax - Device_Param->PCap_DataConvert->CapMin;
    //����Һλ������ֵΪ���̵İٷ�֮��
    //Device_Param->PCap_DataConvert->CapWaveLimit = (uint32_t)(Device_Param->PCap_DataConvert->CapRange * 0.03);
}

/**@brief       ����ADֵ�궨
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void MB_CapAD_Calibration(void *arg)
{
    uint8_t Cur_Param[8];
    //�豸����
    ModBus_Device_Param *Device_Param;
        
    Device_Param = (ModBus_Device_Param *)arg;
    //�������AD�궨ֵ��Ч��д���ڲ�Flash
    if((Calib_ADMin < Calib_ADLow) && (Calib_ADLow < Calib_ADHigh) && (Calib_ADHigh < Calib_ADMax))
    {
        Cur_Param[0] = (uint8_t)(Calib_ADMin >> 8);
        Cur_Param[1] = (uint8_t)Calib_ADMin;
        Cur_Param[2] = (uint8_t)(Calib_ADLow >> 8);
        Cur_Param[3] = (uint8_t)Calib_ADLow;
        Cur_Param[4] = (uint8_t)(Calib_ADHigh >> 8);
        Cur_Param[5] = (uint8_t)Calib_ADHigh;
        Cur_Param[6] = (uint8_t)(Calib_ADMax >> 8);
        Cur_Param[7] = (uint8_t)Calib_ADMax;

        Device_Param->PCap_DataConvert->CapADMin = Calib_ADMin;
        Device_Param->PCap_DataConvert->CapADLow = Calib_ADLow;
        Device_Param->PCap_DataConvert->CapADHigh = Calib_ADHigh;
        Device_Param->PCap_DataConvert->CapADMax = Calib_ADMax;
        InMemory_Write3T_MultiBytes(CAPADMIN, Cur_Param, 8);
    }
}

/**@brief       ����DAֵ�궨
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void MB_CapDAOut_Calibration(void *arg)
{
    uint8_t Cur_Param[8];
    //�豸����
    ModBus_Device_Param *Device_Param;
    
    Device_Param = (ModBus_Device_Param *)arg;
    if(CapDA_ClibFlag == (CALIB_CAPDAMAX_FLAG | CALIB_CAPDAMIN_FLAG))
    {
        if(Calib_CapDAMax <=  Calib_CapDAMin)
        {
            return;
        }
        Calib_CapDAHigh = 0;
        Calib_CapDALow = 0;        
    }
    else
    {
        if((Calib_CapDAMax <= Calib_CapDAHigh) || (Calib_CapDAHigh <= Calib_CapDALow) 
            || (Calib_CapDALow <= Calib_CapDAMin))
        {
            return;
        }
    }                    
    Cur_Param[0] = (uint8_t)(Calib_CapDAMin >> 8);
    Cur_Param[1] = (uint8_t)Calib_CapDAMin;
    Cur_Param[2] = (uint8_t)(Calib_CapDALow >> 8);
    Cur_Param[3] = (uint8_t)Calib_CapDALow;
    Cur_Param[4] = (uint8_t)(Calib_CapDAHigh >> 8);
    Cur_Param[5] = (uint8_t)Calib_CapDAHigh;
    Cur_Param[6] = (uint8_t)(Calib_CapDAMax >> 8);
    Cur_Param[7] = (uint8_t)Calib_CapDAMax;
    InMemory_Write3T_MultiBytes(CAPDAMIN, Cur_Param, 8);
    
    Device_Param->PCap_DataConvert->CapDAMin = Calib_CapDAMin;
    Device_Param->PCap_DataConvert->CapDALow = Calib_CapDALow;
    Device_Param->PCap_DataConvert->CapDAHigh = Calib_CapDAHigh;
    Device_Param->PCap_DataConvert->CapDAMax = Calib_CapDAMax;
}

/**@brief       �¶�DAֵ�궨����
* @param[in]    arg : �û��Զ���Ĳ���,����Ϊ�豸����
* @return       ����ִ�н��
* - None
*/
void MB_TempDAOut_Calibration(void *arg)
{
    uint8_t Cur_Param[4];
    //�豸����
    ModBus_Device_Param *Device_Param;
    
    Device_Param = (ModBus_Device_Param *)arg;
    //����¶�DA�궨ֵ��Ч��д���ڲ�Flash
    if((Calib_TempDAMax > Calib_TempDAMin) && (Calib_TempDAMax <= DAC_VALUE_MAX))
    {
        Cur_Param[0] = (uint8_t)(Calib_TempDAMin >> 8);
        Cur_Param[1] = (uint8_t)Calib_TempDAMin;
        Cur_Param[2] = (uint8_t)(Calib_TempDAMax >> 8);
        Cur_Param[3] = (uint8_t)Calib_TempDAMax;
        InMemory_Write3T_MultiBytes(TEMPDAMIN, Cur_Param, 4);

        Device_Param->ADC_TemperParam->TempDAMin = Calib_TempDAMin;
        Device_Param->ADC_TemperParam->TempDAMax = Calib_TempDAMax;
        Device_Param->ADC_TemperParam->TempDARange = Device_Param->ADC_TemperParam->TempDAMax 
                                                - Device_Param->ADC_TemperParam->TempDAMin;
    }
}

/**@brief       Modbus �������³�ʼ��
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;  
* @return       ����ִ�н��
* - int����ֵ(OP_SUCCESS)
* @note         ����������ModBusBaseParam->ModBus_CallBack��,����ModBus��Ӧ��λ�������
*/
static int MB_USART_ReInit(ModBusBaseParam_TypeDef *ModBusBaseParam)
{        
/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
    rt_sem_take(ModBusBaseParam->TX_Lock, RT_WAITING_FOREVER);    //��ȡ�ź���
#else
    while(Sensor_USART_Get_TX_Cplt_Flag() == 0);    //�ȴ����ڷ������
#endif
    
    Sensor_USART_Init(  ModBusBaseParam->BaudRate, 
                        ModBusBaseParam->Parity);
    
/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
    rt_sem_release(ModBusBaseParam->TX_Lock);    //�ͷ��ź���
#endif  
    return OP_SUCCESS;
}

/**@brief       Modbus �豸����
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;  
* @return       ����ִ�н��
* - int����ֵ(OP_SUCCESS)
* @note         ����������ModBusBaseParam->ModBus_CallBack��,����ModBus��Ӧ��λ�������
*/
static int MB_Freeze(ModBusBaseParam_TypeDef *ModBusBaseParam)
{        
    ModBusBaseParam->Freeze = FREEZE_ENABLE;
    
    return OP_SUCCESS;
}

/**@brief       ϵͳ��λ
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;  
* @return       ����ִ�н��
* - int����ֵ(OP_SUCCESS)
* @note         ����������ModBusBaseParam->ModBus_CallBack��,����ModBus��Ӧ��λ�������
*/
static int MB_System_Reset(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */
#ifdef USING_RT_THREAD_OS
    rt_sem_take(ModBusBaseParam->TX_Lock, RT_WAITING_FOREVER);    //��ȡ�ź���
#else
    while(Sensor_USART_Get_TX_Cplt_Flag() == 0);    //�ȴ����ڷ������
#endif
    
    HAL_NVIC_SystemReset();
    
    return OP_SUCCESS;
}

/**@brief       ����ModBus�����Ҳ�����豸��ַ
* @param[in]    ModBusBaseParam : ModBus����Ļ��������ṹ��;  
* @return       ����ִ�н��
* - int����ֵ(OP_SUCCESS)
* @note         ����������ModBusBaseParam->ModBus_CallBack��,����ModBus��Ӧ��λ�������
*/
static int MB_SendData_NoCheck(ModBusBaseParam_TypeDef *ModBusBaseParam)
{
    //����Modbus RTU
#if defined(USING_MODBUS_RTU)
    MODBUS_RTU_SendData(ModBusBaseParam, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Buf, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Len,
                        NO_CHECK_ADDRESS);
#elif defined(USING_MODBUS_ASCII)
    MODBUS_ASCII_SendData(ModBusBaseParam, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Buf, 
                        ModBusBaseParam->ModBus_TX_RX.Send_Len,
                        NO_CHECK_ADDRESS);
#endif
    
    return OP_SUCCESS;
}

#endif // BOOT_PROGRAM

#endif // __PICOCAP_APP_H
