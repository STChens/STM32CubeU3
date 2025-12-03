/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    PKA/PKA_ECDSA_Sign/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use PKA through
  *          the STM32U3xx HAL API.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "crypto.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

PKA_HandleTypeDef hpka;

RNG_HandleTypeDef hrng;

/* USER CODE BEGIN PV */
PKA_ECDSASignInTypeDef in = {0};
PKA_ECDSASignOutTypeDef out = {0};
__IO uint32_t operationComplete = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_RNG_Init(void);
static void MX_PKA_Init(void);
static void MX_ICACHE_Init(void);

static void print_buf(char* str, const uint8_t *buf, int size);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void print_buf(char* str, const uint8_t *buf, int size)
{
  int i;
  
  printf("\r\n-----------------------------------------------\r\n");
  if(str != NULL)
  {
    printf("%s\r\n", str);
    printf("-----------------------------------------------\r\n");
  }
  for(i = 0; i<size; i++)
  {
    printf("%02x ", buf[i]);
    if((i+1)%16 == 0)
    {
      printf("\r\n");
    }
  }
  if ( i % 16 != 0) printf("\r\n");
  printf("===============================================\r\n");
  
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* STM32U3xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Configure LD2 */
  BSP_LED_Init(LD2);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_RNG_Init();
  MX_PKA_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */
  
  COM_InitTypeDef COM_Init;
  COM_Init.BaudRate = 115200;
  COM_Init.HwFlowCtl = COM_HWCONTROL_NONE;
  COM_Init.Parity = COM_PARITY_NONE;
  COM_Init.StopBits = COM_STOPBITS_1;
  COM_Init.WordLength = COM_WORDLENGTH_8B;
  BSP_COM_Init(COM1, &COM_Init);
  
  printf("COM Init done.\r\n");
  
  /* Set input parameters */
  in.primeOrderSize =  prime256v1_Order_len;
  in.modulusSize =     prime256v1_Prime_len;
  in.coefSign =        prime256v1_A_sign;
  in.coef =            prime256v1_absA;
  in.coefB =           prime256v1_B;
  in.modulus =         prime256v1_Prime;
  in.basePointX =      prime256v1_GeneratorX;
  in.basePointY =      prime256v1_GeneratorY;
  in.primeOrder =      prime256v1_Order;

  in.integer =         SigGen_k;
  in.hash =            SigGen_Hash_Msg;
  in.privateKey =      SigGen_d;

  /* Launch the verification */
  if(HAL_PKA_ECDSASign(&hpka, &in, 5000) != HAL_OK)
  {
    Error_Handler();
  }

  /* Allocate required space */
  out.RSign = malloc(prime256v1_Order_len);
  out.SSign = malloc(prime256v1_Order_len);
  if(out.RSign == NULL || out.SSign == NULL)
  {
    /* Not enough memory in heap */
    Error_Handler();
  }

  /* Copy the result to allocated space */
  HAL_PKA_ECDSASign_GetResult(&hpka , &out, NULL);
  
  print_buf("HASH value", (const uint8_t *)SigGen_Hash_Msg, SigGen_Hash_Msg_len);
  print_buf("Signature R", out.RSign, SigGen_R_len);
  print_buf("Signature S", out.SSign, SigGen_S_len);

  /* Compare to expected result */
  if (memcmp(out.RSign, SigGen_R, SigGen_R_len) != 0)
  {
    Error_Handler();
  }

  if (memcmp(out.SSign, SigGen_S, SigGen_S_len) != 0)
  {
    Error_Handler();
  }

  /* Deinitialize the PKA */
  if(HAL_PKA_DeInit(&hpka) != HAL_OK)
  {
    Error_Handler();
  }

  /* Success */
  operationComplete = 1;
  BSP_LED_On(LD2);
  
  /* USER CODE END 2 */
  printf("Test Crypto API ===> Signature generation\r\n");
  INT8U outsig[32];
  INT16U ret = Generate_ECDSA_With_SHA_Hash_Value(ECC_CURVE_SECP256R1, 
                                          (INT8U *)SigGen_d, 
                                          (INT8U *)SigGen_Hash_Msg, 
                                          &outsig[0]);
  if (ret == 0 )
  {
    print_buf("Output signature:", outsig, sizeof(outsig));
  }  
  else
  {
    printf("Call function Generate_ECDSA_With_SHA_Hash_Value returned error!\r\n");
  }

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Enable Epod Booster
  */
  if (HAL_RCCEx_EpodBoosterClkConfig(RCC_EPODBOOSTER_SOURCE_MSIS, RCC_EPODBOOSTER_DIV1) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_PWREx_EnableEpodBooster() != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Set Flash latency before increasing MSIS
  */
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_MSIS;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief PKA Initialization Function
  * @param None
  * @retval None
  */
static void MX_PKA_Init(void)
{

  /* USER CODE BEGIN PKA_Init 0 */
  __HAL_RCC_PKA_CLK_ENABLE();
  /* USER CODE END PKA_Init 0 */

  /* USER CODE BEGIN PKA_Init 1 */

  /* USER CODE END PKA_Init 1 */
  hpka.Instance = PKA;
  if (HAL_PKA_Init(&hpka) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN PKA_Init 2 */

  /* USER CODE END PKA_Init 2 */

}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  operationComplete = 2;
  while (1)
  {
    /* Error if LD2 is slowly blinking (1 sec. period) */
    BSP_LED_Toggle(LD2);
    HAL_Delay(1000);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
