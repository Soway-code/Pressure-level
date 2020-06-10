#ifndef __TYPECONVERT_H
#define __TYPECONVERT_H

#include "stdint.h"

/**
* 名称     : HexToFloat()
* 功能     : 将32位整型变量(大端模式)转换成浮点型值(小端模式)
* 输入参数 : pData,无符号字符型指针
* 输出参数 : 无
* 返回结果 : 转换后的浮点型数值
*/
float HexToFloat(uint8_t *pData);

/**
* 名称     : HexToUlong()
* 功能     : 将32位整型变量(大端模式)转换成无符号32位整型值(小端模式)
* 输入参数 : pData,无符号字符型指针
* 输出参数 : 无
* 返回结果 : 转换后的无符号32位整型值
*/
uint32_t HexToUlong(uint8_t *pData);

/**
* 名称     : floatToHexArray()
* 功能     : 将浮点型变量(小端模式)转换成无符号8位整型值的数组(大端模式)
* 输入参数 : fSource:浮点型变量; num:输入的字节个数
* 输出参数 : pu8Temp:转换后的结果
* 返回结果 : 无
*/
void floatToHexArray(float fSource, uint8_t *pu8Temp, uint16_t num);

/**
* 名称     : GetDelExtremeAndAverage()
* 功能     : 计算Array[]数组按指定头长度SortHeadSize和尾长度SortTailSize进行顺序排列后,
*            从Array[DelHeadSize]到Array[ArraySize - DelTailSize - 1]的平均值
* 输入参数 : Array[]:要进行排列并计算平均值的数组; ArraySize:数组长度;
*            SortHeadSize:数组头部排列长度,计算平均值时数组头部剔除的长度; 
*            SortTailSize:数组尾部排列长度,计算平均值时数组尾部剔除的长度;
* 输出参数 : 无
* 返回结果 : Array[]数组按指定头长度SortHeadSize和尾长度SortTailSize进行顺序排列后,
*            从Array[DelHeadSize]到Array[ArraySize - DelTailSize - 1]的平均值
*/
uint32_t GetDelExtremeAndAverage(uint32_t Array[], uint32_t ArraySize, uint32_t SortHeadSize, uint32_t SortTailSize);

#endif
