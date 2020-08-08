/**@file        Picocap.h
* @details      Picocap.c��ͷ�ļ�,������PCap������API����,
* �����˼Ĵ����Ĳ���
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

#ifndef __PICOCAP_H
#define __PICOCAP_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "VariaType.h"

extern const unsigned char SRAM_DATA[];
/** PCap�̼���С */
#define SRAM_DATA_NUM       sizeof(SRAM_DATA) / sizeof(SRAM_DATA[0])

extern const unsigned long REG[];
/** PCap�������ݴ�С */
#define REG_CONF_NUM        sizeof(REG) / sizeof(REG[0])

#define WRITE_PCAPFIRM_ERR_MAX              10          /** дPCAP�̼����������� */


/******************************** ����Ϊ21�����üĴ����ľ���ֵ ***************************************/
/** REG0��OTP���üĴ������� */
/** SRAM��OTP�ıȽϻ��� */
#define REG0_MEMCOMP_OFF                    0x00        /** �ر� */
#define REG0_MEMCOMP_5BYTE                  (1 << 18)   /** 5 Byte */
#define REG0_MEMCOMP_33BYTE                 (2 << 18)   /** 33 Byte */
#define REG0_MEMCOMP_257BYTE                (3 << 18)   /** 257 Byte */

/** OTP-�ڲ���������޸����� */
#define REG0_ECC_MODE_DISABLE               0x00        /** �ر�(Singleģʽ)������4032Byte */
#define REG0_ECC_MODE_DOUBLE                (0x0F << 8) /** Doubleģʽ������4032Byte */
#define REG0_ECC_MODE_QUAD                  (0xF0 << 8) /** Quadģʽ������1984Byte */

/** �Զ���OTP������ */
#define REG0_AUTOBOOT_ENABLE                0x00        /** ��оƬ����ģʽ��оƬ�����Զ���OTP���������� */
#define REG0_AUTOBOOT_DISABLE               (0x0F << 4) /** �ӻ�ģʽ��оƬͨ��ͨѶ�ӿڱ���λ���� */

/** �洢������ */
#define REG0_MEM_LOCK_ENABLE                0x00        /** ������ֹ�洢���ݶ�������ֹ�̼�������Ϣ������ */
#define REG0_MEM_LOCK_DISABLE               (0x0F << 0) /** ��������ֹ�洢���ݶ��� */


/** REG1�������ڲ�����Ƶ�� */
#define REG1_OLF_TUNE_10KHZ                 0x35        /** 10kHz */
#define REG1_OLF_TUNE_50KHZ                 0x22        /** 50kHz */
#define REG1_OLF_TUNE_100KHZ                0x13        /** 100kHz */
#define REG1_OLF_TUNE_200KHZ                0x04        /** 200kHz */


/** REG2�����ݲ������� */
/** CDC�˿�ʹ�� */
#define REG2_CMEAS_PC0_EN                   (0x01 << 16)/** ʹ��PC0�˿� */    
#define REG2_CMEAS_PC1_EN                   (0x02 << 16)/** ʹ��PC1�˿� */  
#define REG2_CMEAS_PC2_EN                   (0x04 << 16)/** ʹ��PC2�˿� */  
#define REG2_CMEAS_PC3_EN                   (0x08 << 16)/** ʹ��PC3�˿� */  
#define REG2_CMEAS_PC4_EN                   (0x10 << 16)/** ʹ��PC4�˿� */  
#define REG2_CMEAS_PC5_EN                   (0x20 << 16)/** ʹ��PC5�˿� */  
#define REG2_CMEAS_PC6_EN                   (0x40 << 16)/** ʹ��PC6�˿� */  
#define REG2_CMEAS_PC7_EN                   (0x80 << 16)/** ʹ��PC7�˿� */  

/** ���������ӷ�ʽ */
#define REG2_CMEAS_GROUND_SINGLE            0x00        /** ���˽ӵ� */
#define REG2_CMEAS_GROUND_DIFFERENTIAL      0x00        /** ��ֽӵ� */
#define REG2_CMEAS_FLOATING_SINGLE          (0x01 << 14)/** ���˸��� */
#define REG2_CMEAS_FLOATING_DIFFERENTIAL    (0x02 << 14)/** ��ָ��� */

/** ��ֵ��ݾ���ԳƳ�ŵ��ѡ�񣬷�ֹ��еӦ�� */
#define REG2_CMEAS_DUMMY_DISABLE            0x00        /** �ر� */
#define REG2_CMEAS_DUMMY_ENABLE             (0x01 << 13)/** ʹ�� */

/** �ⲿ�������ݵĲ������� */
#define REG2_COMPENS_EXT_DISABLE            0x00        /** �ر� */
#define REG2_COMPENS_EXT_ENABLE             (0x01 << 12)/** ʹ�� */

/** �ڲ��������ݵĲ������� */
#define REG2_COMPENS_INT_DISABLE            0x00        /** �ر� */
#define REG2_COMPENS_INT_ENABLE             (0x01 << 11)/** ʹ�� */

/** ѡ���ڲ��ŵ���� */
#define REG2_RDCHG_INT_180K                 (0x04 << 8)/** 180Kŷ */
#define REG2_RDCHG_INT_90K                  (0x05 << 8)/** 90Kŷ */
#define REG2_RDCHG_INT_30K                  (0x06 << 8)/** 30Kŷ */
#define REG2_RDCHG_INT_10K                  (0x07 << 8)/** 10Kŷ */


/** REG3�����ݲ������� */
/** CDCѭ����ʱ�ӻ�׼ */
#define REG3_CY_CLK_20US                    0x00        /** 20 ��s (������Ƶʱ�ӵ�Ӧ��) */
#define REG3_CY_CLK_1US                     (0x02 << 22)/** 1 ��s (����Ӧ���ⲿ 4 MHz��) */
#define REG3_CY_CLK_0_25US                  (0x03 << 22)/** 0.25 ��s (����Ӧ���ⲿ 4 MHz��) */

/** ��ʱ��-����ģʽ���ô��������� (�Ĵ���4, TMEAS_TRIG_SEL = 2) */
#define REG3_SEQ_TIME_CONFIG                13          /** ���ô�������������Χ��1~24��0 = �ر� */
#define REG3_SEQ_TIME                       (REG3_SEQ_TIME_CONFIG << 16)

/** �趨CDCÿ�β�����ļٲ����������ɴ�0��4ѡ�� */
#define REG3_CMEAS_FAKE_NONE                0x00        /** �޼ٲ��� */
#define REG3_CMEAS_FAKE_1_TIME              (0x01 << 13)/** 1�μٲ��� */
#define REG3_CMEAS_FAKE_2_TIMES             (0x02 << 13)/** 2�μٲ��� */
#define REG3_CMEAS_FAKE_4_TIMES             (0x03 << 13)/** 4�μٲ��� */

/** CDC �������ڲ�ƽ���Ĵ���������Ϊ0��Ч��Ĭ��Ϊ1 */
#define REG3_C_AVRG_CONFIG                  512         /** �����ڲ�ƽ����������Χ��0~4095 */
#define REG3_C_AVRG                         REG3_C_AVRG_CONFIG


/** REG4������/�¶Ȳ������� */
/** ѡ����Ӧ��������ݲ��������崥���ܽ� */
#define REG4_CMEAS_STARTPIN_PG0             0x00        /** PG0 */
#define REG4_CMEAS_STARTPIN_PG1             (0x01 << 20)/** PG1 */
#define REG4_CMEAS_STARTPIN_PG2             (0x02 << 20)/** PG2 */
#define REG4_CMEAS_STARTPIN_PG3             (0x03 << 20)/** PG3 */

/** ѡ����ݲ����Ĵ���Դ */
#define REG4_CMEAS_TRIG_SEL_SOFT            0x00        /** ��������� */
#define REG4_CMEAS_TRIG_SEL_CONTINUE        (0x01 << 18)/** ����ģʽ */
#define REG4_CMEAS_TRIG_SEL_CLOCK           (0x02 << 18)/** ʱ�Ӵ���ģʽ */
#define REG4_CMEAS_TRIG_SEL_PULSE           (0x03 << 18)/** ���崥��ģʽ */

/** �������ڵ��ݲ����� cycle time��CDC ����ʱ�� = 
(CMEAS_CYTIME+1) * Clock_Period��Clock_period=20��s / 1��s / 0.25��s 
���� CYCLKSEL=0/2/3 */
#define REG4_CMEAS_CYTIME_CONFIG            2           /** ��Χ��0~1023 */
#define REG4_CMEAS_CYTIME                   (REG4_CMEAS_CYTIME_CONFIG << 8)

