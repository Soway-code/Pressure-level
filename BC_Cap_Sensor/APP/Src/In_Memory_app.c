/**@file        In_Memory_app.c
* @brief        ��д�ڲ��洢����Ӧ��
* @details      �ڲ��洢���ж�ȡ��д��ϵͳ�ڲ�����
* @author       ׯ��Ⱥ
* @date         2020-07-20
* @version      V2.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2020/07/20  <td>2.0.0    <td>ׯ��Ⱥ  <td>���      <td>ά�������µİ汾
* </table>
*
**********************************************************************************
*/

#include "In_Memory_app.h"


/** Ĭ��ϵͳ���� */
const uint8_t User_Default_Param[PRO_DEFAULT_LEN] = 
{
    0x01,                                               //������ʼ����־λ

    0x01,                                               //�豸��ַ
    0x01,                                               //������(9600)
    0x00,                                               //��żУ��(��У��)
    0x01,                                               //�˲�ϵ��(10s)
    0x00,                                               //�Զ���������
    COMPENSATE_DISABLE,                                 //����ʹ��
    0x00,                                               //�Ƿ񶳽��豸
    0x00,                                               //�����ʽ

    0x00,0x94,                                          //����Ĭ��148(mm)

    0x00,0x19,0x43,0x4E,                                //������ֵ      1655630
    0x00,0x25,0x07,0xEF,                                //�����̵���ֵ    2426863
    0x00,0x19,0x43,0x4E,                                //�����ݱ���    1655630
    0x00,0x25,0x07,0xEF,                                //�����̵��ݱ���  2426863

    0x00,0x01,                                          //�������ADֵ    0
    0x51,0xA4,                                          //�����¿̶�ADֵ  20900
    0xB7,0x98,                                          //�����Ͽ̶�ADֵ  47000
    0xFF,0xFF,                                          //����������ADֵ  65535

    0x00,0x82,                                          //�������DAֵ   130
    0x02,0x44,                                          //�����¿̶�DAֵ 580
    0x03,0xA2,                                          //�����Ͽ̶�DAֵ 930
    0x04,0x6A,                                          //��������DAֵ   1130 
    0x00,0x00,                                          //�¶����DAֵ
    0x0F,0xFF,                                          //�¶�����DAֵ

    0x00,0x64,                                          //��������ϵ��K
    0x00,0x64,                                          //��������ϵ��B
    0x00,0x64,                                          //�¶�1����ϵ��K1
    0x00,0x64,                                          //�¶�1����ϵ��B1
    0x00,0x64,                                          //�¶�2����ϵ��K2
    0x00,0x64,                                          //�¶�2����ϵ��B2
    
    SYSTEMPARAM_IS_PROGRAMED                            //д���ʼֵ��־                                                                    
};


/**@brief       ���ڲ��洢��ָ��λ��д1���ֽ�
* @param[in]    RWAddr : д��ʼ��ַ
* @param[in]    WrData : Ҫд�������;
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ������ͨ�������ڲ�Flash��EEPROM����APIʵ�ֵ�
*/
uint8_t InMemory_Write_OneByte(uint16_t RWAddr, uint8_t WrData)
{
#if defined(__IN_EEPROM_H)
    return InEEPROM_Write_OneByte(RWAddr, WrData);
#elif defined(__IN_FLASH_H)
    return InFlash_Write_OneByte(RWAddr, WrData);
#endif // defined(__IN_EEPROM_H) or defined(__IN_FLASH_H)
}

