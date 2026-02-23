/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/app_hsp_bram_alloc.h
  * @brief   Header file
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
#ifndef APP_HSP_BRAM_ALLOC_H
#define APP_HSP_BRAM_ALLOC_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "hsp_def.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup APP_HSP_ENGINE
  * @{
  */
/** @defgroup APP_HSP_BRAM_ALLOC_Exported_Constants Application HSP BRAM Allocation Constants
  * @{
  */

/* USER CODE BEGIN BRAM Constants */
#define HSP_BRAM_RSRC_INPUT_DATA_SIZE  512U
#define HSP_BRAM_RSRC_INPUT_COMPLEX_SAMPLES_SIZE  (HSP_BRAM_RSRC_INPUT_DATA_SIZE / 2U)
#define HSP_BRAM_RSRC_CFFT_RESULT_SIZE  HSP_BRAM_RSRC_INPUT_COMPLEX_SAMPLES_SIZE

/* USER CODE END BRAM Constants */

/**
  * @}
  */

/** @defgroup HSP_BRAM_DYNAMIC_Resources HSP BRAM Dynamic Resources
  * @{
  */
typedef struct {

  /* HSP BRAM Resources allocated statically */
  /* ... */
  /* USER CODE BEGIN BRAM Static Allocation */
  /* USER CODE END BRAM Dynamic Allocation */

  /* HSP BRAM Resources allocated dynamically */
  /* ... */
/* USER CODE BEGIN BRAM Dynamic Allocation */
  float32_t *p_buff_cfft;
  float32_t *p_cfft_magnitude;
/* USER CODE END BRAM Dynamic Allocation */
} hsp_bram_resources_t;

/**
  * @}
  */

/** @defgroup APP_HSP_BRAM_ALLOC_Exported_Functions Application HSP BRAM Exported Functions
  * @{
  */

uint32_t MX_HSP_BRAM_Allocate(hsp_engine_context_t *hmw);

hsp_bram_resources_t *MX_HSP_BRAM_GetResources(void);

/* USER CODE BEGIN Exported Functions */

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

#endif /* APP_HSP_BRAM_ALLOC_H */