/** ���������¶Ȳ����� cycle time. ѡ��33nF,1000kOhm ����������Ϊ 0 */
#define REG4_TMEAS_CYTIME_140US             0x00        /** 140 ��s */
#define REG4_TMEAS_CYTIME_280US             (0x01 << 4) /** 280 ��s */

/** ѡ�������¶Ȳ��������崥���ܽ� */
#define REG4_TMEAS_STARTPIN_PG0             0x00        /** PG0 */
#define REG4_TMEAS_STARTPIN_PG1             (0x01 << 2) /** PG1 */
#define REG4_TMEAS_STARTPIN_PG2             (0x02 << 2) /** PG2 */
#define REG4_TMEAS_STARTPIN_PG3             (0x03 << 2) /** PG3 */

/** ѡ���¶Ȳ����Ĵ���Դ */
#define REG4_TMEAS_TRIG_SEL_OFF             0x00        /** �ر� / �����봥�� */
#define REG4_TMEAS_TRIG_SEL_CMEAS           0x01        /** CMEAS-���� (ͨ�� C-����,�Ƽ�) */


/** REG5���¶Ȳ������� */
/** RDC������ٴβ�������ƽ�� */
#define REG5_T_AVRG_ONCE                    0x00        /** ��ƽ�� */
#define REG5_T_AVRG_4_TIMES                 (0x01 << 22)/** 4��ƽ�� */
#define REG5_T_AVRG_8_TIMES                 (0x02 << 22)/** 8��ƽ�� */
#define REG5_T_AVRG_16_TIMES                (0x03 << 22)/** 16��ƽ�� */

/** ��ȷ�������CDC�Ĳ���RDC�������ֵĴ�����
����Ϊ0��Ϊ1�Ρ�������ʪ�ȼ�������Ϊ0����Ϊ�¶Ȳ�����ʪ��
�����Ĵ���Ӧ����һ����/ ��ѹ������������ 100...1k ���࣬
��Ϊѹ���ı仯һ��ǳ��죬���¶�Ҫ��ܶ�*/
#define REG5_TMEAS_TRIG_PREDIV_CONFIG       0           /** ��Χ��0~4194303 */
#define REG5_TMEAS_TRIG_PREDIV              REG5_TMEAS_TRIG_PREDIV_CONFIG


/** REG6���¶Ȳ������� */
/** ѡ��ٲ����Ĵ���.����/�ֱ��ʵ�����Ϊ1��ʱ��ȽϺã����ٶȽ���Ƚ��� */
#define REG6_TMEAS_FAKE_2_TIMES             0x00        /** 2 �μٲ��� */
#define REG6_TMEAS_FAKE_8_TIMES             (0x01 << 15)/** 8 �μٲ��� */

/** оƬ�ڲ�������һ�����������һ���ڲ��ο�����.���⻹�ṩ�������ⲿ�¶ȴ������Ͳ�
������Ķ˿�.�ܶ���֮,���оƬ֧��11�����.�����������������Ĵ��б�׼�̼���оƬ,
�������6����Ͽ���: */
/** Res11 = R(�ڲ�������)/R(�ڲ��ྦྷ�����) */
#define REG6_TMEAS_7BITS_AL_SI              (0x43 << 8)        
/** Res10 = R(�ⲿ����)/R(�ڲ��ྦྷ�����)��Res11 = R(�ڲ�������)/R(�ڲ��ྦྷ�����) */
#define REG6_TMEAS_7BITS_EX_SI_AL_SI        (0x4B << 8)        
/** Res10 = R(�ⲿ����)/R(�ڲ��ྦྷ�����) */
#define REG6_TMEAS_7BITS_EX_SI              (0x49 << 8)        
/** Res11 = R(�ڲ�������)/R(�ⲿ�ο�����) */
#define REG6_TMEAS_7BITS_AL_REF             (0x06 << 8)        
/** Res10 = R(�ⲿ����)/R(�ⲿ�ο�����)��Res11 = R(�ڲ�������)/R(�ⲿ�ο�����) */
#define REG6_TMEAS_7BITS_EX_REF_AL_REF      (0x0E << 8)        
/** Res10 = R(�ⲿ����)/R(�ⲿ�ο�����) */
#define REG6_TMEAS_7BITS_EX_REF             (0x0C << 8)       


/** REG8��DSP���� */
/** Ϊ��Ƭ��ѡ�����洢�ռ� */
#define REG8_DSP_OTP_SEL                    0x00        /** �洢��OTP�� */
#define REG8_DSP_SRAM_SEL                   (0x01 << 23)/** �洢��SRAM�� */

/** ��Ƭ���Ŀ�ʼ����; ��Ƭ����ʱ�ӿ�ʼ����,��������ת����ַ0Ȼ��������
ʼ����. ���̼���ɺ�,DSP����ֹͣ�������ʱ�ӣ� */
#define REG8_DSP_NOT_START                  0x00        /** PCap��DSP������ */
#define REG8_DSP_START                      (0x01 << 22)/** PCap��DSP���� */

/** �������ȷ���˵�Ƭ���ڴ������ʱ�ɱ����� */
#define REG8_DSPSTARTONOVL_DEFAULT          0x00        /** ��ҪĬ������ */
#define REG8_DSPSTARTONOVL                  (0x01 << 21)/** ��Ƭ���ڴ������ʱ�ɱ����� */

/** ������ñ�֤�˴��������¶Ȳ�����ɺ��������ݲ�ͬ�̼����ڹ̼� 03.01.
xx �����¶Ȳ������� CDC-���֮����е� */
#define REG8_DSP_STARTONTEMP_DEFAULT        0x00        /** Ĭ������, �ڹ̼�03.01.xx���Ǳ���� */
#define REG8_DSP_STARTONTEMP                (0x01 << 20)/** ���������¶Ȳ�����ɺ��� */

/** �������津���ĸ����ܽ� */
#define REG8_DSP_STARTPIN_PG0               (0x01 << 16)/** PG0 */
#define REG8_DSP_STARTPIN_PG1               (0x02 << 16)/** PG1 */
#define REG8_DSP_STARTPIN_PG2               (0x04 << 16)/** PG2 */
#define REG8_DSP_STARTPIN_PG3               (0x08 << 16)/** PG3 */

/** �������津���ĸ����ܽ� */
#define REG8_DSP_FF_IN_PG0                  (0x01 << 12)/** PG0 */
#define REG8_DSP_FF_IN_PG1                  (0x02 << 12)/** PG1 */
#define REG8_DSP_FF_IN_PG2                  (0x04 << 12)/** PG2 */
#define REG8_DSP_FF_IN_PG3                  (0x08 << 12)/** PG3 */

/** ��Ƭ����,���������ʦ�����ж��� */
#define REG8_DSP_WATCHDOG_LENGTH_CONFIG     0           /** ��ʱ��ʱ��, ��Χ��0~3 */
#define REG8_DSP_WATCHDOG_LENGTH            (REG8_DSP_WATCHDOG_LENGTH_CONFIG << 10)

/** ������ PG0 �� PG1���ϵĿ������˲� */
#define REG8_DSP_MOFLO_PG0_EN               (0x01 << 8) /**  PG0���� */
#define REG8_DSP_MOFLO_PG1_EN               (0x02 << 8) /**  PG1���� */

/** ���� DSP �ٶ� */
#define REG8_DSP_SPEED_STANDARD             (0x01 << 4)/** ��׼ (����) */
#define REG8_DSP_SPEED_LOW_POWER            (0x03 << 4)/** �͹���(����) */

/** ��QFN24��װ��������,��Ϊ���ʱ��û��INTN�жϹܽ�.�����ж��ź�ͨ��
PG2�˿���� */
#define REG8_INT2PG2_INTN_ONLY              0x00        /** ֻ��INTN�ܽ� */
#define REG8_INT2PG2_ENABLE                 (0x01 << 2) /** INTN��PG2�ܽŶ��� */

/** ���������һ���ڶ˿� PG0 �� PG1�������I2Cͨ��ģʽ�£�������Ҫ��ѡ��
Ϊ����� PG2 �� PG3 */
#define REG8_PG0_X_G2_PG0                   0x00        /** ѡ��PG0 */
#define REG8_PG0_X_G2_PG2                   0x01        /** ѡ��PG2 */
#define REG8_PG1_X_G3_PG1                   0x00        /** ѡ��PG1 */
#define REG8_PG1_X_G3_PG3                   (0x01 << 2) /** ѡ��PG3 */


