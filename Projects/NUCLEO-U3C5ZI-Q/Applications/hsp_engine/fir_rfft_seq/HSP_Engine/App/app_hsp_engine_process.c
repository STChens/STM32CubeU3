/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : app_hsp_engine_process.c
  * @brief           : This file implements the HSP_Engine Process functions
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
#include "main.h"
#include "app_hsp_engine_process.h"
#include "app_hsp_engine_seq.h"
#include "app_hsp_bram_alloc.h"
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NB_DOMINANT_FREQUENCY  5U
#define SAMPLING_FREQUENCY  48000
#define RFFT_RESULT_SIZE  (HSP_BRAM_RSRC_SAMPLES_NBR >> 1U)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

const static float samples[3][HSP_BRAM_RSRC_SAMPLES_NBR] = {
{
  0, 2.05383, 2.76822, 1.92127, 0.516658, -0.177856, 0.262677, 1.176, 1.57313, 1.09743, 
  0.258337, -0.200877, -0.0761205, 0.191979, 0.139704, -0.0946055, 0.133975, 1.21895, 2.68789, 3.45395,
  2.7899, 1.09869, -0.327449, -0.417438, 0.707107, 1.82484, 1.71449, 0.254624, -1.48334, -2.20673,
  -1.51202, -0.125748, 0.866025, 0.991773, 0.645991, 0.474676, 0.617317, 0.611401, 0.017562, -0.958817,
  -1.57313, -1.31461, -0.538576, -0.232669, -1.05785, -2.58793, -3.55392, -2.951, -1, 0.960631,
  1.59235, 0.674046, -0.789905, -1.53117, -1.12436, -0.231408, 0.158919, -0.30997, -1.1287, -1.55419,
  -1.38268, -1.05525, -1.03617, -1.18781, -0.866025, 0.321782, 1.9022, 2.7873, 2.24871, 0.688169,
  -0.603348, -0.556055, 0.707107, 1.96346, 1.99039, 0.665148, -0.942146, -1.54007, -0.726322, 0.77142,
  1.86603, 2.08498, 1.82187, 1.7219, 1.92388, 1.96472, 1.4046, 0.448587, -0.158919, 0.0927912,
  0.848463, 1.12065, 0.248709, -1.3407, -2.37804, -1.8578, -7.9602e-15, 1.8578, 2.37804, 1.3407,
  -0.248709, -1.12065, -0.848463, -0.0927912, 0.158919, -0.448587, -1.4046, -1.96472, -1.92388, -1.7219,
  -1.82187, -2.08498, -1.86603, -0.77142, 0.726322, 1.54007, 0.942146, -0.665148, -1.99039, -1.96346,
  -0.707107, 0.556055, 0.603348, -0.688169, -2.24871, -2.7873, -1.9022, -0.321782, 0.866025, 1.18781,
  1.03617, 1.05525, 1.38268, 1.55419, 1.1287, 0.30997, -0.158919, 0.231408, 1.12436, 1.53117,
  0.789905, -0.674046, -1.59235, -0.960631, 1, 2.951, 3.55392, 2.58793, 1.05785, 0.232669,
  0.538576, 1.31461, 1.57313, 0.958817, -0.017562, -0.611401, -0.617317, -0.474676, -0.645991, -0.991773,
  -0.866025, 0.125748, 1.51202, 2.20673, 1.48334, -0.254624, -1.71449, -1.82484, -0.707107, 0.417438,
  0.327449, -1.09869, -2.7899, -3.45395, -2.68789, -1.21895, -0.133975, 0.0946055, -0.139704, -0.191979,
  0.0761205, 0.200877, -0.258337, -1.09743, -1.57313, -1.176, -0.262677, 0.177856, -0.516658, -1.92127,
  -2.76822, -2.05383, -1.739e-14, 2.05383, 2.76822, 1.92127, 0.516658, -0.177856, 0.262677, 1.176,
  1.57313, 1.09743, 0.258337, -0.200877, -0.0761205, 0.191979, 0.139704, -0.0946055, 0.133975, 1.21895,
  2.68789, 3.45395, 2.7899, 1.09869, -0.327449, -0.417438, 0.707107, 1.82484, 1.71449, 0.254624,
  -1.48334, -2.20673, -1.51202, -0.125748, 0.866025, 0.991773, 0.645991, 0.474676, 0.617317, 0.611401,
  0.017562, -0.958817, -1.57313, -1.31461, -0.538576, -0.232669, -1.05785, -2.58793, -3.55392, -2.951,
  -1, 0.960631, 1.59235, 0.674046, -0.789905, -1.53117, -1.12436, -0.231408, 0.158919, -0.30997,
  -1.1287, -1.55419, -1.38268, -1.05525, -1.03617, -1.18781
},
{
  -0.866025, 0.321782, 1.9022, 2.7873, 2.24871, 0.688169, -0.603348, -0.556055, 0.707107, 1.96346,
  1.99039, 0.665148, -0.942146, -1.54007, -0.726322, 0.77142, 1.86603, 2.08498, 1.82187, 1.7219,
  1.92388, 1.96472, 1.4046, 0.448587, -0.158919, 0.0927912, 0.848463, 1.12065, 0.248709, -1.3407,
  -2.37804, -1.8578, -2.38806e-14, 1.8578, 2.37804, 1.3407, -0.248709, -1.12065, -0.848463, -0.0927912,
  0.158919, -0.448587, -1.4046, -1.96472, -1.92388, -1.7219, -1.82187, -2.08498, -1.86603, -0.77142,
  0.726322, 1.54007, 0.942146, -0.665148, -1.99039, -1.96346, -0.707107, 0.556055, 0.603348, -0.688169,
  -2.24871, -2.7873, -1.9022, -0.321782, 0.866025, 1.18781, 1.03617, 1.05525, 1.38268, 1.55419,
  1.1287, 0.30997, -0.158919, 0.231408, 1.12436, 1.53117, 0.789905, -0.674046, -1.59235, -0.960631,
  1, 2.951, 3.55392, 2.58793, 1.05785, 0.232669, 0.538576, 1.31461, 1.57313, 0.958817,
  -0.017562, -0.611401, -0.617317, -0.474676, -0.645991, -0.991773, -0.866025, 0.125748, 1.51202, 2.20673,
  1.48334, -0.254624, -1.71449, -1.82484, -0.707107, 0.417438, 0.327449, -1.09869, -2.7899, -3.45395,
  -2.68789, -1.21895, -0.133975, 0.0946055, -0.139704, -0.191979, 0.0761205, 0.200877, -0.258337, -1.09743,
  -1.57313, -1.176, -0.262677, 0.177856, -0.516658, -1.92127, -2.76822, -2.05383, -3.478e-14, 2.05383,
  2.76822, 1.92127, 0.516658, -0.177856, 0.262677, 1.176, 1.57313, 1.09743, 0.258337, -0.200877,
  -0.0761205, 0.191979, 0.139704, -0.0946055, 0.133975, 1.21895, 2.68789, 3.45395, 2.7899, 1.09869,
  -0.327449, -0.417438, 0.707107, 1.82484, 1.71449, 0.254624, -1.48334, -2.20673, -1.51202, -0.125748,
  0.866025, 0.991773, 0.645991, 0.474676, 0.617317, 0.611401, 0.017562, -0.958817, -1.57313, -1.31461,
  -0.538576, -0.232669, -1.05785, -2.58793, -3.55392, -2.951, -1, 0.960631, 1.59235, 0.674046,
  -0.789905, -1.53117, -1.12436, -0.231408, 0.158919, -0.30997, -1.1287, -1.55419, -1.38268, -1.05525,
  -1.03617, -1.18781, -0.866025, 0.321782, 1.9022, 2.7873, 2.24871, 0.688169, -0.603348, -0.556055,
  0.707107, 1.96346, 1.99039, 0.665148, -0.942146, -1.54007, -0.726322, 0.77142, 1.86603, 2.08498,
  1.82187, 1.7219, 1.92388, 1.96472, 1.4046, 0.448587, -0.158919, 0.0927912, 0.848463, 1.12065,
  0.248709, -1.3407, -2.37804, -1.8578, -1.35724e-13, 1.8578, 2.37804, 1.3407, -0.248709, -1.12065,
  -0.848463, -0.0927912, 0.158919, -0.448587, -1.4046, -1.96472, -1.92388, -1.7219, -1.82187, -2.08498,
  -1.86603, -0.77142, 0.726322, 1.54007, 0.942146, -0.665148, -1.99039, -1.96346, -0.707107, 0.556055,
  0.603348, -0.688169, -2.24871, -2.7873, -1.9022, -0.321782
},

{
  0.866025, 1.18781, 1.03617, 1.05525, 1.38268, 1.55419, 1.1287, 0.30997, -0.158919, 0.231408,
  1.12436, 1.53117, 0.789905, -0.674046, -1.59235, -0.960631, 1, 2.951, 3.55392, 2.58793,
  1.05785, 0.232669, 0.538576, 1.31461, 1.57313, 0.958817, -0.017562, -0.611401, -0.617317, -0.474676, 
  -0.645991, -0.991773, -0.866025, 0.125748, 1.51202, 2.20673, 1.48334, -0.254624, -1.71449, -1.82484,
  -0.707107, 0.417438, 0.327449, -1.09869, -2.7899, -3.45395, -2.68789, -1.21895, -0.133975, 0.0946055,
  -0.139704, -0.191979, 0.0761205, 0.200877, -0.258337, -1.09743, -1.57313, -1.176, -0.262677, 0.177856,
  -0.516658, -1.92127, -2.76822, -2.05383, -5.217e-14, 2.05383, 2.76822, 1.92127, 0.516658, -0.177856,
  0.262677, 1.176, 1.57313, 1.09743, 0.258337, -0.200877, -0.0761205, 0.191979, 0.139704, -0.0946055,
  0.133975, 1.21895, 2.68789, 3.45395, 2.7899, 1.09869, -0.327449, -0.417438, 0.707107, 1.82484,
  1.71449, 0.254624, -1.48334, -2.20673, -1.51202, -0.125748, 0.866025, 0.991773, 0.645991, 0.474676,
  0.617317, 0.611401, 0.017562, -0.958817, -1.57313, -1.31461, -0.538576, -0.232669, -1.05785, -2.58793,
  -3.55392, -2.951, -1, 0.960631, 1.59235, 0.674046, -0.789905, -1.53117, -1.12436, -0.231408,
  0.158919, -0.30997, -1.1287, -1.55419, -1.38268, -1.05525, -1.03617, -1.18781, -0.866025, 0.321782,
  1.9022, 2.7873, 2.24871, 0.688169, -0.603348, -0.556055, 0.707107, 1.96346, 1.99039, 0.665148,
  -0.942146, -1.54007, -0.726322, 0.77142, 1.86603, 2.08498, 1.82187, 1.7219, 1.92388, 1.96472,
  1.4046, 0.448587, -0.158919, 0.0927912, 0.848463, 1.12065, 0.248709, -1.3407, -2.37804, -1.8578,
  -1.66416e-14, 1.8578, 2.37804, 1.3407, -0.248709, -1.12065, -0.848463, -0.0927912, 0.158919, -0.448587,
  -1.4046, -1.96472, -1.92388, -1.7219, -1.82187, -2.08498, -1.86603, -0.77142, 0.726322, 1.54007,
  0.942146, -0.665148, -1.99039, -1.96346, -0.707107, 0.556055, 0.603348, -0.688169, -2.24871, -2.7873,
  -1.9022, -0.321782, 0.866025, 1.18781, 1.03617, 1.05525, 1.38268, 1.55419, 1.1287, 0.30997,
  -0.158919, 0.231408, 1.12436, 1.53117, 0.789905, -0.674046, -1.59235, -0.960631, 1, 2.951,
  3.55392, 2.58793, 1.05785, 0.232669, 0.538576, 1.31461, 1.57313, 0.958817, -0.017562, -0.611401,
  -0.617317, -0.474676, -0.645991, -0.991773, -0.866025, 0.125748, 1.51202, 2.20673, 1.48334, -0.254624,
  -1.71449, -1.82484, -0.707107, 0.417438, 0.327449, -1.09869, -2.7899, -3.45395, -2.68789, -1.21895,
  -0.133975, 0.0946055, -0.139704, -0.191979, 0.0761205, 0.200877, -0.258337, -1.09743, -1.57313, -1.176,
  -0.262677, 0.177856, -0.516658, -1.92127, -2.76822, -2.05383
}

};

