/**
  ******************************************************************************
  * @file    IAP_Main/Src/flash_if.c 
  * @author  MCD Application Team
  * @brief   This file provides all the memory related operation functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */ 

/** @addtogroup STM32F0xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{
  /* Unlock the Program memory */
  HAL_FLASH_Unlock();

  /* Clear all FLASH flags */
#if defined(STM32F0)
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
#elif defined(STM32L0)
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR |
                         FLASH_FLAG_OPTVERR | FLASH_FLAG_RDERR | FLASH_FLAG_FWWERR |
                         FLASH_FLAG_NOTZEROERR);
#endif
  /* Unlock the Program memory */
  HAL_FLASH_Lock();
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  start: start of user flash area
  * @retval FLASHIF_OK : user flash area successfully erased
  *         FLASHIF_ERASEKO : error occurred
  */
uint32_t FLASH_If_Erase(uint32_t start)
{
  uint32_t NbrOfPages = 0;
  uint32_t PageError = 0;
  FLASH_EraseInitTypeDef pEraseInit;
  HAL_StatusTypeDef status = HAL_OK;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Get the sector where start the user flash area */
  NbrOfPages = (USER_FLASH_END_ADDRESS - start)/FLASH_PAGE_SIZE;

  pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
  pEraseInit.PageAddress = start;
  pEraseInit.NbPages = NbrOfPages;
  status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  if (status != HAL_OK)
  {
    /* Error occurred while page erase */
    return FLASHIF_ERASEKO;
  }

  return FLASHIF_OK;
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  destination: start address for target location
  * @param  p_source: pointer on buffer with data to write
  * @param  length: length of data buffer (unit is 32-bit word)
  * @retval uint32_t 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
uint32_t FLASH_If_Write(uint32_t destination, uint32_t *p_source, uint32_t length)
{
  uint32_t i = 0;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  for (i = 0; (i < length) && (destination <= (USER_FLASH_END_ADDRESS-4)); i++)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, destination, *(uint32_t*)(p_source+i)) == HAL_OK)      
    {
     /* Check the written value */
      if (*(uint32_t*)destination != *(uint32_t*)(p_source+i))
      {
        /* Flash content doesn't match SRAM content */
        return(FLASHIF_WRITINGCTRL_ERROR);
      }
      /* Increment FLASH destination address */
      destination += 4;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return (FLASHIF_WRITING_ERROR);
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  return (FLASHIF_OK);
}

/**
  * @brief  Returns the write protection status of application flash area.
  * @param  None
  * @retval If a sector in application area is write-protected returned value is a combinaison
            of the possible values : FLASHIF_PROTECTION_WRPENABLED, FLASHIF_PROTECTION_PCROPENABLED, ...
  *         If no sector is write-protected FLASHIF_PROTECTION_NONE is returned.
  */
uint32_t FLASH_If_GetWriteProtectionStatus(void)
{
#if defined(STM32F0)
  uint32_t ProtectedPAGE = FLASHIF_PROTECTION_NONE;
  FLASH_OBProgramInitTypeDef OptionsBytesStruct;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* Check if there are write protected sectors inside the user flash area ****/
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct);

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  /* Get pages already write protected ****************************************/
  ProtectedPAGE = ~(OptionsBytesStruct.WRPPage) & FLASH_PAGE_TO_BE_PROTECTED;

  /* Check if desired pages are already write protected ***********************/
  if(ProtectedPAGE != 0)
  {
    /* Some sectors inside the user flash area are write protected */
    return FLASHIF_PROTECTION_WRPENABLED;
  }
  else
  { 
    /* No write protected sectors inside the user flash area */
    return FLASHIF_PROTECTION_NONE;
  }
#elif defined(STM32L0)
  FLASH_OBProgramInitTypeDef config;
  FLASH_AdvOBProgramInitTypeDef adv_config;
  uint32_t wrp1_status = 0, wrp2_status = 0, sectornumber = 0;
  uint32_t protected = FLASHIF_PROTECTION_NONE;

  /* Get the current configuration */
  HAL_FLASHEx_OBGetConfig( &config );
  HAL_FLASHEx_AdvOBGetConfig( &adv_config );

  sectornumber = FLASH_SECTOR_NUMBER;

  /* Not taking application size into account, all the memory starting the beginning address is checked */
  /* As the APPLICATION_ADDRESS is a define constant, this code may be omitted by the optimization in   */
  /* compiler. However it is present for case of user specified APPLICATION_ADDRESS value.               */
  if ( adv_config.BootConfig == OB_BOOT_BANK1 )  /* Test on the user application to be ran in Bank1 */
  {
    if (sectornumber < 32)
    {
      wrp1_status = config.WRPSector & FLASH_PROTECTED_SECTORS;
      wrp2_status = config.WRPSector2 & OB_WRP2_AllPages;
    }
    else
    {
      wrp1_status = config.WRPSector;
      wrp2_status = config.WRPSector2 & ~(uint32_t)(((1 << (sectornumber - 32 )) - 1));
    }
  }
  else /* running from bank 2 */
  {
    if (sectornumber < 32)
    {
      wrp1_status = config.WRPSector & ( FLASH_PROTECTED_SECTORS | (~FLASH_BANK1_MASK) );
      wrp2_status = config.WRPSector2 & OB_WRP2_AllPages;
    }
    else
    {
      wrp1_status = config.WRPSector & (~FLASH_BANK1_MASK);
      wrp2_status = config.WRPSector2 & ~(uint32_t)(((1 << (sectornumber - 32 )) - 1));
    }
  }

  /* Final evaluation of status */
  if ((wrp1_status != 0) || (wrp2_status != 0))
  {
    protected = FLASHIF_PROTECTION_WRPENABLED;
  }
  
  return protected;	