/** REG9��GPIO���� */
/** ������л�Ϊ����(PG3/λ23 �� PG0/λ20) */    
#define REG9_PG_DIR_PG0_IN                  (0x01 << 20)/** PG0���� */
#define REG9_PG_DIR_PG1_IN                  (0x02 << 20)/** PG1���� */
#define REG9_PG_DIR_PG2_IN                  (0x04 << 20)/** PG2���� */
#define REG9_PG_DIR_PG3_IN                  (0x08 << 20)/** PG3���� */

/** �� PG0 �� PG3 ���Ͽ�����������; Ӧ�û�е�л����� */
#define REG9_PG_PULL_PG0_UP                 (0x01 << 16)/** PG0���� */
#define REG9_PG_PULL_PG1_UP                 (0x02 << 16)/** PG1���� */
#define REG9_PG_PULL_PG2_UP                 (0x04 << 16)/** PG2���� */
#define REG9_PG_PULL_PG3_UP                 (0x08 << 16)/** PG3���� */

/** ���������ܶȻ��������ȵ���������� PWM0/PDM0 ��������ڶ˿� PG0
���� PG2. PWM1/PDM1 ��������ڶ˿�PG1 ���� PG3 */
#define REG9_PI_PWM0_EN                     (0x01 << 12)/** PWM0 ���� */
#define REG9_PI_PDM0_EN                     (0x02 << 12)/** PDM0 ���� */
#define REG9_PI_PWM1_EN                     (0x04 << 12)/** PWM1 ���� */
#define REG9_PI_PDM1_EN                     (0x08 << 12)/** PDM1 ���� */

/** ��������ӿڵĻ���Ƶ��,�����ڲ���Ƶ��(OLF)�����ⲿ��Ƶ���� (OHF) */
#define REG9_PCLK_SEL_2OHF                  0x04        /** OHF * 2 */
#define REG9_PCLK_SEL_OHF                   0x05        /** OHF     */
#define REG9_PCLK_SEL_OHF2                  0x06        /** OHF / 2 */
#define REG9_PCLK_SEL_OHF4                  0x07        /** OHF / 4 */
#define REG9_PCLK_SEL_2OLF                  0x08        /** OLF * 2 */
#define REG9_PCLK_SEL_OLF                   0x09        /** OLF     */
#define REG9_PCLK_SEL_OLF2                  0x0A        /** OLF / 2 */
#define REG9_PCLK_SEL_OLF4                  0x0B        /** OLF / 4 */

#define REG9_PI0_CLK_SEL_2OHF               (REG9_PCLK_SEL_2OHF << 4)
#define REG9_PI0_CLK_SEL_OHF                (REG9_PCLK_SEL_OHF << 4)
#define REG9_PI0_CLK_SEL_OHF2               (REG9_PCLK_SEL_OHF2 << 4)
#define REG9_PI0_CLK_SEL_OHF4               (REG9_PCLK_SEL_OHF4 << 4)
#define REG9_PI0_CLK_SEL_2OLF               (REG9_PCLK_SEL_2OLF << 4)
#define REG9_PI0_CLK_SEL_OLF                (REG9_PCLK_SEL_OLF << 4)
#define REG9_PI0_CLK_SEL_OLF2               (REG9_PCLK_SEL_OLF2 << 4)
#define REG9_PI0_CLK_SEL_OLF4               (REG9_PCLK_SEL_OLF4 << 4)

#define REG9_PI1_CLK_SEL_2OHF               (REG9_PCLK_SEL_2OHF << 8)
#define REG9_PI1_CLK_SEL_OHF                (REG9_PCLK_SEL_OHF << 8)
#define REG9_PI1_CLK_SEL_OHF2               (REG9_PCLK_SEL_OHF2 << 8)
#define REG9_PI1_CLK_SEL_OHF4               (REG9_PCLK_SEL_OHF4 << 8)
#define REG9_PI1_CLK_SEL_2OLF               (REG9_PCLK_SEL_2OLF << 8)
#define REG9_PI1_CLK_SEL_OLF                (REG9_PCLK_SEL_OLF << 8)
#define REG9_PI1_CLK_SEL_OLF2               (REG9_PCLK_SEL_OLF2 << 8)
#define REG9_PI1_CLK_SEL_OLF4               (REG9_PCLK_SEL_OLF4 << 8)

/** ��������ľ��� */
#define REG9_PRES_7BITS                     0x00        /** 7 bit */
#define REG9_PRES_8BITS                     0x01        /** 8 bit */
#define REG9_PRES_9BITS                     0x02        /** 9 bit */
#define REG9_PRES_10BITS                    0x03        /** 10 bit */

#define REG9_PI0_RES_7BITS                  REG9_PRES_7BITS
#define REG9_PI0_RES_8BITS                  REG9_PRES_8BITS
#define REG9_PI0_RES_9BITS                  REG9_PRES_9BITS
#define REG9_PI0_RES_10BITS                 REG9_PRES_10BITS

#define REG9_PI1_RES_7BITS                  (REG9_PRES_7BITS << 2)
#define REG9_PI1_RES_8BITS                  (REG9_PRES_8BITS << 2)
#define REG9_PI1_RES_9BITS                  (REG9_PRES_9BITS << 2)
#define REG9_PI1_RES_10BITS                 (REG9_PRES_10BITS << 2)


/** REG10������1.8 V ���ĵ�ѹ���������͹��ĵ�����Ҳ���Ի�úõĲ�����
��,���������RDC�Ĳ����ٶȺ�ʱ���й�.��ο���¼���е����� */
#define REG10_PV_CORE_CTL_STANDARD          0x47        /** ��׼ */
#define REG10_V_CORE_CTL_LOW_POWER          0x87        /** �͹��� */


/** Param2��ѡ��PDM/PWM������ź�Դ */
#define PARAM2_SELECT_C1_C0                 0x01        /** C1/C0 */
#define PARAM2_SELECT_R0_RREF               0x08        /** R0/R�ο� */
#define PARAM2_SELECT_R2_RREF               0x09        /** R2/R�ο� */

/** ��һģʽ */
#define PARAM2_SELECT_C2_C0                 0x02        /** C2/C0 */
#define PARAM2_SELECT_C3_C0                 0x03        /** C3/C0 */
#define PARAM2_SELECT_C4_C0                 0x04        /** C4/C0 */
#define PARAM2_SELECT_C5_C0                 0x05        /** C5/C0 */
#define PARAM2_SELECT_C6_C0                 0x06        /** C6/C0 */
#define PARAM2_SELECT_C7_C0                 0x07        /** C7/C0 */

/** ���ģʽ */
#define PARAM2_SELECT_C3_C2                 0x02        /** C3/C2 */
#define PARAM2_SELECT_C5_C4                 0x03        /** C5/C4 */
#define PARAM2_SELECT_C7_C6                 0x04        /** C7/C6 */

/** pulse0ѡ��PDM/PWM������ź�Դ */
#define PARAM2_PULSE0_SELECT_C1_C0          PARAM2_SELECT_C1_C0     /** C1/C0 */
#define PARAM2_PULSE0_SELECT_R0_RREF        PARAM2_SELECT_R0_RREF   /** R0/R�ο� */
#define PARAM2_PULSE0_SELECT_R2_RREF        PARAM2_SELECT_R2_RREF   /** R2/R�ο� */

/** pulse0��һģʽ */
#define PARAM2_PULSE0_SELECT_C2_C0          PARAM2_SELECT_C2_C0     /** C2/C0 */
#define PARAM2_PULSE0_SELECT_C3_C0          PARAM2_SELECT_C3_C0     /** C3/C0 */
#define PARAM2_PULSE0_SELECT_C4_C0          PARAM2_SELECT_C4_C0     /** C4/C0 */
#define PARAM2_PULSE0_SELECT_C5_C0          PARAM2_SELECT_C5_C0     /** C5/C0 */
#define PARAM2_PULSE0_SELECT_C6_C0          PARAM2_SELECT_C6_C0     /** C6/C0 */
#define PARAM2_PULSE0_SELECT_C7_C0          PARAM2_SELECT_C7_C0     /** C7/C0 */

/** pulse0���ģʽ */
#define PARAM2_PULSE0_SELECT_C3_C2          PARAM2_SELECT_C3_C2     /** C3/C2 */
#define PARAM2_PULSE0_SELECT_C5_C4          PARAM2_SELECT_C5_C4     /** C5/C4 */
#define PARAM2_PULSE0_SELECT_C7_C6          PARAM2_SELECT_C7_C6     /** C7/C6 */