uint32_t samples_serie = 0U;
uint32_t ping_id = 0U;
static float a_pingpong[2][HSP_BRAM_RSRC_SAMPLES_NBR];
static float a_pingpong_results[2][RFFT_RESULT_SIZE];

extern DMA_HandleTypeDef handle_GPDMA1_Channel9;
extern DMA_HandleTypeDef handle_GPDMA1_Channel1;
extern volatile uint32_t end_of_copy_ping_to_bramab;
extern volatile uint32_t end_of_copy_bramab_to_ping_result;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* Application functions prototypes *******/
extern void Error_Handler(void);
extern hsp_engine_context_t hmw;

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t MX_HSP_Engine_ProcessFIR_FFT(void)
{
  HAL_StatusTypeDef ret_hal;

  uint32_t dominant_frequencies[NB_DOMINANT_FREQUENCY] = {0U};
  uint32_t nb_dominant_fq = 0U;
  hsp_bram_resources_t *p_bram_rsrc = MX_HSP_BRAM_GetResources();

  do
  {
    /* Toggle the Green LED for processing */
    HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);

    /*--- Fill the Pong buffer with the next samples series during the DMA Transfer of Ping buffer in HSP BRAMAB ---*/
    uint32_t pong_id = (ping_id + 1) % 2;
    samples_serie = (samples_serie + 1) % 3; /* Select the samples to copy */

#if defined(USE_TRACE) && (USE_TRACE == 1)
    printf("--> Transfer next series to pong buffer: pong_id = %d | series  = %d\n", pong_id, samples_serie);
#endif /* USE_TRACE */
    memcpy(a_pingpong[pong_id], &(samples[samples_serie]), HSP_BRAM_RSRC_SAMPLES_NBR * sizeof(float));

    /* Wait the DMA Transfer Complete of Results from BRAMAB to SRAM */
    while (end_of_copy_bramab_to_ping_result == 0U);

#if defined(USE_TRACE) && (USE_TRACE == 1)
    printf("## End HSP Process (%d) of Ping buffer = %d\n", *(p_bram_rsrc->pl_call), ping_id);
    printf("## Start Process of Pong buffer = %d | series = %d\n", pong_id, samples_serie);
#endif /* USE_TRACE */

    /*---- Start the DMA Channel 9 to transfer the results from HSP BRAMAB to "pong" buffer reults ----*/
    /* The DMA will be triggered by the HSP Processing List with the HSP_TRGO_OUT0 trigger             */
    /* The DMA TC callback will increment the variable end_of_copy_bramab_to_ping_result               */
    end_of_copy_bramab_to_ping_result = 0U;
    ret_hal = HAL_DMA_Start_IT(&handle_GPDMA1_Channel9, (uint32_t)(p_bram_rsrc->p_results), (uint32_t)&(a_pingpong_results[pong_id]), RFFT_RESULT_SIZE * sizeof(float));
    if (ret_hal != HAL_OK)
    {
      return 1;
    }

    /*---- Start the DMA Channel 1 to transfer the samples from Pong Buffer to HSP BRAMAB ----*/
    /* When DMA TC is completed, the HSP Processing List will be executed                     */
    end_of_copy_ping_to_bramab = 0;
    ret_hal = HAL_DMA_Start_IT(&handle_GPDMA1_Channel1, (uint32_t)&(a_pingpong[pong_id]), (uint32_t)(uint32_t *)(p_bram_rsrc->p_buff_in), HSP_BRAM_RSRC_SAMPLES_NBR * sizeof(float));
    if (ret_hal != HAL_OK)
    {
      return 1;
    }

    /*---- Check the results of Ping Buffer results  during the HSP process the Pong Buffer ----*/
#if defined(USE_TRACE) && (USE_TRACE == 1)
    printf("## Check the Ping Buffer results (id = %d) during the HSP process the Pong Buffer (id = %d)\n", ping_id, pong_id);
#endif /* USE_TRACE */

    /* Retrieve the dominant frequencies */
    nb_dominant_fq = 0U;
    for (uint32_t idx = 0; idx < RFFT_RESULT_SIZE; idx++)
    {
      float b = a_pingpong_results[ping_id][idx];
      if ((b > 100))
      {
        /* Retrieve the RFFT Frequency related to the index */
        /* Because of HSP RFFT returned a buffer of complex value, idx shall be divide by 2 */
        /* f(k) = k * fs / N                              */
        dominant_frequencies[nb_dominant_fq] = (idx) * SAMPLING_FREQUENCY / HSP_BRAM_RSRC_SAMPLES_NBR;
        nb_dominant_fq++;
      }
    }

    /* Reset the results buffer to proof it will be fill the next time and then guarantee the validity of process */
    memset(a_pingpong_results[ping_id], 0, RFFT_RESULT_SIZE * sizeof(float));

#if defined(USE_TRACE) && (USE_TRACE == 1)
    printf("## Switch Ping<->Pong Buffer (Ping becomes Pong and vice versa): ping_id = %d\n", pong_id);
#endif /* USE_TRACE */

    ping_id = pong_id; /* Switch the Ping Buffer to the pong buffer */

    /* Check Expected Dominant Frequencies */
    if (nb_dominant_fq != 2U)
    {
      return 1;
    }

    if ((dominant_frequencies[0] != 750U) || (dominant_frequencies[1] != 3000U))
    {
      return 1;
    }
  }
  while (samples_serie != 0U);

  return 0;
}

