/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   app_hsp_engine_seq.c
  * @brief  This file implements the application using Processing List 
  ******************************************************************************
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
#include "app_hsp_engine_seq.h"
#include "app_hsp_bram_alloc.h"

#include <string.h>
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
/* Exported variables --------------------------------------------------------*/
/* Access to external variables ----------------------------------------------*/

extern void Error_Handler(void);

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes ---------------------------------------------- */

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
 * @brief Record Processing list: PL_FIR
 * @param hsp_core_handle_t
 * @retval uint32_t
 */
uint32_t MX_HSP_SEQ_Record_PL_FIR(hsp_engine_context_t *hmw)
{
  uint32_t error = 0UL;
  hsp_core_status_t mw_status;
  hsp_bram_resources_t *p_bram_rsrc = MX_HSP_BRAM_GetResources();

  mw_status = HSP_SEQ_StartRecordPL(hmw, HSP_PL_ID_FIR);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_Fir_f32(hmw, p_bram_rsrc->p_buff_in, p_bram_rsrc->p_coef, p_bram_rsrc->fir_state_id,
                              p_bram_rsrc->p_buff_out, HSP_BRAM_RSRC_INPUT_SIGNAL_SAMPLES_SIZE,
                              HSP_SEQ_IOTYPE_DEFAULT);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_StopRecordPL(hmw);
  if (mw_status != HSP_CORE_OK) error++;

  return error;
}

/* USER CODE END 1 */