/** pulse1ѡ��PDM/PWM������ź�Դ */
#define PARAM2_PULSE1_SELECT_C1_C0          (PARAM2_SELECT_C1_C0 << 4)  /** C1/C0 */
#define PARAM2_PULSE1_SELECT_R0_RREF        (PARAM2_SELECT_R0_RREF << 4)/** R0/R�ο� */
#define PARAM2_PULSE1_SELECT_R2_RREF        (PARAM2_SELECT_R2_RREF << 4)/** R2/R�ο� */

/** pulse1��һģʽ */
#define PARAM2_PULSE1_SELECT_C2_C0          (PARAM2_SELECT_C2_C0 << 4)  /** C2/C0 */
#define PARAM2_PULSE1_SELECT_C3_C0          (PARAM2_SELECT_C3_C0 << 4)  /** C3/C0 */
#define PARAM2_PULSE1_SELECT_C4_C0          (PARAM2_SELECT_C4_C0 << 4)  /** C4/C0 */
#define PARAM2_PULSE1_SELECT_C5_C0          (PARAM2_SELECT_C5_C0 << 4)  /** C5/C0 */
#define PARAM2_PULSE1_SELECT_C6_C0          (PARAM2_SELECT_C6_C0 << 4)  /** C6/C0 */
#define PARAM2_PULSE1_SELECT_C7_C0          (PARAM2_SELECT_C7_C0 << 4)  /** C7/C0 */

/** pulse1���ģʽ */
#define PARAM2_PULSE1_SELECT_C3_C2          (PARAM2_SELECT_C3_C2 << 4)  /** C3/C2 */
#define PARAM2_PULSE1_SELECT_C5_C4          (PARAM2_SELECT_C5_C4 << 4)  /** C5/C4 */
#define PARAM2_PULSE1_SELECT_C7_C6          (PARAM2_SELECT_C7_C6 << 4)  /** C7/C6 */

/** Param3�������������0��б�ʣ��з��Ź̶�����,19λ������4λС�� */
#define PARAM3_PULSE0_SLOPE_CONFIG          0           /** -524,288 �� +524,288 �� 0.0625Ϊ��С��λ */
#define PARAM3_PULSE0_SLOPE                 PARAM3_PULSE0_SLOPE_CONFIG 


/** Param4�������������0������з��Ź̶�����, 22 ����, 1 λС��.
��һλС��������ѧ�������ֵ, ����������� */
#define PARAM4_PULSE0_OFFSET_CONFIG         0           /** -4,194,304�� + 4,194,303.5 �� 0.5Ϊ��С��λ */
#define PARAM4_PULSE0_OFFSET                PARAM4_PULSE0_OFFSET_CONFIG 


/** Param5�������������1��б�ʣ��з��Ź̶�����,19λ������4λС�� */
#define PARAM5_PULSE1_SLOPE_CONFIG         0           /** -524,288 �� +524,288 �� 0.0625Ϊ��С��λ */
#define PARAM5_PULSE1_SLOPE                 PARAM5_PULSE1_SLOPE_CONFIG 


/** Param6�������������1������з��Ź̶�����, 22 ����, 1 λС��.
��һλС��������ѧ�������ֵ, ����������� */
#define PARAM6_PULSE1_OFFSET_CONFIG         0           /** -4,194,304�� + 4,194,303.5 �� 0.5Ϊ��С��λ */
#define PARAM6_PULSE1_OFFSET                PARAM6_PULSE1_OFFSET_CONFIG 


/** Param7��ѡ��һ���߲�ִ����� */
#define PARAM7_SEL_SINGLE                   0x00        /** ��һ */
#define PARAM7_SEL_DIFFERENTIAL             0x000001    /** ��� */


/** Param8�������ڲ������ĳ˷�ϵ�� �޷��Ź̶�������,3λ����,21λС�� */
#define PARAM8_GAIN_CORR_CONFIG             0x200000    /** �����÷�Χ: 0 �� 7.9999 */
#define PARAM8_GAIN_CORR                    PARAM8_GAIN_CORR_CONFIG

/** REG20���豸�����Ĵ��� */
/** ���������豸,��֤�����ù����о�ֹ,ǿ��DSP���ھ�ֹ״̬ */
#define REG20_RUN_DISABLE                   0x00        
/** ���� ����ģʽ */
#define REG20_RUN_ENABLE                    0x000001    



/******************************** ����Ϊ21�����üĴ��������� ***************************************/
/** REG0�Ĵ���ֵ������0x400000��0x20000�ǹ̶�ֵ */
#define REG0_OTP_CONFIG_VALUE               0x400000 \
                                            /** SRAM��OTP�ıȽϻ��� */             \
                                            /**  REG0_MEMCOMP_OFF        �ر�      \
                                                REG0_MEMCOMP_5BYTE      5 Byte      \
                                                REG0_MEMCOMP_33BYTE     33 Byte     \
                                                REG0_MEMCOMP_257BYTE    257 Byte  */\
                                            | REG0_MEMCOMP_OFF  \
                                            | 0x20000 \
                                            /** OTP-�ڲ���������޸����� */                           \
                                            /**  REG0_ECC_MODE_DISABLE   �ر�(Singleģʽ)������4032Byte \
                                                REG0_ECC_MODE_DOUBLE    Doubleģʽ������4032Byte     \
                                                REG0_ECC_MODE_QUAD      Quadģʽ������1984Byte */ \
                                            | REG0_ECC_MODE_DISABLE \
                                            /** �Զ���OTP������ */                                                 \
                                            /**  REG0_AUTOBOOT_ENABLE    ��оƬ����ģʽ��оƬ�����Զ���OTP���������� \
                                                REG0_AUTOBOOT_DISABLE   �ӻ�ģʽ��оƬͨ��ͨѶ�ӿڱ���λ���� */   \
                                            | REG0_AUTOBOOT_DISABLE \
                                            /** �洢������ */                                                         \
                                            /**  REG0_MEM_LOCK_ENABLE    ������ֹ�洢���ݶ�������ֹ�̼�������Ϣ������  \
                                                REG0_MEM_LOCK_DISABLE   ��������ֹ�洢���ݶ��� */                      \
                                            | REG0_MEM_LOCK_DISABLE 
                                            
/** REG1�Ĵ���ֵ������0x201000�ǹ̶�ֵ */
#define REG1_IN_OSC_CONFIG_VALUE            0x201000 \
                                            /** REG1�������ڲ�����Ƶ�� */        \
                                            /**  REG1_OLF_TUNE_10KHZ     10kHz       \
                                                REG1_OLF_TUNE_50KHZ     50kHz       \
                                                REG1_OLF_TUNE_100KHZ    100kHz      \
                                                REG1_OLF_TUNE_200KHZ    200kHz */   \
                                            | REG1_OLF_TUNE_50KHZ

/** REG2�Ĵ���ֵ������0x0B�ǹ̶�ֵ */
#define REG2_CAP_MEASURE_VALUE              /** CDC�˿�ʹ�� */                       \
                                            /**  REG2_CMEAS_PC0_EN   ʹ��PC0�˿�     \ 
                                                REG2_CMEAS_PC1_EN   ʹ��PC1�˿�     \
                                                REG2_CMEAS_PC2_EN   ʹ��PC2�˿�     \
                                                REG2_CMEAS_PC3_EN   ʹ��PC3�˿�     \
                                                REG2_CMEAS_PC4_EN   ʹ��PC4�˿�     \
                                                REG2_CMEAS_PC5_EN   ʹ��PC5�˿�     \
                                                REG2_CMEAS_PC6_EN   ʹ��PC6�˿�     \
                                                REG2_CMEAS_PC7_EN   ʹ��PC7�˿� */  \
                                            REG2_CMEAS_PC0_EN \
                                            | REG2_CMEAS_PC1_EN \
                                            /** ���������ӷ�ʽ */                               \
                                            /**  REG2_CMEAS_GROUND_SINGLE            ���˽ӵ�    \
                                                REG2_CMEAS_GROUND_DIFFERENTIAL      ��ֽӵ�    \
                                                REG2_CMEAS_FLOATING_SINGLE          ���˸���    \
                                                REG2_CMEAS_FLOATING_DIFFERENTIAL    ��ָ��� */ \
                                            | REG2_CMEAS_GROUND_SINGLE \
                                            /** ��ֵ��ݾ���ԳƳ�ŵ��ѡ�񣬷�ֹ��еӦ�� */ \
                                            /**  REG2_CMEAS_DUMMY_DISABLE    �ر�              \
                                                REG2_CMEAS_DUMMY_ENABLE     ʹ�� */           \
                                            | REG2_CMEAS_DUMMY_DISABLE \
                                            /** �ⲿ�������ݵĲ������� */           \
                                            /**  REG2_COMPENS_EXT_DISABLE    �ر�      \
                                                REG2_COMPENS_EXT_ENABLE     ʹ�� */   \
                                            | REG2_COMPENS_EXT_ENABLE \
                                            /** �ڲ��������ݵĲ������� */           \
                                            /**  REG2_COMPENS_INT_DISABLE    �ر�      \
                                                REG2_COMPENS_INT_ENABLE     ʹ�� */   \
                                            | REG2_COMPENS_INT_DISABLE \
                                            /** ѡ���ڲ��ŵ���� */              \
                                            /**  REG2_RDCHG_INT_180K     180Kŷ   \
                                                REG2_RDCHG_INT_90K      90Kŷ    \
                                                REG2_RDCHG_INT_30K      30Kŷ    \
                                                REG2_RDCHG_INT_10K      10Kŷ */ \
                                            | REG2_RDCHG_INT_90K \
                                            | 0x0B
                                            