/**@brief       ���ڲ��洢��ָ��λ�ö�1���ֽ�
* @param[in]    RWAddr : ����ʼ��ַ
* @return       ����ִ�н��
* - 1���ֽ�����
* @note         ������ͨ�������ڲ�Flash��EEPROM����APIʵ�ֵ�
*/
uint8_t InMemory_Read_OneByte(uint16_t RWAddr)
{
#if defined(__IN_EEPROM_H)
    return InEEPROM_Read_OneByte(RWAddr);
#elif defined(__IN_FLASH_H)
    return InFlash_Read_OneByte(RWAddr);
#endif // defined(__IN_EEPROM_H) or defined(__IN_FLASH_H)
}
/**@brief       ���ڲ��洢��ָ��λ��д����ֽ�
* @param[in]    RWAddr : д��ʼ��ַ
* @param[in]    pWrbuf : Ҫд������ݻ���ָ��;
* @param[in]    Wrlen : ���ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ������ͨ�������ڲ�Flash��EEPROM����APIʵ�ֵ�
*/
uint8_t InMemory_Write_MultiBytes(uint16_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen)
{
#if defined(__IN_EEPROM_H)
    return InEEPROM_Write_MultiBytes(RWAddr, pWrbuf, Wrlen);
#elif defined(__IN_FLASH_H)
    return InFlash_Write_MultiBytes(RWAddr, pWrbuf, Wrlen);
#endif // defined(__IN_EEPROM_H) or defined(__IN_FLASH_H)
}

/**@brief       ���ڲ��洢��ָ��λ�ö�����ֽ�
* @param[in]    RWAddr : ����ʼ��ַ
* @param[in]    pWrbuf : Ҫ��ȡ�����ݻ���ָ��;
* @param[in]    Wrlen : ���ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ������ͨ�������ڲ�Flash��EEPROM����APIʵ�ֵ�
*/
void InMemory_Read_MultiBytes(uint16_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen)
{
#if defined(__IN_EEPROM_H)
    InEEPROM_Read_MultiBytes(RWAddr, pRdbuf, Rdlen);
#elif defined(__IN_FLASH_H)
    InFlash_Read_MultiBytes(RWAddr, pRdbuf, Rdlen);
#endif // defined(__IN_EEPROM_H) or defined(__IN_FLASH_H)
}

/**@brief       ��STM32F072xx�ڲ�Flashָ��λ��д����ֽ��ұ���3��
* @param[in]    FlashAddr : д��ʼ��ַ
* @param[in]    pWrbuf : Ҫд������ݻ���ָ��;
* @param[in]    Wrlen : ���ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
*/
uint8_t InMemory_Write3T_MultiBytes(uint16_t FlashAddr, const uint8_t *pWrbuf, uint16_t Wrlen)
{
    //����״̬
    uint8_t Err;
    //д״̬
    uint8_t Wrsta;
    //д��ʼ��ַ
    uint16_t EepAddress;

    Err = OP_SUCCESS;
    Wrsta = OP_SUCCESS;
    EepAddress = FlashAddr;

    //ϵͳ�����洢��
    Wrsta = InMemory_Write_MultiBytes(EepAddress, pWrbuf, Wrlen);
    if(OP_SUCCESS != Wrsta)
    {
        Err = OP_FAILED;
    }
    //ϵͳ����������1
    Wrsta = InMemory_Write_MultiBytes(SYSTEM_PARAM_BAK1 + EepAddress, pWrbuf, Wrlen);
    if(OP_SUCCESS != Wrsta)
    {
        Err = OP_FAILED;
    }
    //ϵͳ����������2
    Wrsta = InMemory_Write_MultiBytes(SYSTEM_PARAM_BAK2 + EepAddress, pWrbuf, Wrlen);
    if(OP_SUCCESS != Wrsta)
    {
        Err = OP_FAILED;
    }

    return Err;
}

