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
#include "crypto_test.h"
    
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_PKA_Init(void);
static void MX_PKA_DeInit(void);
void print_buf(char* str, const uint8_t *buf, int size);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/**
 * @brief function to print data of a buffer
 */
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


/**
  * @brief PKA Initialization Function
  * @param None
  * @retval None
  */
void MX_PKA_Init(void)
{
  
  /* USER CODE BEGIN PKA_Init 0 */
  /* USER CODE END PKA_Init 0 */

  /* USER CODE BEGIN PKA_Init 1 */

  /* USER CODE END PKA_Init 1 */
  hpka.Instance = PKA;
  HAL_PKA_DeInit(&hpka);  
  if (HAL_PKA_Init(&hpka) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN PKA_Init 2 */

  /* USER CODE END PKA_Init 2 */

}

void MX_PKA_DeInit(void)
{

  HAL_PKA_DeInit(&hpka);
  /* Peripheral clock disable */
  __HAL_RCC_PKA_CLK_DISABLE(); 
}

/** -----------------------------------------
  * @section ECC TEST code using HAL API
  * -----------------------------------------
  */

/**
 * @brief function to run ECDSA sign test with P-256 curve using HAL API
 */
void ECDSA_SignTest_SECP256R1_HALAPI(void)
{
  PKA_ECDSASignInTypeDef in = {0};
  PKA_ECDSASignOutTypeDef out = {0};
  int result_ok = 0;
  
  printf("\r\n"BRIGHT_YELLOW"Test with direct call to HAL driver\r\n"RESET_COLOR);
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Sign with P-256 SHA256 ==> Start\r\n"RESET_COLOR);
  print_buf("HASH value for signing", (const uint8_t *)SigGen256_Hash_Msg, SigGen256_Hash_Msg_len);
  
  
  MX_PKA_Init();
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

  in.integer =         SigGen256_k;
  in.hash =            SigGen256_Hash_Msg;
  in.privateKey =      SigGen256_d;

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
  
  print_buf("Signature R", out.RSign, SigGen256_R_len);
  print_buf("Signature S", out.SSign, SigGen256_S_len);

  /* Compare to expected result */
  result_ok = 0;
  if (memcmp(out.RSign, SigGen256_R, SigGen256_R_len) != 0)
  {
    result_ok++;
    printf(BRIGHT_RED"Signature R data is NOT as expected\r\n"RESET_COLOR);  
  }
  if (memcmp(out.SSign, SigGen256_S, SigGen256_S_len) != 0)
  {
    result_ok++;
    printf(BRIGHT_RED"Signature S data is NOT as expected\r\n"RESET_COLOR);  
  }
  if(result_ok == 0)
  {
    printf(BRIGHT_GREEN"Signature data (r,s) data is as expected\r\n"RESET_COLOR);
  }

  /* Deinitialize the PKA */
  MX_PKA_DeInit();
  
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Sign with P-256 SHA256 <== Done.\r\n\r\n"RESET_COLOR); 
}


/**
 * @brief function to run ECDSA sign test with P-384 curve using HAL API
 */
void ECDSA_SignTest_SECP384R1_HALAPI(void)
{
  int result_ok = 0;
  PKA_ECDSASignInTypeDef in = {0};
  PKA_ECDSASignOutTypeDef out = {0};

  printf("\r\n"BRIGHT_YELLOW"Test with direct call to HAL driver\r\n"RESET_COLOR);
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Sign with P-384 SHA384 ==> Start\r\n"RESET_COLOR);
  print_buf("HASH value for signing", (const uint8_t *)SigGen384_Hash_Msg, SigGen384_Hash_Msg_len);
  
  
  MX_PKA_Init();
  /* Set input parameters */
  in.primeOrderSize =  prime384v1_Order_len;
  in.modulusSize =     prime384v1_Prime_len;
  in.coefSign =        prime384v1_A_sign;
  in.coef =            prime384v1_absA;
  in.coefB =           prime384v1_B;
  in.modulus =         prime384v1_Prime;
  in.basePointX =      prime384v1_GeneratorX;
  in.basePointY =      prime384v1_GeneratorY;
  in.primeOrder =      prime384v1_Order;

  in.integer =         SigGen384_k;
  in.hash =            SigGen384_Hash_Msg;
  in.privateKey =      SigGen384_d;

  /* Launch the verification */
  if(HAL_PKA_ECDSASign(&hpka, &in, 5000) != HAL_OK)
  {
    Error_Handler();
  }

  /* Allocate required space */
  out.RSign = malloc(prime384v1_Order_len);
  out.SSign = malloc(prime384v1_Order_len);
  if(out.RSign == NULL || out.SSign == NULL)
  {
    /* Not enough memory in heap */
    Error_Handler();
  }

  /* Copy the result to allocated space */
  HAL_PKA_ECDSASign_GetResult(&hpka , &out, NULL);
  
  print_buf("Signature R", out.RSign, SigGen384_R_len);
  print_buf("Signature S", out.SSign, SigGen384_S_len);

  /* Compare to expected result */
  result_ok = 0;
  if (memcmp(out.RSign, SigGen384_R, SigGen384_R_len) != 0)
  {
    result_ok++;
    printf(BRIGHT_RED"Signature R data is NOT as expected\r\n"RESET_COLOR); 
  }

  if (memcmp(out.SSign, SigGen384_S, SigGen384_S_len) != 0)
  {
    result_ok++;
    printf(BRIGHT_RED"Signature S data is NOT as expected\r\n"RESET_COLOR); 
  }
  printf(BRIGHT_GREEN"Signature data (r,s) data is as expected\r\n"RESET_COLOR);

  /* Deinitialize the PKA */
  if(HAL_PKA_DeInit(&hpka) != HAL_OK)
  {
    Error_Handler();
  }
  
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Sign with P-384 SHA384 <== Done.\r\n\r\n"RESET_COLOR); 
}


/**
 * @brief function to run ECDSA verify test with P-256 curve using HAL API
 */
void ECDSA_VerifyTest_SECP256R1_HALAPI(void)
{
  PKA_ECDSAVerifInTypeDef in = {0};
  
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Verify with P-256 SHA256 ==> Start\r\n"RESET_COLOR);
  
  MX_PKA_Init();
  
  /* Set input parameters */
  in.primeOrderSize =  prime256v1_Order_len;
  in.modulusSize =     prime256v1_Prime_len;
  in.coefSign =        prime256v1_A_sign;
  in.coef =            prime256v1_absA;
  in.modulus =         prime256v1_Prime;
  in.basePointX =      prime256v1_GeneratorX;
  in.basePointY =      prime256v1_GeneratorY;
  in.primeOrder =      prime256v1_Order;

  in.pPubKeyCurvePtX = SigVer256_Qx;
  in.pPubKeyCurvePtY = SigVer256_Qy;
  in.RSign =           SigVer256_R;
  in.SSign =           SigVer256_S;
  in.hash =            SigVer256_Hash_Msg;

  /* Launch the verification */
  if(HAL_PKA_ECDSAVerif(&hpka, &in, 5000) != HAL_OK)
  {
    Error_Handler();
  }

  printf("\r\n"BRIGHT_YELLOW"\r\n***Test with direct call to HAL driver with VALID hash\r\n"RESET_COLOR);
  print_buf("HASH value for signature verification", (const uint8_t *)in.hash, SigGen256_Hash_Msg_len);
  
  /* Compare to expected result */
  if(HAL_PKA_ECDSAVerif_IsValidSignature(&hpka) != SigVer256_Result)
  {
    printf(BRIGHT_RED"Verification failed! (NOT as expected)\r\n"RESET_COLOR);
  }
  else
  {
    printf(BRIGHT_GREEN"Verification OK (as expected)\r\n"RESET_COLOR);
  }
 
  printf("\r\n"BRIGHT_YELLOW"\r\n***Test with direct call to HAL driver with WRONG hash\r\n"RESET_COLOR);
  
  /* Simulate a wrong hash message verification */
  in.hash = SigVer256_Hash_Msg_False;
  print_buf("HASH value for signature verification", (const uint8_t *)in.hash, SigGen256_Hash_Msg_len);
  
  /* Launch the verification */
  if(HAL_PKA_ECDSAVerif(&hpka, &in, 5000) != HAL_OK)
  {
    Error_Handler();
  }

  /* Compare to expected result */
  if(HAL_PKA_ECDSAVerif_IsValidSignature(&hpka) != SigVer256_Result)
  {
    printf(BRIGHT_RED"Verification failed"BRIGHT_GREEN" (as expected)!\r\n"RESET_COLOR);
  }
  else
  {
    printf(BRIGHT_GREEN"Verification OK"BRIGHT_RED" (NOT as expected)!\r\n"RESET_COLOR);
  }  
  
  MX_PKA_DeInit();
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Verify with P-256 SHA256 <== Done.\r\n"RESET_COLOR);
}



/**
 * @brief function to run ECDSA verify test with P-384 curve using HAL API
 */
void ECDSA_VerifyTest_SECP384R1_HALAPI(void)
{
  PKA_ECDSAVerifInTypeDef in = {0};
  
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Verify with P-384 SHA384 ==> Start\r\n"RESET_COLOR);
  
  MX_PKA_Init();
  
  /* Set input parameters */
  in.primeOrderSize =  prime384v1_Order_len;
  in.modulusSize =     prime384v1_Prime_len;
  in.coefSign =        prime384v1_A_sign;
  in.coef =            prime384v1_absA;
  in.modulus =         prime384v1_Prime;
  in.basePointX =      prime384v1_GeneratorX;
  in.basePointY =      prime384v1_GeneratorY;
  in.primeOrder =      prime384v1_Order;

  in.pPubKeyCurvePtX = SigVer384_Qx;
  in.pPubKeyCurvePtY = SigVer384_Qy;
  in.RSign =           SigVer384_R;
  in.SSign =           SigVer384_S;
  in.hash =            SigVer384_Hash_Msg;

  /* Launch the verification */
  if(HAL_PKA_ECDSAVerif(&hpka, &in, 5000) != HAL_OK)
  {
    Error_Handler();
  }

  printf("\r\n"BRIGHT_YELLOW"\r\n***Test with direct call to HAL driver with VALID hash\r\n"RESET_COLOR);
  print_buf("HASH value for signature verification", (const uint8_t *)in.hash, SigGen384_Hash_Msg_len);
  
  /* Compare to expected result */
  if(HAL_PKA_ECDSAVerif_IsValidSignature(&hpka) != SigVer384_Result)
  {
    printf(BRIGHT_RED"Verification failed! (NOT as expected)\r\n"RESET_COLOR);
  }
  else
  {
    printf(BRIGHT_GREEN"Verification OK (as expected)\r\n"RESET_COLOR);
  }
 
  printf("\r\n"BRIGHT_YELLOW"\r\n***Test with direct call to HAL driver with WRONG hash\r\n"RESET_COLOR);
  
  /* Simulate a wrong hash message verification */
  in.hash = SigVer384_Hash_Msg_False;
  print_buf("HASH value for signature verification", (const uint8_t *)in.hash, SigGen384_Hash_Msg_len);
  
  /* Launch the verification */
  if(HAL_PKA_ECDSAVerif(&hpka, &in, 5000) != HAL_OK)
  {
    Error_Handler();
  }

  /* Compare to expected result */
  if(HAL_PKA_ECDSAVerif_IsValidSignature(&hpka) != SigVer384_Result)
  {
    printf(BRIGHT_RED"Verification failed"BRIGHT_GREEN" (as expected)!\r\n"RESET_COLOR);
  }
  else
  {
    printf(BRIGHT_GREEN"Verification OK"BRIGHT_RED" (NOT as expected)!\r\n"RESET_COLOR);
  }  
  
  MX_PKA_DeInit();
  
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Verify with P-384 SHA384 <== Done.\r\n"RESET_COLOR);
}

/** -----------------------------------------
  * @section ECC TEST code using CRYPTO API
  * -----------------------------------------
  */

/**
 * @brief function to run ECDSA sign test with P-256 curve using CYRPTO API
 */
void ECDSA_SignTest_SECP256R1_CRYPTOAPI(void)
{
  int result_ok = 0;
  
  printf("\r\n"BRIGHT_BLUE"Test ECDSA Sign with P-256 SHA256 ==> Start\r\n"RESET_COLOR);
  printf("\r\n"BRIGHT_BLUE"Test with call to crypto API\r\n"RESET_COLOR);
  
  printf(BRIGHT_BLUE"Test Crypto API ===> Signature generation\r\n"RESET_COLOR);
  INT8U outsig[64];
  ECDSA_Sign_SetK((uint8_t*)SigGen256_k);
  INT16U ret = Generate_ECDSA_With_SHA_Hash_Value(ECC_CURVE_SECP256R1, 
                                          (INT8U *)SigGen256_d, 
                                          (INT8U *)SigGen256_Hash_Msg, 
                                          &outsig[0]);
  if (ret == 0 )
  {
    result_ok = 0;
    print_buf("Output signature:", outsig, sizeof(outsig));
    /* Compare to expected result */
    if (memcmp(&outsig[0], SigGen256_R, SigGen256_R_len) != 0)
    {
      result_ok++;
      printf(BRIGHT_RED"Signature R data is NOT as expected\r\n"RESET_COLOR);      
    }

    if (memcmp(&outsig[32], SigGen256_S, SigGen256_S_len) != 0)
    {
      result_ok++;
      printf(BRIGHT_RED"Signature S data is NOT as expected\r\n"RESET_COLOR);
    }
    
    if(result_ok == 0)
    {
      printf(BRIGHT_GREEN"Signature data (r,s) data is as expected\r\n"RESET_COLOR);
    }
  }    
  else
  {
    printf(BRIGHT_RED"Call function Generate_ECDSA_With_SHA_Hash_Value returned error!\r\n"RESET_COLOR);
  }  
  
  printf("\r\n"BRIGHT_BLUE"Test ECDSA Sign with P-256 SHA256 <== Done.\r\n\r\n"RESET_COLOR);  
}

/**
 * @brief function to run ECDSA sign test with P-384 curve using CRYPTO API
 */
void ECDSA_SignTest_SECP384R1_CRYPTOAPI(void)
{
  int result_ok = 0;
  
  printf("\r\n"BRIGHT_BLUE"Test ECDSA Sign with P-384 SHA384 ==> Start\r\n"RESET_COLOR);
  printf("\r\n"BRIGHT_BLUE"Test with call to crypto API\r\n"RESET_COLOR);
  
  printf(BRIGHT_BLUE"Test Crypto API ===> Signature generation\r\n"RESET_COLOR);
  INT8U outsig[96];
  ECDSA_Sign_SetK((uint8_t*)SigGen384_k);
  INT16U ret = Generate_ECDSA_With_SHA_Hash_Value(ECC_CURVE_SECP384R1, 
                                          (INT8U *)SigGen384_d, 
                                          (INT8U *)SigGen384_Hash_Msg, 
                                          &outsig[0]);
  if (ret == 0 )
  {
    print_buf("Output signature:", outsig, sizeof(outsig));
    
    /* Compare to expected result */
    if (memcmp(&outsig[0], SigGen384_R, SigGen384_R_len) != 0)
    {
      result_ok++;
      printf(BRIGHT_RED"Signature R data is NOT as expected\r\n"RESET_COLOR);
    }

    if (memcmp(&outsig[48], SigGen384_S, SigGen384_S_len) != 0)
    {
      result_ok++;
      printf(BRIGHT_RED"Signature S data is NOT as expected\r\n"RESET_COLOR);
    }
    
    if (result_ok == 0)
    {
      printf(BRIGHT_GREEN"Signature data (r,s) data is as expected\r\n"RESET_COLOR);
    }
  }  
  else
  {
    printf(BRIGHT_RED"Call function Generate_ECDSA_With_SHA_Hash_Value returned error!\r\n"RESET_COLOR);
  }  
  
  printf("\r\n"BRIGHT_BLUE"Test ECDSA Sign with P-384 SHA384 <== Done.\r\n\r\n"RESET_COLOR);  
}

/**
 * @brief function to run ECDSA verify test with P-256 curve using CRYPO API
 */
void ECDSA_VerifyTest_SECP256R1_CRYPTOAPI(void)
{  
  printf("\r\n"BRIGHT_BLUE"\r\n***Test with call to crypto API\r\n"RESET_COLOR);
  
  uint8_t pubkey[64], sig[64];
  memcpy(&pubkey[0], SigVer256_Qx, 32);
  memcpy(&pubkey[32], SigVer256_Qy, 32);
  memcpy(&sig[0], SigVer256_R, 32);
  memcpy(&sig[32], SigVer256_S, 32);
  
  printf(BRIGHT_BLUE"\r\n***Test Crypto API ===> Verify signature with VALID hash\r\n"RESET_COLOR);
  print_buf("HASH value for signature verification", (const uint8_t *)SigVer256_Hash_Msg, SigGen256_Hash_Msg_len);
  if ( Verify_ECDSA_With_SHA_Hash_Value(ECC_CURVE_SECP256R1, pubkey, (uint8_t*)SigVer256_Hash_Msg, sig) == 0 )
  {
    printf(BRIGHT_GREEN"Verification OK (as expected)\r\n"RESET_COLOR);
  }
  else
  {
    printf(BRIGHT_RED"Verification failed! (NOT as expected)\r\n"RESET_COLOR);
  }
  
  printf(BRIGHT_BLUE"Test Crypto API ===> Verify signature with WRONG hash\r\n"RESET_COLOR);
  print_buf("HASH value for signature verification", (const uint8_t *)SigVer256_Hash_Msg_False, SigGen256_Hash_Msg_len);
  if ( Verify_ECDSA_With_SHA_Hash_Value(ECC_CURVE_SECP256R1, pubkey, (uint8_t*)SigVer256_Hash_Msg_False, sig) == 0 )
  {
    printf(BRIGHT_GREEN"Verification OK"BRIGHT_RED" (NOT as expected)!\r\n"RESET_COLOR);
  }
  else
  {
    printf(BRIGHT_RED"Verification failed"BRIGHT_GREEN" (as expected)!\r\n"RESET_COLOR);    
  }
  
  printf("\r\n"BRIGHT_BLUE"Test ECDSA Verify with P-256 SHA256 <== Done.\r\n"RESET_COLOR);
}

/**
 * @brief function to run ECDSA verify test with P-384 curve using CRYPTO API
 */
void ECDSA_VerifyTest_SECP384R1_CRYPTOAPI(void)
{
  printf("\r\n"BRIGHT_BLUE"\r\n***Test with call to crypto API\r\n"RESET_COLOR);
  
  uint8_t pubkey[96], sig[96];
  memcpy(&pubkey[0], SigVer384_Qx, 48);
  memcpy(&pubkey[48], SigVer384_Qy, 48);
  memcpy(&sig[0], SigVer384_R, 48);
  memcpy(&sig[48], SigVer384_S, 48);
  
  printf(BRIGHT_BLUE"\r\n***Test Crypto API ===> Verify signature with VALID hash\r\n"RESET_COLOR);
  print_buf("HASH value for signature verification", (const uint8_t *)SigVer384_Hash_Msg, SigGen384_Hash_Msg_len);
  if ( Verify_ECDSA_With_SHA_Hash_Value(ECC_CURVE_SECP384R1, pubkey, (uint8_t*)SigVer384_Hash_Msg, sig) == 0 )
  {
    printf(BRIGHT_GREEN"Verification OK (as expected)\r\n"RESET_COLOR);
  }
  else
  {
    printf(BRIGHT_RED"Verification failed! (NOT as expected)\r\n"RESET_COLOR);
  }
  
  printf(BRIGHT_BLUE"Test Crypto API ===> Verify signature with WRONG hash\r\n"RESET_COLOR);
  print_buf("HASH value for signature verification", (const uint8_t *)SigVer384_Hash_Msg_False, SigGen384_Hash_Msg_len);
  if ( Verify_ECDSA_With_SHA_Hash_Value(ECC_CURVE_SECP384R1, pubkey, (uint8_t*)SigVer384_Hash_Msg_False, sig) == 0 )
  {
    printf(BRIGHT_GREEN"Verification OK"BRIGHT_RED" (NOT as expected)!\r\n"RESET_COLOR);
  }
  else
  {
    printf(BRIGHT_RED"Verification failed"BRIGHT_GREEN" (as expected)!\r\n"RESET_COLOR);    
  }
  
  printf("\r\n"BRIGHT_BLUE"Test ECDSA Verify with P-384 SHA384 <== Done.\r\n"RESET_COLOR);
}


/**
 * @brief function to run ECC key generation test 
 */
void ECC_KeyGenTest_CRYPTOAPI(void)
{
  INT16U ret;
  
  uint8_t priv256[32];
  uint8_t pub256[64];
  uint8_t sig256[64];
  
  uint8_t priv384[48];
  uint8_t pub384[96];
  uint8_t sig384[96];
  
  memset(priv256, 0, sizeof(priv256));
  memset(pub256, 0, sizeof(pub256));
  memset(sig256, 0, sizeof(sig256));
  memset(priv384, 0, sizeof(priv384));
  memset(pub384, 0, sizeof(pub384));
  memset(sig384, 0, sizeof(sig384));
  
  printf("\r\n"BRIGHT_BLUE"Test ECC key gen with CRYPTO API\r\n"RESET_COLOR);
  printf("\r\n"BRIGHT_BLUE"Test ECC Key Gen P-256 ==> Start\r\n"RESET_COLOR);
  ret = ECDH_Generate_Key_Pair_Check_Key_Pair(ECC_CURVE_SECP256R1, priv256, pub256);
  if ( ret == 0 )
  {
    printf(BRIGHT_GREEN"Generate key OK!"RESET_COLOR);
    print_buf("P-256 Private key", priv256, sizeof(priv256));
    print_buf("P-256 Public key", pub256, sizeof(pub256));
    
    printf(BRIGHT_BLUE"Sign message hash with private key\r\n"RESET_COLOR);
    
    /* Test message hash sign and verify with the key pair */
    ret = Generate_ECDSA_With_SHA_Hash_Value(ECC_CURVE_SECP256R1, priv256, (INT8U *)SigVer256_Hash_Msg, sig256);
    if ( ret == 0 )
    {
      printf(BRIGHT_GREEN"Hash sign OK!\r\n"RESET_COLOR);
      print_buf("ECC 256 signature:", sig256, sizeof(sig256));
      
      printf(BRIGHT_BLUE"Verify message hash with public key\r\n"RESET_COLOR);
      ECDSA_Sign_SetK(NULL);
      ret = Verify_ECDSA_With_SHA_Hash_Value(ECC_CURVE_SECP256R1, pub256, (INT8U *)SigVer256_Hash_Msg, sig256);
      if ( ret == 0 )
      {
        printf(BRIGHT_GREEN"Signature verification OK!\r\n"RESET_COLOR);
      }
      else
      {
        printf(BRIGHT_RED"Signature verification FAILED!\r\n"RESET_COLOR);
      }
    }
    else
    {
      printf(BRIGHT_RED"Hash sign FAILED!"RESET_COLOR);
    }    
  }
  else
  {
    printf(BRIGHT_RED"Generate key failed!"RESET_COLOR);
  }
  
  printf("\r\n"BRIGHT_BLUE"Test ECC Key Gen P-256 <== Done.\r\n"RESET_COLOR);
  
  printf("\r\n\r\n"BRIGHT_BLUE"Test ECC Key Gen P-384 ==> Start\r\n"RESET_COLOR);
  ret = ECDH_Generate_Key_Pair_Check_Key_Pair(ECC_CURVE_SECP384R1, priv384, pub384);
  if ( ret == 0 )
  {
    printf(BRIGHT_GREEN"Generate key OK!"RESET_COLOR);
    print_buf("P-384 Private key", priv384, sizeof(priv384));
    print_buf("P-384 Public key", pub384, sizeof(pub384));
    
    printf(BRIGHT_BLUE"Sign message hash with private key\r\n"RESET_COLOR);
    
    /* Test message hash sign and verify with the key pair */
    ret = Generate_ECDSA_With_SHA_Hash_Value(ECC_CURVE_SECP384R1, priv384, (INT8U *)SigVer384_Hash_Msg, sig384);
    if ( ret == 0 )
    {
      printf(BRIGHT_GREEN"Hash sign OK!\r\n"RESET_COLOR);
      print_buf("ECC 384 signature:", sig384, sizeof(sig384));
      
      printf(BRIGHT_BLUE"Verify message hash with public key\r\n"RESET_COLOR);
      ECDSA_Sign_SetK(NULL);
      ret = Verify_ECDSA_With_SHA_Hash_Value(ECC_CURVE_SECP384R1, pub384, (INT8U *)SigVer384_Hash_Msg, sig384);
      if ( ret == 0 )
      {
        printf(BRIGHT_GREEN"Signature verification OK!\r\n"RESET_COLOR);        
      }
      else
      {
        printf(BRIGHT_RED"Signature verification FAILED!\r\n"RESET_COLOR);
      }
    }
    else
    {
      printf(BRIGHT_RED"Hash sign FAILED!"RESET_COLOR);
    } 
  }
  else
  {
    printf(BRIGHT_RED"Generate key failed!"RESET_COLOR);
  }
  
  printf("\r\n"BRIGHT_BLUE"Test ECC Key Gen P-384 <== Done.\r\n"RESET_COLOR);  
}

/**
 * @brief function to run ECDH shared secret establishment test 
 */
void ECDHTest_CRYPTOAPI(void)
{
  INT16U ret;
  INT8U shared_secret[48];
  
  /**
    * Test ECDH P-256 curve
    */
  printf("\r\n"BRIGHT_BLUE"Test ECDH P-256 ==> Start\r\n"RESET_COLOR);
  ret = ECDH_Compute_Z(ECC_CURVE_SECP256R1, (INT8U *)ECDH_256_Private_Key, (INT8U *)ECDH_256_Remote_Public_Key, shared_secret);
  if ( ret == 0 )
  {
    printf("\r\n"BRIGHT_GREEN"Generate shared secret OK!\r\n"RESET_COLOR);
    print_buf("Expected secret value:", ECDH_256_Expected_SecretX, ECDH_256_Expected_SecretX_len);
    print_buf("Generated secret value:", shared_secret, ECDH_256_Expected_SecretX_len);
    
    /* Compare the result with the expected value */
    if ( memcmp(shared_secret, ECDH_256_Expected_SecretX, ECDH_256_Expected_SecretX_len) == 0)
    {
      printf("\r\n"BRIGHT_GREEN"Generated shared secret value is correct!\r\n"RESET_COLOR);
    }
    else
    {
      printf("\r\n"BRIGHT_RED"Generated shared secret value is not as expected!\r\n"RESET_COLOR);
    }
  }
  else
  {
    printf("\r\n"BRIGHT_RED"Generate shared secret failed!\r\n"RESET_COLOR);
  }
  printf("\r\n"BRIGHT_BLUE"Test ECDH P-256 <== Done.\r\n"RESET_COLOR);
  
  /**
  * Test ECDH P-384 curve
  */
  printf("\r\n"BRIGHT_BLUE"Test ECDH P-384 ==> Start\r\n"RESET_COLOR);
  ret = ECDH_Compute_Z(ECC_CURVE_SECP384R1, (INT8U *)ECDH_384_Private_Key, (INT8U *)ECDH_384_Remote_Public_Key, shared_secret);
  if ( ret == 0 )
  {
    printf("\r\n"BRIGHT_GREEN"Generate shared secret OK!\r\n"RESET_COLOR);
    print_buf("Expected secret value:", ECDH_384_Expected_SecretX, ECDH_384_Expected_SecretX_len);
    print_buf("Generated secret value:", shared_secret, ECDH_384_Expected_SecretX_len);
    /* Compare the result with the expected value */
    if ( memcmp(shared_secret, ECDH_384_Expected_SecretX, ECDH_384_Expected_SecretX_len) == 0)
    {
      printf("\r\n"BRIGHT_GREEN"Generated shared secret value is correct!\r\n"RESET_COLOR);
    }
    else
    {
      printf("\r\n"BRIGHT_RED"Generated shared secret value is not as expected!\r\n"RESET_COLOR);
    }
  }
  else
  {
    printf("\r\n"BRIGHT_RED"Generate shared secret failed!\r\n"RESET_COLOR);
  }
  printf("\r\n"BRIGHT_BLUE"Test ECDH P-384 <== Done.\r\n"RESET_COLOR);
}

static void Print_ECC_Test_Menu(void)
{
  printf("\r\n===========================================================\r\n");
  printf("            ECC Test Menu\r\n");
  printf("===========================================================\r\n");
  printf(BRIGHT_YELLOW"ECDSA Sign Test (P-256) with HAL API    ------------------1\r\n"RESET_COLOR);
  printf(BRIGHT_YELLOW"ECDSA Sign Test (P-384) with HAL API    ------------------2\r\n"RESET_COLOR);
  printf(BRIGHT_YELLOW"ECDSA Verify Test (P-256) with HAL API    ----------------3\r\n"RESET_COLOR);
  printf(BRIGHT_YELLOW"ECDSA Verify Test (P-384) with HAL API    ----------------4\r\n"RESET_COLOR);
  
  printf(BRIGHT_BLUE"\r\nECDSA Sign Test (P-256) with CRYPTO API ------------------5\r\n"RESET_COLOR);
  printf(BRIGHT_BLUE"ECDSA Sign Test (P-384) with CRYPTO API ------------------6\r\n"RESET_COLOR);
  printf(BRIGHT_BLUE"ECDSA Verify Test (P-256) with CRYPTO API ----------------7\r\n"RESET_COLOR);
  printf(BRIGHT_BLUE"ECDSA Verify Test (P-384) with CRYPTO API ----------------8\r\n"RESET_COLOR);
  
  printf(BRIGHT_BLUE"ECC Key generation Test CRYPTO API    --------------------9\r\n"RESET_COLOR);
  printf(BRIGHT_BLUE"ECDH Secret generation Test with CRYPTO API --------------0\r\n"RESET_COLOR);
  
  printf("\r\nRun all cases --------------------------------------------a\r\n");
  printf(BRIGHT_YELLOW"Run all HAL API call cases -------------------------------h\r\n"RESET_COLOR);
  printf(BRIGHT_BLUE"Run all CRYPTO API call cases ----------------------------c\r\n"RESET_COLOR);
  printf("\r\nExit from test menu --------------------------------------x\r\n");
  
  printf("===========================================================\r\n");
  printf("Please enter your choice:\r\n");
}

void ECC_Test_Run(UART_HandleTypeDef *phuart)
{
  int loop=1;
  char c = 0;
  
  while(loop == 1)
  {
    Print_ECC_Test_Menu();
    if (HAL_UART_Receive(phuart, &c, 1, HAL_MAX_DELAY) == HAL_OK)
    {
      printf("Your input is: %c\r\n", c);
      switch(c)
      {
      case '1':   ECDSA_SignTest_SECP256R1_HALAPI();
        break;
      case '2':   ECDSA_SignTest_SECP384R1_HALAPI();
        break;
      case '3':   ECDSA_VerifyTest_SECP256R1_HALAPI();
        break;
      case '4':   ECDSA_VerifyTest_SECP384R1_HALAPI();
        break;
      case '5':   ECDSA_SignTest_SECP256R1_CRYPTOAPI();
        break;
      case '6':   ECDSA_SignTest_SECP384R1_CRYPTOAPI();
        break;
      case '7':   ECDSA_VerifyTest_SECP256R1_CRYPTOAPI();
        break;
      case '8':   ECDSA_VerifyTest_SECP384R1_CRYPTOAPI(); 
        break;
      case '9':   ECC_KeyGenTest_CRYPTOAPI();
        break;
      case '0':   ECDHTest_CRYPTOAPI();
        break;
      case 'a':   
        ECDSA_SignTest_SECP256R1_HALAPI();
        ECDSA_SignTest_SECP384R1_HALAPI();

        ECDSA_VerifyTest_SECP256R1_HALAPI();
        ECDSA_VerifyTest_SECP384R1_HALAPI();
        
        ECDSA_SignTest_SECP256R1_CRYPTOAPI();
        ECDSA_SignTest_SECP384R1_CRYPTOAPI();
        
        ECDSA_VerifyTest_SECP256R1_CRYPTOAPI();
        ECDSA_VerifyTest_SECP384R1_CRYPTOAPI();  
        
        ECC_KeyGenTest_CRYPTOAPI();
        ECDHTest_CRYPTOAPI();
        break;
      case 'h':   
        ECDSA_SignTest_SECP256R1_HALAPI();
        ECDSA_SignTest_SECP384R1_HALAPI();

        ECDSA_VerifyTest_SECP256R1_HALAPI();
        ECDSA_VerifyTest_SECP384R1_HALAPI();
        break;
      case 'c':   
        ECDSA_SignTest_SECP256R1_CRYPTOAPI();
        ECDSA_SignTest_SECP384R1_CRYPTOAPI();
        
        ECDSA_VerifyTest_SECP256R1_CRYPTOAPI();
        ECDSA_VerifyTest_SECP384R1_CRYPTOAPI();  
        
        ECC_KeyGenTest_CRYPTOAPI();
        ECDHTest_CRYPTOAPI();
        break;
      case 'x':   loop = 0;
        printf("Exit from ECC Test menu\r\n");
        break;      
      }
    }    
  }
}