/** REG3�Ĵ���ֵ */
#define REG3_CAP_MEASURE_VALUE              /** CDCѭ����ʱ�ӻ�׼ */                                    \
                                            /**  REG3_CY_CLK_20US    20 ��s (������Ƶʱ�ӵ�Ӧ��)           \
                                                REG3_CY_CLK_1US     1 ��s (����Ӧ���ⲿ 4 MHz��)       \
                                                REG3_CY_CLK_0_25US  0.25 ��s (����Ӧ���ⲿ 4 MHz��) */ \
                                            REG3_CY_CLK_20US \
                                            /** ��ʱ��-����ģʽ���ô��������� (�Ĵ���4, TMEAS_TRIG_SEL = 2) */                \
                                            /**  REG3_SEQ_TIME_CONFIG    ���ô�������������Χ��1~24��0 = �ر�                  \
                                                REG3_SEQ_TIME           ����������,���ľ���ֵ��REG3_SEQ_TIME_CONFIG������ */  \
                                            | REG3_SEQ_TIME | \
                                            /** �趨CDCÿ�β����ĺ�ļٲ����������ɴ�0��4ѡ�� */ \
                                            /**  REG3_CMEAS_FAKE_NONE        �޼ٲ���                \
                                                REG3_CMEAS_FAKE_1_TIME      1�μٲ���               \
                                                REG3_CMEAS_FAKE_2_TIMES     2�μٲ���               \
                                                REG3_CMEAS_FAKE_4_TIMES     4�μٲ��� */            \
                                            REG3_CMEAS_FAKE_1_TIME | \
                                            /** CDC �������ڲ�ƽ���Ĵ���������Ϊ0��Ч��Ĭ��Ϊ1 */                                \
                                            /**  REG3_C_AVRG_CONFIG      �����ڲ�ƽ����������Χ��0~4095                          \
                                                REG3_C_AVRG             �ڲ�ƽ������,���ľ���ֵ��REG3_C_AVRG_CONFIG������  */  \
                                            REG3_C_AVRG
                                            
/** REG4�Ĵ���ֵ */
#define REG4_CAP_TEMP_MEASURE_VALUE         /** ѡ����Ӧ��������ݲ��������崥���ܽ� */    \
                                            /**  REG4_CMEAS_STARTPIN_PG0     PG0             \
                                                REG4_CMEAS_STARTPIN_PG1     PG1             \
                                                REG4_CMEAS_STARTPIN_PG2     PG2             \
                                                REG4_CMEAS_STARTPIN_PG3     PG3 */          \
                                            REG4_CMEAS_STARTPIN_PG0 | \
                                            /** ѡ����ݲ����Ĵ���Դ */                            \
                                            /**  REG4_CMEAS_TRIG_SEL_SOFT        ���������       \
                                                REG4_CMEAS_TRIG_SEL_CONTINUE    ����ģʽ        \
                                                REG4_CMEAS_TRIG_SEL_CLOCK       ʱ�Ӵ���ģʽ      \
                                                REG4_CMEAS_TRIG_SEL_PULSE       ���崥��ģʽ */   \
                                            REG4_CMEAS_TRIG_SEL_SOFT | \
                                            /**  �������ڵ��ݲ����� cycle time��CDC ����ʱ�� =                                 \
                                                (CMEAS_CYTIME+1) * Clock_Period��Clock_period=20��s / 1��s / 0.25��s            \
                                                ���� CYCLKSEL=0/2/3 */                                                        \
                                            /**  REG4_CMEAS_CYTIME_CONFIG        ��Χ��0~1023                                   \
                                                REG4_CMEAS_CYTIME       ����ʱ��,���ľ���ֵ��REG4_CMEAS_CYTIME_CONFIG������  */  \
                                            REG4_CMEAS_CYTIME \
                                            /** ���������¶Ȳ����� cycle time. ѡ��33nF,1000kOhm ����������Ϊ 0 */    \
                                            /**  REG4_TMEAS_CYTIME_140US     140 ��s                                      \
                                                REG4_TMEAS_CYTIME_280US     280 ��s */                                   \
                                            | REG4_TMEAS_CYTIME_280US \
                                            /** ѡ�������¶Ȳ��������崥���ܽ� */   \
                                            /**  REG4_TMEAS_STARTPIN_PG0     PG0     \
                                                REG4_TMEAS_STARTPIN_PG1     PG1     \
                                                REG4_TMEAS_STARTPIN_PG2     PG2     \
                                                REG4_TMEAS_STARTPIN_PG3     PG3 */  \
                                            | REG4_TMEAS_STARTPIN_PG0 \
                                            /** ѡ���¶Ȳ����Ĵ���Դ */                 \
                                            /**  REG4_TMEAS_TRIG_SEL_OFF     �ر� / �����봥��                  \
                                                REG4_TMEAS_TRIG_SEL_CMEAS   CMEAS-���� (ͨ�� C-����,�Ƽ�) */    \
                                            | REG4_TMEAS_TRIG_SEL_OFF
                                            
/** REG5�Ĵ���ֵ */
#define REG5_TEMP_MEASURE_VALUE             /** RDC������ٴβ�������ƽ�� */            \
                                            /**  REG5_T_AVRG_ONCE        ��ƽ��         \
                                                REG5_T_AVRG_4_TIMES     4��ƽ��        \
                                                REG5_T_AVRG_8_TIMES     8��ƽ��        \
                                                REG5_T_AVRG_16_TIMES    16��ƽ�� */    \
                                            REG5_T_AVRG_8_TIMES | \
                                            /**  ��ȷ�������CDC�Ĳ���RDC�������ֵĴ�����                                     \
                                                ����Ϊ0��Ϊ1�Ρ�������ʪ�ȼ�������Ϊ0����Ϊ�¶Ȳ�����ʪ��                     \ 
                                                �����Ĵ���Ӧ����һ����/ ��ѹ������������ 100...1k ���࣬                      \
                                                ��Ϊѹ���ı仯һ��ǳ��죬���¶�Ҫ��ܶ�*/                                      \
                                            /**  REG5_TMEAS_TRIG_PREDIV_CONFIG   ��Χ��0~4194303                                \
                                                REG5_TMEAS_TRIG_PREDIV          �����CDC�Ĳ���RDC�������ֵĴ���,               \ 
                                                                            ���ľ���ֵ��REG5_TMEAS_TRIG_PREDIV_CONFIG������ */   \
                                            REG5_TMEAS_TRIG_PREDIV
                                            
/** REG6�Ĵ���ֵ������0x40�ǹ̶�ֵ */
#define REG6_TEMP_MEASURE_VALUE             /** оƬ�ڲ�������һ�����������һ���ڲ��ο�����.���⻹�ṩ�������ⲿ�¶ȴ������Ͳ� \
                                            ������Ķ˿�.�ܶ���֮,���оƬ֧��11�����.�����������������Ĵ��б�׼�̼���оƬ,     \
                                            �������6����Ͽ���: */                                                              \
                                            /**  Res11 = R(�ڲ�������)/R(�ڲ��ྦྷ�����)                                         \
                                                    REG6_TMEAS_7BITS_AL_SI                                                      \
                                                Res10 = R(�ⲿ����)/R(�ڲ��ྦྷ�����)��Res11 = R(�ڲ�������)/R(�ڲ��ྦྷ�����)  \
                                                    REG6_TMEAS_7BITS_EX_SI_AL_SI                                                \
                                                Res10 = R(�ⲿ����)/R(�ڲ��ྦྷ�����)                                          \
                                                    REG6_TMEAS_7BITS_EX_SI                                                      \
                                                Res11 = R(�ڲ�������)/R(�ⲿ�ο�����)                                          \
                                                    REG6_TMEAS_7BITS_AL_REF                                                     \
                                                Res10 = R(�ⲿ����)/R(�ⲿ�ο�����)��Res11 = R(�ڲ�������)/R(�ⲿ�ο�����)    \
                                                    REG6_TMEAS_7BITS_EX_REF_AL_REF                                              \
                                                Res10 = R(�ⲿ����)/R(�ⲿ�ο�����)                                           \
                                                    REG6_TMEAS_7BITS_EX_REF     */                                              \
                                            REG6_TMEAS_FAKE_2_TIMES \
                                            | 0x40
                                            
