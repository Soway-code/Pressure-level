#ifndef __MODBUSRTU_H
#define __MODBUSRTU_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "usart_app.h"
#include "ModBus_Conf.h"

//发送缓存大小
#define SEND_SIZE                       32

#ifndef RECEIVE_SIZE
//接收缓存的大小
#define RECEIVE_SIZE                    32
#endif

//自动上传周期,单位 ms
#define AUTOUPLOAD_CYCLE                1000

#define BROADCASTADDR                     0x00                                  //广播地址

#define HOLDING_REG_REGION1_BGEIN         0x0030                                //保持寄存器起始地址1
#define HOLDING_REG_REGION1_END           0x0041                                //保持寄存器结束地址1

#define INPUT_REG_REGION1_BEGIN           0x0000                                //输入寄存器起始地址1
#define INPUT_REG_REGION1_END             0x0006                                //输入寄存器结束地址1
#define INPUT_REG_REGION2_BEGIN           0x0080                                //输入寄存器起始地址2
#define INPUT_REG_REGION2_END             0x0080                                //输入寄存器结束地址2

#define SINGLE_COIL_REGION1_BEGIN         0x0050                                //单个线圈寄存器起始地址1
#define SINGLE_COIL_REGION1_END           0x0053                                //单个线圈寄存器结束地址1

#define SPECIAL_COIL_REGION1_BEGIN        0x0000                                //特殊寄存器起始地址1
#define SPECIAL_COIL_REGION1_END          0x0005                                //特殊寄存器结束地址1

#define MUL_REG_REGION1_BEGIN             0x0030                                //多个寄存器起始地址1
#define MUL_REG_REGION1_END               0x0041                                //多个寄存器结束地址1

#define DEF_MUL_REG_REGION1_BEGIN         0x0080                                //自定义26/27功能码寄存器起始地址1
#define DEF_MUL_REG_REGION1_END           0x00A2                                //自定义26/27功能码寄存器结束地址1
//#define DEF_MUL_REG_REGION2_BEGIN         0x0090                                //自定义26/27功能码寄存器起始地址2
//#define DEF_MUL_REG_REGION2_END           0x0096                                //自定义26/27功能码寄存器结束地址2
//#define DEF_MUL_REG_REGION3_BEGIN         0x00B0                                //自定义26/27功能码寄存器起始地址3
//#define DEF_MUL_REG_REGION3_END           0x00C0                                //自定义26/27功能码寄存器结束地址3

#define MUL_VERSION_INF_BEGIN             0x00E0                                //版本信息起始地址
#define MUL_VERSION_INF_END               0x00E5                                //版本信息结束地址

#define MB_NONE_EXCEPTION                 0x00                                  //Modbus无异常
#define MB_FUNC_EXCEPTION                 0x01                                  //Modbus功能码无效
#define MB_ADDR_EXCEPTION                 0x02                                  //Modbus访问地址失败
#define MB_VALU_EXCEPTION                 0x03                                  //Modbus访问数值无效
#define MB_DEVC_EXCEPTION                 0x04                                  //Modbus访问设备失败
#define MB_ACKN_EXCEPTION                 0x05                                  //Modbus访问设备无应答
#define MB_BUSY_EXCEPTION                 0x06                                  //Modbus访问设备忙
#define MB_PARI_EXCEPTION                 0x08                                  //Modbus设备校验错误
#define MB_REQ_FAILURE                    0x80                                  //Modbus请求错误

#define CALIB_CAPMIN_FLAG                 0x01                                  //标定电容零点标志
#define CALIB_CAPMAX_FLAG                 0x10                                  //标定电容满度标志
#define CALIB_CAPEOC_FLAG                 0x11                                  //标点电容结束标志

#define CALIB_CAPADMIN_FLAG               0x01                                  //标定电容AD零点标志
#define CALIB_CAPADLOW_FLAG               0x02                                  //标定电容AD下刻度标志
#define CALIB_CAPADHIH_FLAG               0x10                                  //标定电容AD上刻度标志
#define CALIB_CAPADMAX_FLAG               0x20                                  //标定电容AD满度标志
#define CALIB_CAPADEOC_FLAG               0x33                                  //标定电容AD结束标志

#define CALIB_CAPDAMIN_FLAG               0x01                                  //标定电容DA零点标志
#define CALIB_CAPDALOW_FLAG               0x02                                  //标定电容DA下刻度标志
#define CALIB_CAPDAHIH_FLAG               0x10                                  //标定电容DA上刻度标志
#define CALIB_CAPDAMAX_FLAG               0x20                                  //标定电容DA满度标志
#define CALIB_CAPDAEOC_FLAG               0x33                                  //标定电容DA结束标志

#define CALIB_TEMPDAMIN_FLAG              0x01                                  //标定温度DA零点标志
#define CALIB_TEMPDAMAX_FLAG              0x10                                  //标定温度DA满度标志
#define CALIB_TEMPDAEOC_FLAG              0x11   

//冻结设备
#define FREEZE_ENABLE                                0x0F
//解冻设备
#define FREEZE_DISABLE                               0x00

#define ADCLIB_ENABLE          0x0F
#define ADCLIB_DISABLE         0x00

#define NO_CHECK_ADDRESS                0
#define CHECK_ADDRESS                   1
    
    
/* ModBus发送/接收的缓存指针和数据长度结构 */
typedef struct {
    uint8_t *Send_Buf;
    uint16_t Send_Len;
    uint8_t *Receive_Buf;
}ModBus_TX_RX_TypeDef;

