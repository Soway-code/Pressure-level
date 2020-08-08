/**@file        TypeConvert.h
* @details      TypeConvert.c��ͷ�ļ�,��������������ת��ģ���API����
* @author       ���
* @date         2020-04-29
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/04/29  <td>1.0.0    <td>���    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#ifndef __TYPECONVERT_H
#define __TYPECONVERT_H

#include "stdint.h"

/**@brief       ��32λ���ͱ���(���ģʽ)ת���ɸ�����ֵ(С��ģʽ)
* @param[in]    pData : �޷����ַ���ָ��
* @return       ����ִ�н��
* - ת����ĸ�������ֵ
*/
float HexToFloat(uint8_t *pData);

/**@brief       ��32λ���ͱ���(���ģʽ)ת�����޷���32λ����ֵ(С��ģʽ)
* @param[in]    pData : �޷����ַ���ָ��
* @return       ����ִ�н��
* - ת������޷���32λ����ֵ
*/
uint32_t HexToUlong(uint8_t *pData);

/**@brief       �������ͱ���(С��ģʽ)ת�����޷���8λ����ֵ������(���ģʽ)
* @param[in]    fSource : �����ͱ���; 
* @param[in]    num : ������ֽڸ���
* @param[out]   pu8Temp : ת����Ľ��
* @return       ����ִ�н��
* - None
*/
void floatToHexArray(float fSource, uint8_t *pu8Temp, uint16_t num);

/**@brief       ����Array[]���鰴ָ��ͷ����SortHeadSize��β����SortTailSize����˳�����к�,
* ��Array[DelHeadSize]��Array[ArraySize - DelTailSize - 1]��ƽ��ֵ
* @param[in]    Array[] : Ҫ�������в�����ƽ��ֵ������;           
* @param[in]    ArraySize : ���鳤��;
* @param[in]    SortHeadSize : ����ͷ�����г���,����ƽ��ֵʱ����ͷ���޳��ĳ���;
* @param[in]    SortTailSize : ����β�����г���,����ƽ��ֵʱ����β���޳��ĳ���;
* @return       ����ִ�н��
* - Array[]���鰴ָ��ͷ����SortHeadSize��β����SortTailSize����˳�����к�,
* ��Array[DelHeadSize]��Array[ArraySize - DelTailSize - 1]��ƽ��ֵ
*/
uint32_t GetDelExtremeAndAverage(uint32_t Array[], uint32_t ArraySize, uint32_t SortHeadSize, uint32_t SortTailSize);

#endif // __TYPECONVERT_H