/** REG7�Ĵ���ֵ���̶�0x1F0000 */
#define REG7_CONFIG_VALUE                   0x1F0000

/** REG8�Ĵ���ֵ */
#define REG8_DSP_CONFIG_VALUE               /** Ϊ��Ƭ��ѡ�����洢�ռ� */              \
                                            /**  REG8_DSP_OTP_SEL        �洢��OTP��     \
                                                REG8_DSP_SRAM_SEL       �洢��SRAM�� */ \
                                            REG8_DSP_SRAM_SEL \
                                            /** ��Ƭ���Ŀ�ʼ����; ��Ƭ����ʱ�ӿ�ʼ����,��������ת����ַ0Ȼ�������� \
                                            ʼ����. ���̼���ɺ�,DSP����ֹͣ�������ʱ�ӣ� */                       \
                                            /**  REG8_DSP_NOT_START      PCap��DSP������                             \
                                                REG8_DSP_START          PCap��DSP���� */                           \
                                            | REG8_DSP_NOT_START \
                                            /** �������ȷ���˵�Ƭ���ڴ������ʱ�ɱ����� */                      \
                                            /**  REG8_DSPSTARTONOVL_DEFAULT      Ĭ������                        \
                                                REG8_DSPSTARTONOVL              ��Ƭ���ڴ������ʱ�ɱ����� */    \
                                            | REG8_DSPSTARTONOVL_DEFAULT \
                                            /** ������ñ�֤�˴��������¶Ȳ�����ɺ��������ݲ�ͬ�̼����ڹ̼� 03.01.   \
                                            xx �����¶Ȳ������� CDC-���֮����е� */                                  \
                                            /**  REG8_DSP_STARTONTEMP_DEFAULT    Ĭ������, �ڹ̼�03.01.xx���Ǳ����      \
                                                REG8_DSP_STARTONTEMP            ���������¶Ȳ�����ɺ��� */            \
                                            | REG8_DSP_STARTONTEMP_DEFAULT \
                                            /** �������津���ĸ����ܽ� */           \
                                            /**  REG8_DSP_STARTPIN_PG0       PG0     \
                                                REG8_DSP_STARTPIN_PG1       PG1     \
                                                REG8_DSP_STARTPIN_PG2       PG2     \
                                                REG8_DSP_STARTPIN_PG3       PG3 */  \
                                            | 0x00  \
                                            /** �������津���ĸ����ܽ� */       \
                                            /**  REG8_DSP_FF_IN_PG0      PG0     \
                                                REG8_DSP_FF_IN_PG1      PG1     \
                                                REG8_DSP_FF_IN_PG2      PG2     \
                                                REG8_DSP_FF_IN_PG3      PG3 */  \
                                            | 0x00  \
                                            /** ��Ƭ����,���������ʦ�����ж��� */                  \
                                            /**  REG8_DSP_WATCHDOG_LENGTH_CONFIG     ��ʱ��ʱ������ \
                                                REG8_DSP_WATCHDOG_LENGTH            ��ʱ��ʱ����  \
                                                                            ���ľ���ֵ��REG8_DSP_WATCHDOG_LENGTH_CONFIG������ */ \
                                            | REG8_DSP_WATCHDOG_LENGTH \
                                            /** ������ PG0 �� PG1���ϵĿ������˲� */     \
                                            /**  REG8_DSP_MOFLO_PG0_EN       PG0����       \
                                                REG8_DSP_MOFLO_PG1_EN       PG1���� */    \
                                            | REG8_DSP_MOFLO_PG0_EN \
                                            | REG8_DSP_MOFLO_PG1_EN \
                                            /** ���� DSP �ٶ� */                             \
                                            /**  REG8_DSP_SPEED_STANDARD     ��׼ (����)     \
                                                REG8_DSP_SPEED_LOW_POWER    �͹���(����) */  \
                                            | REG8_DSP_SPEED_STANDARD   \
                                            /** ��QFN24��װ��������,��Ϊ���ʱ��û��INTN�жϹܽ�.�����ж��ź�ͨ��  \
                                            PG2�˿���� */                                                          \
                                            /**  REG8_INT2PG2_INTN_ONLY      ֻ��INTN�ܽ�           \
                                                REG8_INT2PG2_ENABLE         INTN��PG2�ܽŶ��� */    \
                                            | 0x00  \
                                            /** ���������һ���ڶ˿� PG0 �� PG1�������I2Cͨ��ģʽ�£�������Ҫ��ѡ�� \
                                            Ϊ����� PG2 �� PG3 */                   \
                                            /**  REG8_PG0_X_G2_PG0       ѡ��PG0       \
                                                REG8_PG0_X_G2_PG2       ѡ��PG2       \
                                                REG8_PG1_X_G3_PG1       ѡ��PG1       \
                                                REG8_PG1_X_G3_PG3       ѡ��PG3 */    \
                                            | 0x00

/** REG9�Ĵ���ֵ */
#define REG9_GPIO_CONFIG_VALUE              /** ������л�Ϊ����(PG3/λ23 �� PG0/λ20) */    \
                                            /**  REG9_PG_DIR_PG0_IN      PG0����               \
                                                REG9_PG_DIR_PG1_IN      PG1����               \
                                                REG9_PG_DIR_PG2_IN      PG2����               \
                                                REG9_PG_DIR_PG3_IN      PG3���� */            \
                                            REG9_PG_DIR_PG0_IN | \
                                            REG9_PG_DIR_PG1_IN | \
                                            REG9_PG_DIR_PG3_IN | \
                                            /** �� PG0 �� PG3 ���Ͽ�����������; Ӧ�û�е�л����� */    \
                                            /**  REG9_PG_PULL_PG0_UP     PG0����                       \
                                                REG9_PG_PULL_PG1_UP     PG1����                       \
                                                REG9_PG_PULL_PG2_UP     PG2����                       \
                                                REG9_PG_PULL_PG3_UP     PG3���� */                    \
                                            REG9_PG_PULL_PG0_UP | \
                                            REG9_PG_PULL_PG1_UP | \
                                            REG9_PG_PULL_PG2_UP | \
                                            REG9_PG_PULL_PG3_UP | \
                                            /** ���������ܶȻ��������ȵ���������� PWM0/PDM0 ��������ڶ˿� PG0 \
                                            ���� PG2. PWM1/PDM1 ��������ڶ˿�PG1 ���� PG3 */                      \
                                            /**  REG9_PI_PWM0_EN     PWM0 ����                                     \
                                                REG9_PI_PDM0_EN     PDM0 ����                                     \
                                                REG9_PI_PWM1_EN     PWM1 ����                                     \
                                                REG9_PI_PDM1_EN     PDM1 ���� */                                  \
                                            REG9_PI_PDM0_EN | \
                                            /** ��������ӿڵĻ���Ƶ��,�����ڲ���Ƶ��(OLF)�����ⲿ��Ƶ���� (OHF) */  \
                                            /**  REG9_PCLK_SEL_2OHF      OHF * 2                                     \
                                                REG9_PCLK_SEL_OHF       OHF                                         \
                                                REG9_PCLK_SEL_OHF2      OHF / 2                                     \
                                                REG9_PCLK_SEL_OHF4      OHF / 4                                     \
                                                REG9_PCLK_SEL_2OLF      OLF * 2                                     \
                                                REG9_PCLK_SEL_OLF       OLF                                         \
                                                REG9_PCLK_SEL_OLF2      OLF / 2                                     \
                                                REG9_PCLK_SEL_OLF4      OLF / 4                                     \
                                                                                                                    \
                                                REG9_PI0_CLK_SEL_2OHF   PI0ʱ��ѡ�� OHF * 2                         \
                                                REG9_PI0_CLK_SEL_OHF    PI0ʱ��ѡ�� OHF                             \
                                                REG9_PI0_CLK_SEL_OHF2   PI0ʱ��ѡ�� OHF / 2                         \
                                                REG9_PI0_CLK_SEL_OHF4   PI0ʱ��ѡ�� OHF / 4                         \
                                                REG9_PI0_CLK_SEL_2OLF   PI0ʱ��ѡ�� OLF * 2                         \
                                                REG9_PI0_CLK_SEL_OLF    PI0ʱ��ѡ�� OLF                             \
                                                REG9_PI0_CLK_SEL_OLF2   PI0ʱ��ѡ�� OLF / 2                         \
                                                REG9_PI0_CLK_SEL_OLF4   PI0ʱ��ѡ�� OLF / 4                         \
                                                                                                                    \
                                                REG9_PI1_CLK_SEL_2OHF   PI1ʱ��ѡ�� OHF * 2                         \
                                                REG9_PI1_CLK_SEL_OHF    PI1ʱ��ѡ�� OHF                             \
                                                REG9_PI1_CLK_SEL_OHF2   PI1ʱ��ѡ�� OHF / 2                         \
                                                REG9_PI1_CLK_SEL_OHF4   PI1ʱ��ѡ�� OHF / 4                         \
                                                REG9_PI1_CLK_SEL_2OLF   PI1ʱ��ѡ�� OLF * 2                         \
                                                REG9_PI1_CLK_SEL_OLF    PI1ʱ��ѡ�� OLF                             \
                                                REG9_PI1_CLK_SEL_OLF2   PI1ʱ��ѡ�� OLF / 2                         \
                                                REG9_PI1_CLK_SEL_OLF4   PI1ʱ��ѡ�� OLF / 4 */                      \
                                            REG9_PI0_CLK_SEL_2OLF | \
                                            /** ��������ľ��� */                           \
                                            /**  REG9_PRES_7BITS     7 bit                   \
                                                REG9_PRES_8BITS     8 bit                   \
                                                REG9_PRES_9BITS     9 bit                   \
                                                REG9_PRES_10BITS    10 bit                  \
                                                                                            \
                                                REG9_PI0_RES_7BITS  PI0���徫�� 7 bit       \
                                                REG9_PI0_RES_8BITS  PI0���徫�� 8 bit       \
                                                REG9_PI0_RES_9BITS  PI0���徫�� 9 bit       \
                                                REG9_PI0_RES_10BITS PI0���徫�� 10 bit      \
                                                                                            \
                                                REG9_PI1_RES_7BITS  PI1���徫�� 7 bit       \
                                                REG9_PI1_RES_8BITS  PI1���徫�� 8 bit       \
                                                REG9_PI1_RES_9BITS  PI1���徫�� 9 bit       \
                                                REG9_PI1_RES_10BITS PI1���徫�� 10 bit*/    \
                                            REG9_PI0_RES_10BITS | \
                                            REG9_PI1_RES_10BITS

