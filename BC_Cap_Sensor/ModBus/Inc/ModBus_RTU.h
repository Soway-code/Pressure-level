/**@file        ModBus_RTU.h
* @details      ModBus_RTU.c的头文件,声明了ModBus RTU 报文处理的API函数
* @author       庄明群
* @date         2020-07-20
* @version      V2.0.0
* @copyright    2020-2030,深圳市信为科技发展有限公司
**********************************************************************************
* @par 修改日志:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2020/07/20  <td>2.0.0    <td>庄明群  <td>杨春林      <td>新添加的程序代码(RTU协议部分)
* </table>
*
**********************************************************************************
*/

#ifndef _MODBUS_ASCII_H
#define _MODBUS_ASCII_H
#ifdef __cplusplus
 extern "C" {
#endif
     
#include "ModBus.h"


/**@brief       对输入的消息包进行CRC16校验
* @param[in]    Msg : 消息包缓存;
* @param[in]    MsgLen : 消息包长度;
* @return       函数执行结果
* - CRC16校验码
* @note         消息长度不能为0
*/
uint16_t MDB_Get_CRC16(uint8_t *Msg, uint16_t MsgLen);

/**@brief       发送 Modbus RTU 消息包
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @param[in]    pMsg : 消息包缓存; 
* @param[in]    len : 消息包长度;
* @param[in]    Check_Addr : 是否检查设备地址有效
* @return       函数执行结果
* - None
* @note         消息长度不能为0，发送时先解冻设备
*/
void MODBUS_RTU_SendData(ModBusBaseParam_TypeDef *ModBusBaseParam, uint8_t *pMsg, uint16_t len, uint8_t Check_Addr);

/**@brief       获取 Modbus RTU 消息包
* @param[in]    cyRecvBuff : 消息包缓存; 
* @param[in]    pCyLen : 输出的消息包长度(无错误则不包括CRC);
* @return       函数执行结果
* - ERR_INVALID_DATA : 无效数据
* - ERR_CRC : CRC校验错误
* - ERR_NONE : 无错误
*/
uint8_t MODBUS_RTU_RecvData(uint8_t* cyRecvBuff, uint16_t *pCyLen);

#ifdef __cplusplus
}
#endif
#endif // _MODBUS_ASCII_H

