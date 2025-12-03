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
#define BRIGHT_RED "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[94m"
#define RESET_COLOR "\033[0m"
    
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

PKA_HandleTypeDef hpka;

RNG_HandleTypeDef hrng;

/* USER CODE BEGIN PV */

__IO uint32_t operationComplete = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_RNG_Init(void);
static void MX_PKA_Init(void);
static void MX_PKA_DeInit(void);
static void MX_ICACHE_Init(void);

static void print_buf(char* str, const uint8_t *buf, int size);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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
 * @brief function to run ECDSA sign test with P-256 curve
 */
static void ECDSA_SignTest_SECP256R1(void)
{
  PKA_ECDSASignInTypeDef in = {0};
  PKA_ECDSASignOutTypeDef out = {0};
  int result_ok = 0;
  
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Sign with P-256 SHA256 ==> Start\r\n"RESET_COLOR);
  print_buf("HASH value for signing", (const uint8_t *)SigGen256_Hash_Msg, SigGen256_Hash_Msg_len);
  
  printf("\r\n"BRIGHT_BLUE"Test with direct call to HAL driver\r\n"RESET_COLOR);
  
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
  
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Sign with P-256 SHA256 <== Done.\r\n\r\n"RESET_COLOR);  
}

/**
 * @brief function to run ECDSA sign test with P-384 curve
 */
static void ECDSA_SignTest_SECP384R1(void)
{
  int result_ok = 0;
  PKA_ECDSASignInTypeDef in = {0};
  PKA_ECDSASignOutTypeDef out = {0};

  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Sign with P-384 SHA384 ==> Start\r\n"RESET_COLOR);
  print_buf("HASH value for signing", (const uint8_t *)SigGen384_Hash_Msg, SigGen384_Hash_Msg_len);
  
  printf("\r\n"BRIGHT_BLUE"Test with direct call to HAL driver\r\n"RESET_COLOR);
  
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
  
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Sign with P-384 SHA384 <== Done.\r\n\r\n"RESET_COLOR);  
}

/**
 * @brief function to run ECDSA verify test with P-256 curve
 */
static void ECDSA_VerifyTest_SECP256R1(void)
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

  printf("\r\n"BRIGHT_BLUE"\r\n***Test with direct call to HAL driver with VALID hash\r\n"RESET_COLOR);
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
 
  printf("\r\n"BRIGHT_BLUE"\r\n***Test with direct call to HAL driver with WRONG hash\r\n"RESET_COLOR);
  
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
  
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Verify with P-256 SHA256 <== Done.\r\n"RESET_COLOR);
}

/**
 * @brief function to run ECDSA verify test with P-384 curve
 */
static void ECDSA_VerifyTest_SECP384R1(void)
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

  printf("\r\n"BRIGHT_BLUE"\r\n***Test with direct call to HAL driver with VALID hash\r\n"RESET_COLOR);
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
 
  printf("\r\n"BRIGHT_BLUE"\r\n***Test with direct call to HAL driver with WRONG hash\r\n"RESET_COLOR);
  
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
  
  printf("\r\n"BRIGHT_YELLOW"Test ECDSA Verify with P-384 SHA384 <== Done.\r\n"RESET_COLOR);
}

/**
 * @brief function to run ECC key generation test 
 */
static void ECC_KeyGen(void)
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
  
  printf("\r\n"BRIGHT_YELLOW"Test ECC Key Gen P-256 ==> Start\r\n"RESET_COLOR);
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
  
  printf("\r\n"BRIGHT_YELLOW"Test ECC Key Gen P-256 <== Done.\r\n"RESET_COLOR);
  
  printf("\r\n"BRIGHT_YELLOW"Test ECC Key Gen P-384 ==> Start\r\n"RESET_COLOR);
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
  
  printf("\r\n"BRIGHT_YELLOW"Test ECC Key Gen P-384 <== Done.\r\n"RESET_COLOR);
  
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
  
  ECDSA_SignTest_SECP256R1();
  ECDSA_SignTest_SECP384R1();
  
  ECDSA_VerifyTest_SECP256R1();
  ECDSA_VerifyTest_SECP384R1();
  
  ECC_KeyGen();

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
  __HAL_RCC_PKA_FORCE_RESET();
  /* Release PKA from reset state */
  __HAL_RCC_PKA_RELEASE_RESET();
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

static void MX_PKA_DeInit(void)
{

  HAL_PKA_DeInit(&hpka);
  __HAL_RCC_PKA_FORCE_RESET();
  /* Release PKA from reset state */
  __HAL_RCC_PKA_RELEASE_RESET();
  /* Peripheral clock disable */
  __HAL_RCC_PKA_CLK_DISABLE(); 
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
  printf(BRIGHT_RED"%s:%d\r\n"RESET_COLOR, __FUNCTION__, __LINE__);
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
