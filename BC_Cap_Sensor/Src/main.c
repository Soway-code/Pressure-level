#include "main.h"
#include "Picocap_app.h"
#include "adc_app.h"
#include "dac_bsp.h"
#include "iwdg_bsp.h"
#include "ModBus.h"
#include "In_Memory_app.h"

#define APPLICATION_ADDRESS     (uint32_t)0x08003000

#define VECTOR_TABLE_SIZE       48 * 4

#if   (defined ( __CC_ARM ))
__IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
#elif (defined (__ICCARM__))
#pragma location = 0x20000000
__no_init __IO uint32_t VectorTable[48];
#elif defined   (  __GNUC__  )
__IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
#endif

/* ϵͳʱ������ */
void SystemClock_Config(void);


int main(void)
{
    /* F0ϵ��û���ж���������ӳ��Ĵ��������ԣ�����ʹ����һ�ַ����������ж�������
    0x20000000��ʼ���ڴ�ռ��У��ٽ�����ڴ��ַ��0x00000000������ж����������ӳ�� */
    memcpy((void*)VectorTable, (void*)APPLICATION_ADDRESS, VECTOR_TABLE_SIZE); 
    /* ��ϵͳ�ڴ�ӳ�䵽0x00000000 */
    __HAL_SYSCFG_REMAPMEMORY_SRAM();
    
    uint32_t                    FilterResult;           //�˲���Ľ��
    DataFilterParam             FilterParam;            //�����˲���Ҫ�Ĳ����ṹ��
    PCap_DataConvert_Param      DataConvert_Param;      //PCap������ת����Ҫ�Ĳ����ṹ��
    PCap_DataConvert_Out_Param  DataConvert_Out;        //PCapת����������ݵĽṹ��
    ADC_TemperParam_TypeDef     ADC_TemperParam;        //ADC�¶ȴ�����Ҫ�Ĳ����ṹ��
    ADC_TemperOut_TypeDef       ADC_TemperOut;          //ADC�¶ȴ���������ݵĽṹ��
    ModBusBaseParam_TypeDef     ModBusBaseParam;        //ModBus����Ļ��������ṹ
    ModBus_Device_Param         ModBus_Device;          //ModBus�����豸�Ĳ����ṹ��
    uint32_t                    PCap_Tick_Old  = HAL_GetTick();    
    
    ModBus_Device.ADC_TemperParam         = &ADC_TemperParam;
    ModBus_Device.ADC_TemperOut           = &ADC_TemperOut;
    ModBus_Device.DataFilter              = &FilterParam;
    ModBus_Device.PCap_DataConvert        = &DataConvert_Param; 
    ModBus_Device.PCap_DataConvert_Out    = &DataConvert_Out;
    
    HAL_Init();                                         //HAL���ʼ��
    SystemClock_Config();                               //ϵͳʱ������

    Check_Device_Param();                               //����ڲ��豸����    
    DataFilterParam_Init(&FilterParam, DATA_BUF_MAX);   //�˲�������ʼ��
    DataConvertParam_Init(&DataConvert_Param);          //PCap����ת��������ʼ��
    Sensor_ADC_TemperParam_Init(&ADC_TemperParam);      //ADC�¶ȴ��������ʼ��
    PCap_Init();                                        //PCap��ʼ��    
    Sensor_ADC_Init();                                  //ADC��ʼ��
    BSP_DAC_Init();                                     //DAC��ʼ��
    BSP_IWDG_Init();                                    //�������Ź���ʼ��
    ModBus_Init(&ModBusBaseParam);                      //ModBus��ʼ��(�������ڳ�ʼ��)
    
    while (1)
    {
        //���Ź�ι��
        BSP_IWDG_Refresh();
        //ModBus����
        if(Sensor_USART_Get_RX_Updata_Flag())       //�������ݸ�����
        {
            ModbusHandle(&ModBusBaseParam, &ModBus_Device);
            Sensor_USART_Clear_RX_Updata_Flag();    //����������ݸ��±�־
        }        
        //��ʱʱ�䵽�ɼ�����ֵ
        if(HAL_GetTick() - PCap_Tick_Old > PCAP_COLLECT_CYCLE)
        {
            //��ȡPCap���ݲ��жϷ���״̬,�ɹ�״̬������˲�������ת��
            if(Sensor_PCap_GetResult(RESULT_REG1_ADDR, 
                                    &ModBus_Device.PCap_DataConvert_Out->PCap_ResultValue, 
                                    1) == OP_SUCCESS)
            {
                //�����˲����ж��Ƿ�ɹ�
                if(Sensor_DataFilter(&FilterParam, 
                            ModBus_Device.PCap_DataConvert_Out->PCap_ResultValue, &FilterResult) == OP_SUCCESS)
                {
                    //��ֵת��
                    Sensor_PCap_DataConvert(&DataConvert_Param, 
                                            FilterResult, 
                                            &DataConvert_Out);
                    //DAת��,ʹ��ͨ��2
                    if(DataConvert_Param.CapDA_ClibEn == DAOUTCLIB_ENABLE)
                    {
                        BSP_DAC_Convert(DataConvert_Out.PCapDA_ResultValue, DA_CHANNEL_2);
                    }
                    else
                    {
                        BSP_DAC_Convert(DataConvert_Out.PCapDA_OutValue, DA_CHANNEL_2);
                    }
                }
            }
            PCap_Tick_Old = HAL_GetTick();
        }
        //�ж�ADC�Ƿ񱻸���
        if(Sensor_ADC_Get_Updata_Flag() == UPDATA_OK)
        {
            //��ȡADת�������¶�ֵ
            ADC_TemperOut.TemperInAir = Sensor_ADC_Get_TemperData();
            //���ADC���±�־,����ADCת��
            Sensor_ADC_Clean_Updata_Flag();
        }
        //�������¼�����,һ��Ϊ�Զ��ϴ�
        SensorEvent(&ModBusBaseParam, &ModBus_Device);
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14
                              |RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

