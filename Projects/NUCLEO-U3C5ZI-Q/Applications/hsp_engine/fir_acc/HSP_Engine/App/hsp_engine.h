/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : hsp_engine.h
  * @brief          : Header for hsp_engine.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HSP_ENGINE__H__
#define __HSP_ENGINE__H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hsp.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup HSP_DRIVER
  * @{
  */

/** @defgroup HSP
  * @{
  */

/** @defgroup HSP_Engine_Exported_Variables HSP_Engine_Exported_Variables
  * @brief Public variables.
  * @{
  */

/**
  * @}
  */

/** @defgroup HSP_Engine_Exported_FunctionsPrototype HSP_Engine_Exported_FunctionsPrototype
  * @brief Declaration of public functions for HSP Engine.
  * @{
  */

/* Exported functions -------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0*/

/** @brief HSP Engine initialization function. */
void MX_HSP_Engine_Init(void);
void MX_HSP_Engine_Process(void);
hsp_engine_context_t* MX_HSP_Engine_GetContext(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __HSP_ENGINE__H__ */