/**@brief       �ڲ�Flashϵͳ�������,�����ֲ�һ�µĲ���,���½�User_Default_Param
* д���ڲ�Flash
* @param[in]    Cur_Param : Ҫ���Ĳ�������;
* @param[in]    Num_Of_Cur_Param : ��������ĳ���;
* @return       ����ִ�н��
* - None
*/
void InMemory_SystemParam_Check(uint8_t *Cur_Param, uint16_t Num_Of_Cur_Param)
{
    uint16_t Cnt;
    uint8_t Check_Sta;
    uint8_t Cur_Param_Bak1;
    uint8_t Cur_Param_Bak2;

    for(Cnt = 0; Cnt < Num_Of_Cur_Param; Cnt++)
    {
        //���״̬
        Check_Sta = 0;
        //����1
        Cur_Param_Bak1 = InMemory_Read_OneByte((SYSTEM_PARAM_BAK1 + Cnt));
        //����2
        Cur_Param_Bak2 = InMemory_Read_OneByte((SYSTEM_PARAM_BAK2 + Cnt));
        //��ǰ�����뱸��1��ͬ
        if(Cur_Param[Cnt] != Cur_Param_Bak1)
        {
            Check_Sta |= 0x01;
        }
        //����1�뱸��2��ͬ
        if(Cur_Param_Bak1 != Cur_Param_Bak2)
        {
            Check_Sta |= 0x02;
        }
        //��ǰ�����뱸��2��ͬ
        if(Cur_Param[Cnt] != Cur_Param_Bak2)
        {
            Check_Sta |= 0x04;
        }
        //����2����
        if(0x06 == Check_Sta)
        {
            InMemory_Write_OneByte((SYSTEM_PARAM_BAK2 + Cnt), Cur_Param[Cnt]);
        }
        //����1����
        else if(0x03 == Check_Sta)
        {
            InMemory_Write_OneByte((SYSTEM_PARAM_BAK1 + Cnt), Cur_Param[Cnt]);
        }
        //��ǰ��������
        else if(0x05 == Check_Sta)
        {
            InMemory_Write_OneByte((RUN_ADDR_BASE + Cnt), Cur_Param_Bak1);
        }
        else 
        {
            if(0x00 != Check_Sta)
            {
            break;
            }
        }
    }

    if(InMemory_Read_OneByte(SYSTEMPARAM_PROGRAMED) != SYSTEMPARAM_IS_PROGRAMED)
    {
        goto write_systemparam;
    }
    if(InMemory_Read_OneByte(SYSTEM_PARAM_BAK1 + SYSTEMPARAM_PROGRAMED) != SYSTEMPARAM_IS_PROGRAMED)
    {
        goto write_systemparam;
    }
    if(InMemory_Read_OneByte(SYSTEM_PARAM_BAK2 + SYSTEMPARAM_PROGRAMED) != SYSTEMPARAM_IS_PROGRAMED)
    {
        goto write_systemparam;
    }
    //�����������ȫ���Ծ�д��ȱʡϵͳ����
    if((0x01 == Check_Sta) || (0x02 == Check_Sta) || (0x04 == Check_Sta) || (0x07 == Check_Sta))
    {
write_systemparam:
        //д��ϵͳ����ȱʡֵ�����ݵ�ַ��
        InMemory_Write3T_MultiBytes(RUN_ADDR_BASE, User_Default_Param, Num_Of_Cur_Param);
    }
}

/**@brief       ��ȡ����鱣�����ڲ�Flash���豸����
* @return       ����ִ�н��
* - None
*/
void Check_Device_Param(void)
{
    uint8_t InMemoryInitFlg;
    uint16_t ParaTemp;
    uint8_t Device_Param[PRO_DEFAULT_LEN];

    //��ȡϵͳ������ʼ����־
    InMemoryInitFlg = InMemory_Read_OneByte(RUN_ADDR_BASE);

    //���û�г�ʼ����д���û������������궨�Ĳ������ܶ�
    if(InMemoryInitFlg != User_Default_Param[RUN_ADDR_BASE])
    {
        ParaTemp = 0;
        while(OP_SUCCESS != InMemory_Write3T_MultiBytes(RUN_ADDR_BASE, User_Default_Param, USER_DEFAULT_LEN))
        {
            ParaTemp++;
            if(IN_MEMORY_ERR_MAX < ParaTemp)
            {
            break;
            }
        }
    }

    //��ȡϵͳ����������Device_Param����
    InMemory_Read_MultiBytes(RUN_ADDR_BASE, Device_Param, PRO_DEFAULT_LEN);

    //���ϵͳ����
    InMemory_SystemParam_Check(Device_Param, PRO_DEFAULT_LEN);
}