uint32_t MX_HSP_Engine_InitiateProcess(void)
{
  HAL_StatusTypeDef ret_hal;
  hsp_bram_resources_t *p_bram_rsrc = MX_HSP_BRAM_GetResources();

#if defined(USE_TRACE) && (USE_TRACE == 1)
  printf("## Process: pingid = %d | series = %d\n", ping_id, samples_serie);
#endif /* USE_TRACE */
  memcpy(&(a_pingpong[ping_id]), samples[0], HSP_BRAM_RSRC_SAMPLES_NBR * sizeof(float));

  /*---- Start the DMA Channel 9 to transfer the results from HSP BRAMAB to "ping" buffer reults ----*/
  /* The DMA will be triggered by the HSP Processing List with the HSP_TRGO_OUT0 trigger             */
  /* The DMA TC callback will increment the variable end_of_copy_bramab_to_ping_result               */
  end_of_copy_bramab_to_ping_result = 0U;
  ret_hal = HAL_DMA_Start_IT(&handle_GPDMA1_Channel9, (uint32_t)(p_bram_rsrc->p_results), (uint32_t)&(a_pingpong_results[ping_id]), RFFT_RESULT_SIZE * sizeof(float));
  if (ret_hal != HAL_OK)
  {
    return 1;
  }

  /*---- Start the DMA Channel 1 to transfer the samples from Ping Buffer to HSP BRAMAB ----*/
  /* When DMA TC is completed, the HSP Processing List will be executed                     */
  end_of_copy_ping_to_bramab = 0;
  ret_hal = HAL_DMA_Start_IT(&handle_GPDMA1_Channel1, (uint32_t)&(a_pingpong[ping_id]), (uint32_t)(uint32_t *)(p_bram_rsrc->p_buff_in), HSP_BRAM_RSRC_SAMPLES_NBR * sizeof(float));
  if (ret_hal != HAL_OK)
  {
    return 1;
  }
  
  return 0;
}
/* USER CODE END 0 */