#endif
}

/**
  * @brief  Configure the write protection status of user flash area.
  * @param  protectionstate : FLASHIF_WRP_DISABLE or FLASHIF_WRP_ENABLE the protection
  * @retval uint32_t FLASHIF_OK if change is applied.
  */
uint32_t FLASH_If_WriteProtectionConfig(uint32_t protectionstate)
{
#if defined(STM32F0)
  uint32_t ProtectedPAGE = 0x0;
  FLASH_OBProgramInitTypeDef config_new, config_old;
  HAL_StatusTypeDef result = HAL_OK;
  

  /* Get pages write protection status ****************************************/
  HAL_FLASHEx_OBGetConfig(&config_old);

  /* The parameter says whether we turn the protection on or off */
  config_new.WRPState = (protectionstate == FLASHIF_WRP_ENABLE ? OB_WRPSTATE_ENABLE : OB_WRPSTATE_DISABLE);

  /* We want to modify only the Write protection */
  config_new.OptionType = OPTIONBYTE_WRP;
  
  /* No read protection, keep BOR and reset settings */
  config_new.RDPLevel = OB_RDP_LEVEL_0;
  config_new.USERConfig = config_old.USERConfig;  
  /* Get pages already write protected ****************************************/
  ProtectedPAGE = config_old.WRPPage | FLASH_PAGE_TO_BE_PROTECTED;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Unlock the Options Bytes *************************************************/
  HAL_FLASH_OB_Unlock();
  
  /* Erase all the option Bytes ***********************************************/
  result = HAL_FLASHEx_OBErase();
    
  if (result == HAL_OK)
  {
    config_new.WRPPage    = ProtectedPAGE;
    result = HAL_FLASHEx_OBProgram(&config_new);
  }
  
  return (result == HAL_OK ? FLASHIF_OK: FLASHIF_PROTECTION_ERRROR);
#elif defined(STM32L0)
  FLASH_OBProgramInitTypeDef config_new, config_old;
  FLASH_AdvOBProgramInitTypeDef adv_config;
  HAL_StatusTypeDef result;
  uint32_t sectornumber = 0;

  sectornumber = FLASH_SECTOR_NUMBER;

  /* Get the current configuration */
  HAL_FLASHEx_OBGetConfig( &config_old );
  HAL_FLASHEx_AdvOBGetConfig( &adv_config );

  /* The parameter says whether we turn the protection on or off */
  config_new.WRPState = (protectionstate == FLASHIF_WRP_ENABLE ? OB_WRPSTATE_ENABLE : OB_WRPSTATE_DISABLE);

  /* We want to modify only the Write protection */
  config_new.OptionType = OPTIONBYTE_WRP;
  
  /* No read protection, keep BOR and reset settings */
  config_new.RDPLevel = OB_RDP_LEVEL_0;
  config_new.BORLevel = config_old.BORLevel;
  config_new.USERConfig = config_old.USERConfig;

  /* Not taking application size into account, all the memory starting the beginning address is checked */
  /* As the APPLICATION_ADDRESS is a define constant, this code may be omitted by the optimization in   */
  /* compiler. However it is present for case of user specified APPLICATION_ADDRESS value.               */
  /* Not taking application size into account, all the memory starting the beginning address is modified */
  if ( adv_config.BootConfig != OB_BOOT_BANK1 ) /* BANK2 active for boot */
  {
    if (sectornumber < 32)
    {
      config_new.WRPSector = FLASH_BANK1_MASK & (config_old.WRPSector | FLASH_PROTECTED_SECTORS);
      config_new.WRPSector2 = config_old.WRPSector2 | OB_WRP2_AllPages;
    }
    else
    {
      config_new.WRPSector = FLASH_BANK1_MASK;
      config_new.WRPSector2 = config_old.WRPSector2 | (~(uint32_t)((1 << (sectornumber - 32)) - 1));
    }
  }
  else /* Memory ordering normal */
  {
    if (sectornumber < 32)
    {
      config_new.WRPSector = config_old.WRPSector | FLASH_PROTECTED_SECTORS;
      config_new.WRPSector2 = config_old.WRPSector2 | OB_WRP2_AllPages;
    }
    else
    {
      config_new.WRPSector = 0;
      config_new.WRPSector2 = config_old.WRPSector2 | (~(uint32_t)((1 << (sectornumber - 32)) - 1));
    }
  }

  /* Initiating the modifications */
  result = HAL_FLASH_OB_Unlock();

  /* program if unlock is successful */
  if (result == HAL_OK)
  {
    HAL_FLASHEx_AdvOBProgram(&adv_config);
    result = HAL_FLASHEx_OBProgram(&config_new);
  }

  return (result == HAL_OK ? FLASHIF_OK: FLASHIF_PROTECTION_ERRROR);
#endif
}
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