/* ModBus处理的基本参数结构 */
typedef struct _ModBusBaseParam_TypeDef {
    uint8_t Device_Addr;
    uint8_t BaudRate;
    uint8_t Parity;
    uint8_t AutoUpload;
    uint8_t Output_Mode;
    uint8_t Factory_Reset;
    uint8_t Freeze;
    uint8_t InRomWrEn;
    ModBus_TX_RX_TypeDef ModBus_TX_RX;
#ifdef USING_RT_THREAD_OS
    rt_sem_t TX_Lock;
#endif
    int     (*ModBus_CallBack) (struct _ModBusBaseParam_TypeDef *arg);
}ModBusBaseParam_TypeDef;


#ifdef __IN_FLASH_APP_H
/**
* 名称       : ModBus_Init()
* 创建日期   : 2020-05-07
* 作者       : 杨春林
* 功能       : ModBus初始化
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModBus_Init(ModBusBaseParam_TypeDef *ModBusBaseParam);
#else
/**
* 名称       : ModBus_Init()
* 创建日期   : 2020-05-07
* 作者       : 杨春林
* 功能       : ModBus初始化
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              Param的数据内容(按字节顺序排列): 0,设备地址; 
*              1,波特率代码; 2奇偶校验代码; 3自动上传周期;
*              4,输出模式; 5,设备冻结标志;
* 输出参数   : 无
* 返回结果   : OP_SUCCESS(成功)，OP_FAILED(失败)
* 注意和说明 : 
* 修改内容   :
*/
uint8_t ModBus_Init(ModBusBaseParam_TypeDef *ModBusBaseParam,
                uint8_t *Param, uint16_t Param_Size);
#endif

/**
* 名称       : MDB_Get_CRC16()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 对输入的消息帧进行CRC16校验
* 输入参数   : Msg:消息首地址; MsgLen:消息帧长度;
* 输出参数   : 无
* 返回结果   : CRC16校验码
* 注意和说明 : 消息长度不能为0
* 修改内容   :
*/
uint16_t MDB_Get_CRC16(uint8_t *Msg, uint16_t MsgLen);

/**
* 名称       : MDB_Snd_Msg_RTU()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 发送 Modbus RTU消息帧
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              Msg:消息首地址; 
*              MsgLen:消息帧长度;
*              Check_Addr:是否检查设备地址有效
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 消息长度不能为0，发送时先解冻设备，设备地址不能为广播地址
* 修改内容   :
*/
void MDB_Snd_Msg_RTU(ModBusBaseParam_TypeDef *ModBusBaseParam, uint8_t *pMsg, uint16_t len, uint8_t Check_Addr);

/**
* 名称       : Send_Data()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 发送 Modbus 数据
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              Msg:消息首地址; MsgLen:消息帧长度;
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void Send_Data(ModBusBaseParam_TypeDef *ModBusBaseParam, uint8_t *pMsg, uint16_t len);

/**
* 名称       : ModbusHandle()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : Modbus RTU消息帧处理函数
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              arg:用户自定义的参数,例如设备参数
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModbusHandle(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**
* 名称       : ModbusFunc03()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : Modbus 03功能码消息帧处理
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              arg:用户自定义的参数,例如设备参数
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModbusFunc03(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**
* 名称       : ModbusFunc04()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : Modbus 04功能码消息帧处理
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              arg:用户自定义的参数,例如设备参数
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModbusFunc04(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**
* 名称       : ModbusFunc05()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : Modbus 05功能码消息帧处理
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              arg:用户自定义的参数,例如设备参数
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModbusFunc05(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**
* 名称       : ModbusFunc10()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : Modbus 10功能码消息帧处理
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              arg:用户自定义的参数,例如设备参数
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModbusFunc10(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**
* 名称       : ModbusFunc25()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : Modbus 25功能码消息帧处理
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              arg:用户自定义的参数,例如设备参数
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModbusFunc25(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**
* 名称       : ModbusFunc26()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : Modbus 26功能码消息帧处理
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              arg:用户自定义的参数,例如设备参数
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModbusFunc26(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**
* 名称       : ModbusFunc27()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : Modbus 27功能码消息帧处理
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              arg:用户自定义的参数,例如设备参数
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModbusFunc27(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**
* 名称       : ModbusFunc2A()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : Modbus 2A功能码消息帧处理
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModbusFunc2A(ModBusBaseParam_TypeDef *ModBusBaseParam);

/**
* 名称       : ModbusFunc2B()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : Modbus 2B功能码消息帧处理
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModbusFunc2B(ModBusBaseParam_TypeDef *ModBusBaseParam);

/**
* 名称       : ModbusFunc41()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : Modbus 41功能码消息帧处理
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModbusFunc41(ModBusBaseParam_TypeDef *ModBusBaseParam);

/**
* 名称       : ModbusAutoUpload()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : Modbus 消息帧自动上传处理
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              arg:用户自定义的参数,例如设备参数
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void ModbusAutoUpload(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**
* 名称       : SensorEvent()
* 创建日期   : 2020-04-29
* 作者       : 杨春林
* 功能       : 传感器事件处理
* 输入参数   : ModBusBaseParam: ModBus处理的基本参数结构体;
*              arg:用户自定义的参数,例如设备参数
* 输出参数   : 无
* 返回结果   : 无
* 注意和说明 : 
* 修改内容   :
*/
void SensorEvent(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);


#ifdef __cplusplus
}
#endif
#endif
