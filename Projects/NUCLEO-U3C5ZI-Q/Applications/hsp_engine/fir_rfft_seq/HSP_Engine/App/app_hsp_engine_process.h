/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : app_hsp_engine_process.h
  * @brief          : Header for app_hsp_engine_process.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 - 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef APP_HSP_ENGINE_PROCESS_H
#define APP_HSP_ENGINE_PROCESS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ----------------------------------------------------------------- */
#include "hsp.h"
/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup APP_HSP_ENGINE
* @{
*/

/** @defgroup HSP_Engine_Exported_Variables HSP_Engine_Exported_Variables
  * @{
  */
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/** @defgroup HSP_Engine_Exported_FunctionsPrototype HSP_Engine_Exported_FunctionsPrototype
  * @{
  */
/* Exported functions -------------------------------------------------------*/
/* USER CODE BEGIN Exported Functions */
uint32_t MX_HSP_Engine_InitiateProcess(void);
uint32_t MX_HSP_Engine_ProcessFIR_FFT(void);
/* USER CODE END Exported Functions */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* APP_HSP_ENGINE_PROCESS_H */

