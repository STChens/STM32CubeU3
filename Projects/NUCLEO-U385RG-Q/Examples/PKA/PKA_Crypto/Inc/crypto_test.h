/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    PKA/PKA_Crypto/Inc/crypto.h
  * @author  MCD Application Team
  * @brief   Header for crypto.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#ifndef __CRYPTO_TEST_H
#define __CRYPTO_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
  
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

void ECDSA_SignTest_SECP256R1_HALAPI(void);
void ECDSA_SignTest_SECP384R1_HALAPI(void);
void ECDSA_VerifyTest_SECP384R1_HALAPI(void);
void ECDSA_VerifyTest_SECP256R1_HALAPI(void);

void ECDSA_SignTest_SECP256R1_CRYPTOAPI(void);
void ECDSA_SignTest_SECP384R1_CRYPTOAPI(void);
void ECDSA_VerifyTest_SECP384R1_CRYPTOAPI(void);
void ECDSA_VerifyTest_SECP256R1_CRYPTOAPI(void);     

void ECC_KeyGenTest_CRYPTOAPI(void);
void ECDHTest_CRYPTOAPI(void);

void ECC_Test_Run(UART_HandleTypeDef *phuart);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __CRYPTO_TEST_H */
