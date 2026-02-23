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

/* USER CODE END BRAM Static Allocation */

/* BRAM Resources Dynamic Allocation --------------------------------------- */

/* USER CODE BEGIN BRAM Dynamic Allocation */
static hsp_bram_resources_t g_bram_resources;

static float fir_coefs[HSP_BRAM_RSRC_FIR_COEF_SIZE]  __attribute__((section("HSP_DATA_BRAM"))) = {
  0.00240166, 0.00483097, 0.00663137, 0.00619702, 0.00217623, -0.00566713, -0.0157708, -0.0246505,
  -0.0275608, -0.0198746, 0.00120821, 0.0352238, 0.0778732, 0.121663, 0.157681, 0.178013,
  0.178013, 0.157681, 0.121663, 0.0778732, 0.0352238, 0.00120821, -0.0198746, -0.0275608,
  -0.0246505, -0.0157708, -0.00566713, 0.00217623, 0.00619702, 0.00663137, 0.00483097, 0.00240166
};

static uint32_t pl_call __attribute__((section("HSP_DATA_BRAM"))) = 0U;

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

  /* State buffers allocation */

  /* Buffers allocation */

  /* USER CODE BEGIN BRAM_ALLOC */
  g_bram_resources.fir_coefs = fir_coefs;
  g_bram_resources.pl_call = &pl_call;
  
  g_bram_resources.fir_state_id = HSP_BRAM_MallocStateBuffer_Fir(hmw, HSP_BRAM_RSRC_FIR_STATE_TAPS_SIZE, HSP_BRAM_RSRC_SAMPLES_NBR, HSP_BRAM_ALLOCATION_DEFAULT);
  if (g_bram_resources.fir_state_id == 0UL) error++;

  g_bram_resources.p_buff_in = (float32_t *)HSP_BRAM_Malloc(hmw, HSP_BRAM_RSRC_SAMPLES_NBR, HSP_BRAM_ALLOCATION_DEFAULT);
  if (g_bram_resources.p_buff_in == NULL) error++;

  g_bram_resources.p_results = (float32_t *)HSP_BRAM_Malloc(hmw, HSP_BRAM_RSRC_SAMPLES_NBR, HSP_BRAM_ALLOCATION_DEFAULT);
  if (g_bram_resources.p_results == NULL) error++;
  /* USER CODE END BRAM_ALLOC */

  return error;
}

/**
  * @}
  */

/**
  * @}
  */

