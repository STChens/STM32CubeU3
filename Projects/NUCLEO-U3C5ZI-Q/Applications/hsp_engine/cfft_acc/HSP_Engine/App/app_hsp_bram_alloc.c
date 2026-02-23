/* USER CODE BEGIN Header */
/**
  **********************************************************************************************
  * @file           : App/app_hsp_bram_alloc.c
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
/* USER CODE END Header */

#include "app_hsp_bram_alloc.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
/* Exported variables ------------------------------------------------------- */
/* BRAM Resources Static Allocation ----------------------------------------- */

/* USER CODE BEGIN BRAM Static Allocation */
static hsp_bram_resources_t g_bram_resources;

/* USER CODE END BRAM Static Allocation */

/* BRAM Resources Dynamic Allocation --------------------------------------- */

/* USER CODE BEGIN BRAM Dynamic Allocation */

/* USER CODE END BRAM Dynamic Allocation */

/* Access to external variables ----------------------------------------------*/
extern hsp_engine_context_t hmw;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */
/**
  * @brief  Get the HSP BRAM Resources object.
  * @retval Pointer on the HSP BRAM Resources
  */
hsp_bram_resources_t *MX_HSP_BRAM_GetResources(void)
{
  return &g_bram_resources;
}
/* USER CODE END 1 */

uint32_t MX_HSP_BRAM_Allocation(hsp_engine_context_t *hmw)
{
  uint32_t error = 0UL;

  /* USER CODE BEGIN BRAM_ALLOC */
  g_bram_resources.p_buff_cfft = (float32_t *)HSP_BRAM_Malloc(hmw, HSP_BRAM_RSRC_INPUT_DATA_SIZE, HSP_BRAM_ALLOCATION_DEFAULT);
  if (g_bram_resources.p_buff_cfft == NULL) error++;

  g_bram_resources.p_cfft_magnitude = (float32_t *)HSP_BRAM_Malloc(hmw, HSP_BRAM_RSRC_CFFT_RESULT_SIZE, HSP_BRAM_ALLOCATION_DEFAULT);
  if (g_bram_resources.p_cfft_magnitude == NULL) error++;
  /* USER CODE END BRAM_ALLOC */

  return error;
}

/**
  * @}
  */

/**
  * @}
  */