/** REG10�Ĵ���ֵ������0x180000�ǹ̶�ֵ */
#define REG10_IN_1_8V_REGULATOR_VALUE       0x180000 | \
                                            /** REG10������1.8 V ���ĵ�ѹ���������͹��ĵ�����Ҳ���Ի�úõĲ�����   \
                                            ��,���������RDC�Ĳ����ٶȺ�ʱ���й�.��ο���¼���е����� */          \
                                            /**  REG10_PV_CORE_CTL_STANDARD      ��׼                              \
                                                REG10_V_CORE_CTL_LOW_POWER      �͹��� */                          \
                                            REG10_PV_CORE_CTL_STANDARD

/** PARAM0�Ĵ���ֵ����Ӧ�ã���Ϊ0 */
#define PARAM0_VALUE                        0x00

/** PARAM1�Ĵ���ֵ����Ӧ�ã���Ϊ0 */
#define PARAM1_VALUE                        0x00

/** PARAM2�Ĵ���ֵ */
#define PARAM2_PULSE_SELECT_VALUE           /** Param2��ѡ��PDM/PWM������ź�Դ */        \
                                            /**  PARAM2_SELECT_C1_C0         C1/C0       \
                                                PARAM2_SELECT_R0_RREF       R0/R�ο�      \
                                                PARAM2_SELECT_R2_RREF       R2/R�ο� */   \
                                                                                        \
                                            /** ��һģʽ */                              \
                                            /**  PARAM2_SELECT_C2_C0         C2/C0       \
                                                PARAM2_SELECT_C3_C0         C3/C0       \
                                                PARAM2_SELECT_C4_C0         C4/C0       \
                                                PARAM2_SELECT_C5_C0         C5/C0       \
                                                PARAM2_SELECT_C6_C0         C6/C0       \
                                                PARAM2_SELECT_C7_C0         C7/C0 */    \
                                                                                        \
                                            /** ���ģʽ */                              \
                                            /**  PARAM2_SELECT_C3_C2         C3/C2       \
                                                PARAM2_SELECT_C5_C4         C5/C4       \
                                                PARAM2_SELECT_C7_C6         C7/C6 */    \
                                            0x00

/** PARAM3�Ĵ���ֵ */
#define PARAM3_PULSE0_SLOPE_VALUE           /** Param3�������������0��б�ʣ��з��Ź̶�����,19λ������4λС�� */          \
                                            /**  PARAM3_PULSE0_SLOPE_CONFIG  -524,288 �� +524,288 �� 0.0625Ϊ��С��λ       \
                                                PARAM3_PULSE0_SLOPE         �����������0��б��,                         \
                                                                        ���ľ���ֵ��PARAM3_PULSE0_SLOPE_CONFIG������ */  \
                                            PARAM3_PULSE0_SLOPE
/** PARAM4�Ĵ���ֵ */
#define PARAM4_PULSE0_OFFSET_VALUE          /** Param4�������������0������з��Ź̶�����, 22 ����, 1 λС��.          \
                                            ��һλС��������ѧ�������ֵ, ����������� */                               \
                                            /**  PARAM4_PULSE0_OFFSET_CONFIG -4,194,304�� + 4,194,303.5 �� 0.5Ϊ��С��λ    \
                                                PARAM4_PULSE0_OFFSET        �����������0������з��Ź̶�����,          \
                                                                        ���ľ���ֵ��PARAM4_PULSE0_OFFSET_CONFIG������ */  \
                                            PARAM4_PULSE0_OFFSET

/** PARAM5�Ĵ���ֵ */
#define PARAM5_PULSE1_SLOPE_VALUE           /** Param5�������������1��б�ʣ��з��Ź̶�����,19λ������4λС�� */          \
                                            /**  PARAM5_PULSE1_SLOPE_CONFIG  -524,288 �� +524,288 �� 0.0625Ϊ��С��λ   \
                                                PARAM5_PULSE1_SLOPE         �����������1��б��,                         \
                                                                        ���ľ���ֵ��PARAM5_PULSE1_SLOPE_CONFIG������ */  \
                                            PARAM5_PULSE1_SLOPE

/** PARAM6�Ĵ���ֵ */
#define PARAM6_PULSE1_OFFSET_VALUE          /** Param6�������������1������з��Ź̶�����, 22 ����, 1 λС��.          \
                                            ��һλС��������ѧ�������ֵ, ����������� */                               \
                                            /**  PARAM6_PULSE1_OFFSET_CONFIG -4,194,304�� + 4,194,303.5 �� 0.5Ϊ��С��λ  \  
                                                PARAM6_PULSE1_OFFSET        �����������1������з��Ź̶�����,          \
                                                                        ���ľ���ֵ��PARAM6_PULSE1_OFFSET_CONFIG������ */  \
                                            PARAM6_PULSE1_OFFSET

/** PARAM7�Ĵ���ֵ */
#define PARAM7_CAP_MEASURE_CONFIG_VALUE     /** Param7��ѡ��һ���߲�ִ����� */    \
                                            /**  PARAM7_SEL_SINGLE           ��һ      \
                                                PARAM7_SEL_DIFFERENTIAL     ��� */   \
                                            PARAM7_SEL_SINGLE

/** PARAM8�Ĵ���ֵ */
#define PARAM8_GAIN_CORR_VALUE              /** Param8�������ڲ������ĳ˷�ϵ�� �޷��Ź̶�������,3λ����,21λС�� */    \
                                            /**  PARAM8_GAIN_CORR_CONFIG     0 �� 7.9999                              \
                                                PARAM8_GAIN_CORR            �����ڲ������ĳ˷�ϵ��,                    \
                                                                        ���ľ���ֵ��PARAM8_GAIN_CORR_CONFIG������ */  \
                                            PARAM8_GAIN_CORR

