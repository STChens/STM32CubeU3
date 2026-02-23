/* USER CODE BEGIN Header */
/**
  **********************************************************************************************
  * @file           : App/app_hsp_bram_alloc.h
  * @version        : v1_0_Cube
  * @brief          :This file contains resources allocated in HSP BRAM region memory
  ***********************************************************************************************
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
/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef APP_HSP_BRAM_ALLOC_H
#define APP_HSP_BRAM_ALLOC_H
/* USER CODE END Header */

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
/** @defgroup APP_HSP_BRAM_ALLOC_Exported_Constants Application HSP BRAM Allocation Constants
  * @{
  */

/* USER CODE BEGIN BRAM Constants */
#define HSP_BRAM_RSRC_INPUT_SIGNAL_SAMPLES_SIZE  250U
#define HSP_BRAM_RSRC_FIR_COEF_SIZE  51U
#define HSP_BRAM_RSRC_FIR_STATE_TAPS_SIZE  HSP_BRAM_RSRC_FIR_COEF_SIZE
#define HSP_BRAM_RSRC_OUTPUT_SAMPLES_SIZE  250U
/* USER CODE END BRAM Constants */

/**
  * @}
  */

/** @defgroup HSP_BRAM_STATIC_Resources HSP BRAM Static Resources
  * @{
  */
/* Externalize any buffer/variable required to be use with any HSP Processing functions */

/* USER CODE BEGIN BRAM Static Allocation */

/* USER CODE END BRAM Static Allocation */

/**
  * @}
  */

/** @defgroup HSP_BRAM_DYNAMIC_Resources HSP BRAM Dynamic Resources
  * @{
  */
/* Externalize any buffer/variable required to be use with any HSP Processing functions */
/* USER CODE BEGIN BRAM Dynamic Allocation */
typedef struct {

  /* HSP BRAM Resources allocated statically */
  /* ... */
  
  /* HSP BRAM Resources allocated dynamically */
  /* ... */
  hsp_filter_state_identifier_t fir_state_id;  /* BRAM FIR State Identifier */
  float32_t *p_buff_in;  /* BRAM Input Buffer */
  float32_t *p_coef;  /* BRAM FIR Coefficient Buffer */
  float32_t *p_buff_out;  /* BRAM Output Buffer */
} hsp_bram_resources_t;
/* USER CODE END BRAM Dynamic Allocation */

/**
  * @}
  */

/** @defgroup APP_HSP_BRAM_ALLOC_Exported_Functions Application HSP BRAM Exported Functions
  * @{
  */
uint32_t MX_HSP_BRAM_Allocation(hsp_engine_context_t *hmw);

/* USER CODE BEGIN Exported Functions */
hsp_bram_resources_t *MX_HSP_BRAM_GetResources(void);

/* USER CODE END Exported Functions */

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
#endif /* __cplusplus */

#endif /* APP_HSP_BRAM_ALLOC_H */
