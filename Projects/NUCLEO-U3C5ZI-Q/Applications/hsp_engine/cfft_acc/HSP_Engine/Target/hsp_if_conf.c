/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Target/hsp_if_conf.c
  * @brief          : This file implements the interface between 
  *                   the HSP Engine MW and HSP Peripheral
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

#include "hsp_if_conf.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

HSP_HandleTypeDef hhsp1;
void Error_Handler(void);

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* MSP Init */

void HAL_HSP_MspInit(HSP_HandleTypeDef* hspHandle)
{
  if(hspHandle->Instance==HSP1)
  {
  /* USER CODE BEGIN HSP1_MspInit 0 */

  /* USER CODE END HSP1_MspInit 0 */
    /* Enable Peripheral clock */
    __HAL_RCC_HSP1_CLK_ENABLE();
  /* USER CODE BEGIN HSP1_MspInit 1 */

  /* USER CODE END HSP1_MspInit 1 */
  }
}

void HAL_HSP_MspDeInit(HSP_HandleTypeDef* hspHandle)
{
  if(hspHandle->Instance==HSP1)
  {
  /* USER CODE BEGIN HSP1_MspDeInit 0 */

  /* USER CODE END HSP1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_HSP1_CLK_DISABLE();
  /* USER CODE BEGIN HSP1_MspDeInit 1 */

  /* USER CODE END HSP1_MspDeInit 1 */
  }
}

/*******************************************************************************
                       HAL Driver Interface (HSP Engine Library --> HSP)
*******************************************************************************/
/**
  * @brief  Initializes the low level portion of the driver.
  * @param  hmw  HSP Engine handle
  * @retval hsp_if_status_t status
  */
hsp_if_status_t HSP_Engine_IF_Init(hsp_engine_context_t *hmw)
{
  /* Init HSP Driver. */
  hhsp1.global_state = HAL_HSP_STATE_RESET;

  hhsp1.Instance = HSP1;
  if (HAL_HSP_Init(&hhsp1) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  /* Link the driver to the MW. */
  hmw->hdriver = &hhsp1;

  return HSP_IF_OK;
}
/**
  * @}
  */

/**
  * @}
  */