/** REG20�Ĵ���ֵ */
#define REG20_RUNBIT_VALUE                  /** REG20���豸�����Ĵ��� */                                 \
                                            /** ���������豸,��֤�����ù����о�ֹ,ǿ��DSP���ھ�ֹ״̬    \
                                                REG20_RUN_DISABLE                                       \
                                                ���� ����ģʽ                                         \
                                                REG20_RUN_ENABLE    */                                  \
                                            REG20_RUN_DISABLE


/****************************************** ״̬�Ĵ���ֵ *********************************************/
/** ��ʾͨ���ڲ������з���; �����������ܻ�����½�; ����sequence time�����а��� */
#define PCAP_CYC_ACTIVE                     (1 << 23)
/** ��ʾ�¶Ȳ�������; ���������õ�ģʽ,��һλ�����Ա�ʾĿǰ�豸���ڵ�
��һ����ʼ������ߴ�����һ�ο�ʼ���������� */
#define PCAP_T_END_FLAG                     (1 << 22)
/** ��ʾ�����üĴ���20�����õ�RUNBIT��״̬ */
#define PCAP_RUNBIT                         (1 << 20)
/** �������д�����������һ���ۺ� */
#define PCAP_COMBI_ERR                      (1 << 16)
/** ��ʾ�����������CDC���������� */
#define PCAP_CAP_ERR                        (1 << 13)
/** �� CDC �˿��е�PC7�˿��д���(���ܵ���̫��) */
#define PCAP_CAP_ERR_PC7                    (1 << 12)
/** �� CDC �˿��е�PC6�˿��д���(���ܵ���̫��) */
#define PCAP_CAP_ERR_PC6                    (1 << 11)
/** �� CDC �˿��е�PC5�˿��д���(���ܵ���̫��) */
#define PCAP_CAP_ERR_PC5                    (1 << 10)
/** �� CDC �˿��е�PC4�˿��д���(���ܵ���̫��) */
#define PCAP_CAP_ERR_PC4                    (1 << 9)
/** �� CDC �˿��е�PC3�˿��д���(���ܵ���̫��) */
#define PCAP_CAP_ERR_PC3                    (1 << 8)
/** �� CDC �˿��е�PC2�˿��д���(���ܵ���̫��) */
#define PCAP_CAP_ERR_PC2                    (1 << 7)
/** �� CDC �˿��е�PC1�˿��д���(���ܵ���̫��) */
#define PCAP_CAP_ERR_PC1                    (1 << 6)
/** �� CDC �˿��е�PC0�˿��д���(���ܵ���̫��) */
#define PCAP_CAP_ERR_PC0                    (1 << 5)
/** ��ʾ�¶Ȳ���������� RDC���������� */
#define PCAP_TEMP_ERR                       (1 << 3)

/** ���üĴ�����ַ */
#define CONFIG_REG0_ADDR                    0
#define CONFIG_REG1_ADDR                    1
#define CONFIG_REG2_ADDR                    2
#define CONFIG_REG3_ADDR                    3
#define CONFIG_REG4_ADDR                    4
#define CONFIG_REG5_ADDR                    5
#define CONFIG_REG6_ADDR                    6
#define CONFIG_REG7_ADDR                    7
#define CONFIG_REG8_ADDR                    8
#define CONFIG_REG9_ADDR                    9
#define CONFIG_REG10_ADDR                   10
#define CONFIG_PARAM0_ADDR                  11
#define CONFIG_PARAM1_ADDR                  12
#define CONFIG_PARAM2_ADDR                  13
#define CONFIG_PARAM3_ADDR                  14
#define CONFIG_PARAM4_ADDR                  15
#define CONFIG_PARAM5_ADDR                  16
#define CONFIG_PARAM6_ADDR                  17
#define CONFIG_PARAM7_ADDR                  18
#define CONFIG_PARAM8_ADDR                  19
#define CONFIG_REG20_ADDR                   20


/** ����Ĵ�����ַ */
#define RESULT_REG0_ADDR                    0
#define RESULT_REG1_ADDR                    1
#define RESULT_REG2_ADDR                    2
#define RESULT_REG3_ADDR                    3
#define RESULT_REG4_ADDR                    4
#define RESULT_REG5_ADDR                    5
#define RESULT_REG6_ADDR                    6
#define RESULT_REG7_ADDR                    7
#define RESULT_STATUS_ADDR                  8
#define RESULT_REG8_ADDR                    11          //��Ӧ��
#define RESULT_REG9_ADDR                    12          //��Ӧ��
#define RESULT_REG10_ADDR                   13
#define RESULT_REG11_ADDR                   14

/** ������ */
#define PCAP_WRITE_SRAM_OPT                 0x90        ///< дSRAM����
#define PCAP_READ_SRAM_OPT                  0x10        ///< ��SRAM����
#define PCAP_WRITE_OTP_OPT                  0xA0        ///< дOTP����
#define PCAP_READ_OTP_OPT                   0x20        ///< ��OTP����
#define PCAP_WRITE_CONFIG_OPT               0xC0        ///< д���ò���
#define PCAP_READ_RESULT_OPT                0x40        ///< ���������

#define PCAP_RESET_CMD                      0x88        ///< PCapоƬ��λָ��
#define PCAP_PTL_RESET_CMD                  0x8A        ///< PCapоƬ���ָ�λָ��
#define PCAP_MEASURE_CMD                    0x8C        ///< PCapоƬ��ʼһ�ε��ݲ���ָ��
#define PCAP_TERMINATE_WRITE2OTP_CMD        0x84        ///< PCapоƬ��ֹwrite-to-OTP �Ľ���ָ��
#define PCAP_START_TEMP_MEASURE_CMD         0x8E        ///< PCapоƬ��ʼһ���¶Ȳ���ָ��


///< SPIƬѡ
#define PCAP_SPI_NSS_GPIO_PIN                                GPIO_PIN_15
///< SPIƬѡ�˿�
#define PCAP_SPI_NSS_GPIO_PORT                               GPIOA
///< SPIƬѡ�˿�ʱ��
#define PCAP_SPI_NSS_GPIO_CLK()                              __HAL_RCC_GPIOA_CLK_ENABLE()

///< SPIʹ��
#define PCAP_SPI_ENABLE                     HAL_GPIO_WritePin(PCAP_SPI_NSS_GPIO_PORT, PCAP_SPI_NSS_GPIO_PIN, GPIO_PIN_RESET)
///< SPI��ֹ
#define PCAP_SPI_DISABLE                    HAL_GPIO_WritePin(PCAP_SPI_NSS_GPIO_PORT, PCAP_SPI_NSS_GPIO_PIN, GPIO_PIN_SET)


#ifndef OP_SUCCESS
#define OP_SUCCESS
#endif // OP_SUCCESS

#ifndef OP_FAILED
#define OP_FAILED
#endif // OP_FAILED

/**@brief       PCapоƬ��ʼ��
* @return       ����ִ�н��
* - None
*/
void PCap_Init(void);

/**@brief       PCapоƬ��λ
* @return       ����ִ�н��
* - None
*/
void PCap_Reset(void);

/**@brief       PCapоƬ���ָ�λ
* @return       ����ִ�н��
* - None
*/
void PCap_PTL_Reset(void);

/**@brief       PCapоƬ��ʼ����
* @return       ����ִ�н��
* - None
*/
void PCap_Measure(void);

/**@brief       PCapоƬ����
* @return       ����ִ�н��
* - None
*/
void PCap_Enable(void);

/**@brief       PCapоƬֹͣ����
* @return       ����ִ�н��
* - None
*/
void PCap_Disable(void);

/**@brief       ��PCapоƬд��̼�
* @return       ����ִ�н��
* - OP_SUCCESS(д��ɹ�)
* - OP_FAILED(д��ʧ��)
*/
uint8_t PCap_WR_SRAM(void);

/**@brief       ��PCapоƬд��������Ϣ
* @return       ����ִ�н��
* - None
*/
void PCap_Reg_Config(void);

/**@brief       ��ȡPCapоƬ״̬
* @return       ����ִ�н��
* - PCap״̬�Ĵ���ֵ
*/
uint32_t PCap_Res_Sta(void);

/**@brief       ��ȡPCap���Ĵ�������
* @param[in]    reg_addr : ���Ĵ�����ַ
* @return       ����ִ�н��
* - PCap���Ĵ���������
*/
uint32_t PCap_Res_Data(uint8_t reg_addr);

#ifdef __cplusplus
}
#endif
#endif // __PICOCAP_H

