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
#ifndef __CRYPTO_H
#define __CRYPTO_H

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

#define INT8    int8_t    
#define INT8U   uint8_t
#define INT16   int16_t
#define INT16U  uint16_t

#define ECC_CURVE_SECP256R1       0
#define ECC_CURVE_SECP384R1       1
  
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

INT16U Generate_ECDSA_With_SHA_Hash_Value(INT8U Security_Control_Byte, 
                                          INT8U *Private_Key_Address, 
                                          INT8U *Input_Hash_Value_Address, 
                                          INT8U *Output_ECDSA_RAM_Address);

INT16U Verify_ECDSA_With_SHA_Hash_Value(INT8U Security_Control_Byte, 
                                        INT8U *Public_Key_Address, 
                                        INT8U *Input_Hash_Value_Address, 
                                        INT8U *Input_ECDSA_Address);


INT8U ECDH_Generate_Key_Pair_Check_Key_Pair(INT8U Security_Control_Byte, 
                                            INT8U *Private_Key_Address,
                                            INT8U *Public_Key_Address);

INT8U ECDH_Compute_Z(INT8U Security_Control_Byte, 
                     INT8U *Private_Key_Address, 
                     INT8U *Public_Key_Address, 
                     INT8U *Shared_Secret_Z_Ram);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __CRYPTO_H */
