/**
  ******************************************************************************
  * @file    crypto.c
  * @author  MCD Application Team
  * @brief   This file provides set of APIs to do ECC operations
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

/* Includes ------------------------------------------------------------------*/
#include "crypto.h"

/* Private variables ---------------------------------------------------------*/

/** 
 * @brief ECC Curve Parameters for SECP256R1 
 */
const uint8_t prime256v1_Prime[] = {
/*0x00,*/ 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff
};
const uint32_t prime256v1_Prime_len = 32;

const uint8_t prime256v1_A[] = {
/*0x00,*/ 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfc
};
/* PKA operation need abs(a) */
const uint8_t prime256v1_absA[] = {
/*0x00,*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03
};
const uint32_t prime256v1_A_len = 32;

/* PKA operation need the sign of A */
const uint32_t prime256v1_A_sign = 1;

const uint8_t prime256v1_B[] = {
    0x5a, 0xc6, 0x35, 0xd8, 0xaa, 0x3a, 0x93, 0xe7, 0xb3, 0xeb, 0xbd, 0x55, 0x76, 0x98, 0x86,
    0xbc, 0x65, 0x1d, 0x06, 0xb0, 0xcc, 0x53, 0xb0, 0xf6, 0x3b, 0xce, 0x3c, 0x3e, 0x27, 0xd2,
    0x60, 0x4b
};
const uint32_t prime256v1_B_len = 32;

const uint8_t prime256v1_Generator[] = {
    0x04, 0x6b, 0x17, 0xd1, 0xf2, 0xe1, 0x2c, 0x42, 0x47, 0xf8, 0xbc, 0xe6, 0xe5, 0x63, 0xa4,
    0x40, 0xf2, 0x77, 0x03, 0x7d, 0x81, 0x2d, 0xeb, 0x33, 0xa0, 0xf4, 0xa1, 0x39, 0x45, 0xd8,
    0x98, 0xc2, 0x96, 0x4f, 0xe3, 0x42, 0xe2, 0xfe, 0x1a, 0x7f, 0x9b, 0x8e, 0xe7, 0xeb, 0x4a,
    0x7c, 0x0f, 0x9e, 0x16, 0x2b, 0xce, 0x33, 0x57, 0x6b, 0x31, 0x5e, 0xce, 0xcb, 0xb6, 0x40,
    0x68, 0x37, 0xbf, 0x51, 0xf5
};
const uint32_t prime256v1_Generator_len = 65;

/* This buffer is extracted from prime256v1_Generator as its first part */
const uint8_t prime256v1_GeneratorX[] = {
    0x6b, 0x17, 0xd1, 0xf2, 0xe1, 0x2c, 0x42, 0x47, 0xf8, 0xbc, 0xe6, 0xe5, 0x63, 0xa4, 0x40, 
    0xf2, 0x77, 0x03, 0x7d, 0x81, 0x2d, 0xeb, 0x33, 0xa0, 0xf4, 0xa1, 0x39, 0x45, 0xd8, 0x98, 
    0xc2, 0x96
};
const uint32_t prime256v1_GeneratorX_len = 32;

/* This buffer is extracted from prime256v1_Generator as its second part */    
const uint8_t prime256v1_GeneratorY[] = {
    0x4f, 0xe3, 0x42, 0xe2, 0xfe, 0x1a, 0x7f, 0x9b, 0x8e, 0xe7, 0xeb, 0x4a, 0x7c, 0x0f, 0x9e, 
    0x16, 0x2b, 0xce, 0x33, 0x57, 0x6b, 0x31, 0x5e, 0xce, 0xcb, 0xb6, 0x40, 0x68, 0x37, 0xbf, 
    0x51, 0xf5
};
const uint32_t prime256v1_GeneratorY_len = 32;

const uint8_t prime256v1_Order[] = {
/*0x00,*/ 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xbc, 0xe6, 0xfa, 0xad, 0xa7, 0x17, 0x9e, 0x84, 0xf3, 0xb9, 0xca, 0xc2, 0xfc,
    0x63, 0x25, 0x51
};
const uint32_t prime256v1_Order_len = 32;

const uint32_t prime256v1_Cofactor = 1; /* (0x1) */

const uint8_t prime256v1_Seed[] = {
    0xc4, 0x9d, 0x36, 0x08, 0x86, 0xe7, 0x04, 0x93, 0x6a, 0x66, 0x78, 0xe1, 0x13, 0x9d, 0x26,
    0xb7, 0x81, 0x9f, 0x7e, 0x90
};
const uint32_t prime256v1_Seed_len = 20;

/** 
 * @brief ECC Curve Parameters for SECP384R1 
 */
const uint8_t prime384v1_Prime[] = {
/*0x00,*/     
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF
};
const uint32_t prime384v1_Prime_len = 48;

const uint8_t prime384v1_A[] = {
/*0x00,*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFC
};
/* PKA operation need abs(a) */
const uint8_t prime384v1_absA[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03
};
const uint32_t prime384v1_A_len = 48;

/* PKA operation need the sign of A */
const uint32_t prime384v1_A_sign = 1;

const uint8_t prime384v1_B[] = {
    0xB3, 0x31, 0x2F, 0xA7, 0xE2, 0x3E, 0xE7, 0xE4,
    0x98, 0x8E, 0x05, 0x6B, 0xE3, 0xF8, 0x2D, 0x19,
    0x18, 0x1D, 0x9C, 0x6E, 0xFE, 0x81, 0x41, 0x12,
    0x03, 0x14, 0x08, 0x8F, 0x50, 0x13, 0x87, 0x5A,
    0xC6, 0x56, 0x39, 0x8D, 0x8A, 0x2E, 0xD1, 0x9D,
    0x2A, 0x85, 0xC8, 0xED, 0xD3, 0xEC, 0x2A, 0xEF
};
const uint32_t prime384v1_B_len = 48;

const uint8_t prime384v1_Generator[] = {
    0x04, 0xAA, 0x87, 0xCA, 0x22, 0xBE, 0x8B, 0x05,
    0x37, 0x8E, 0xB1, 0xC7, 0x1E, 0xF3, 0x20, 0xAD,
    0x74, 0x6E, 0x1D, 0x3B, 0x62, 0x8B, 0xA7, 0x9B,
    0x98, 0x59, 0xF7, 0x41, 0xE0, 0x82, 0x54, 0x2A,
    0x38, 0x55, 0x02, 0xF2, 0x5D, 0xBF, 0x55, 0x29,
    0x6C, 0x3A, 0x54, 0x5E, 0x38, 0x72, 0x76, 0x0A,
    0xB7, 0x36, 0x17, 0xDE, 0x4A, 0x96, 0x26, 0x2C,
    0x6F, 0x5D, 0x9E, 0x98, 0xBF, 0x92, 0x92, 0xDC,
    0x29, 0xF8, 0xF4, 0x1D, 0xBD, 0x28, 0x9A, 0x14,
    0x7C, 0xE9, 0xDA, 0x31, 0x13, 0xB5, 0xF0, 0xB8,
    0xC0, 0x0A, 0x60, 0xB1, 0xCE, 0x1D, 0x7E, 0x81,
    0x9D, 0x7A, 0x43, 0x1D, 0x7C, 0x90, 0xEA, 0x0E,
    0x5F
};
const uint32_t prime384v1_Generator_len = 97; 

/* This buffer is extracted from prime384v1_Generator as its first part */
const uint8_t prime384v1_GeneratorX[] = {
    0xAA, 0x87, 0xCA, 0x22, 0xBE, 0x8B, 0x05,
    0x37, 0x8E, 0xB1, 0xC7, 0x1E, 0xF3, 0x20, 0xAD,
    0x74, 0x6E, 0x1D, 0x3B, 0x62, 0x8B, 0xA7, 0x9B,
    0x98, 0x59, 0xF7, 0x41, 0xE0, 0x82, 0x54, 0x2A,
    0x38, 0x55, 0x02, 0xF2, 0x5D, 0xBF, 0x55, 0x29,
    0x6C, 0x3A, 0x54, 0x5E, 0x38, 0x72, 0x76, 0x0A,
    0xB7,
};
const uint32_t prime384v1_GeneratorX_len = 48;

/* This buffer is extracted from prime384v1_Generator as its second part */    
const uint8_t prime384v1_GeneratorY[] = {
    0x36, 0x17, 0xDE, 0x4A, 0x96, 0x26, 0x2C,
    0x6F, 0x5D, 0x9E, 0x98, 0xBF, 0x92, 0x92, 0xDC,
    0x29, 0xF8, 0xF4, 0x1D, 0xBD, 0x28, 0x9A, 0x14,
    0x7C, 0xE9, 0xDA, 0x31, 0x13, 0xB5, 0xF0, 0xB8,
    0xC0, 0x0A, 0x60, 0xB1, 0xCE, 0x1D, 0x7E, 0x81,
    0x9D, 0x7A, 0x43, 0x1D, 0x7C, 0x90, 0xEA, 0x0E,
    0x5F
};
const uint32_t prime384v1_GeneratorY_len = 48;

const uint8_t prime384v1_Order[] = {
/*0x00,*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xC7, 0x63, 0x4D, 0x81, 0xF4, 0x37, 0x2D, 0xDF,
    0x58, 0x1A, 0x0D, 0xB2, 0x48, 0xB0, 0xA7, 0x7A,
    0xEC, 0xEC, 0x19, 0x6A, 0xCC, 0xC5, 0x29, 0x73
};
const uint32_t prime384v1_Order_len = 48;

const uint32_t prime384v1_Cofactor = 1; /* (0x1) */

const uint8_t prime384v1_Seed[] = {
    0xA3, 0x35, 0x92, 0x6A, 0xA3, 0x19, 0xA2, 0x7A,
    0x1D, 0x00, 0x89, 0x6A, 0x67, 0x73, 0xA4, 0x82,
    0x7A, 0xCD, 0xAC, 0x73
};
const uint32_t prime384v1_Seed_len = 20;

/* Private macros -----------------------------------------------*/
#define PKA_OPERATION_TIMEOUT (500)

/* Private function prototypes -----------------------------------------------*/
static INT8U *pECDSA_Sign_K = NULL;
static INT8U ECDSA_Sign_K[48] = {0};

/**
  * @brief Fill the buffer with random in size of bytes
  * @param words  number of words to be filled (4xbytes)
  * @retval 0 if succeed, otherwise failed
  *                  
*/
static INT16U fill_random(INT8U *buf, int words)
{
  HAL_StatusTypeDef status;
    
  RNG_HandleTypeDef hrng;
  hrng.Instance = RNG;
  hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;
  status = HAL_RNG_Init(&hrng);    

  if ( status == HAL_OK )
  {
    while ( words > 0 )
    {
      status |= HAL_RNG_GenerateRandomNumber(&hrng, (uint32_t*)buf + words-1);
      words--;
    }    
  }
  
  return status;
}

/**
  * @brief Get the pointer to the k value for ECDSA signing 
  *        If a pointer is set previously for k then use the existing 
  *        Otherwise fill the Array with random and return the pointer
  * @param size   384 for SECP384R1, 256 for SECP256R1
  * @retval pointer to the k buffer
  *                  
*/
static INT8U *get_k(INT8 size)
{  
  if (pECDSA_Sign_K != NULL) return pECDSA_Sign_K;
  else
  {
    // TODO get data from random number 
    if (size > 48) size = 12;
    else if (size < 48) size = 8;
    fill_random(ECDSA_Sign_K, size);
    return ECDSA_Sign_K;
  }
}

/* Public functions  -----------------------------------------------*/

/**
  * @brief Set k parameter for ECDSA signing. 
  *        If a value is given for k then ECDSA signature value is deterministic
  *        If using random value as k for signing, then it is undterministic signing
  * @param pK        pointer to the buffer holding k value for ECDSA sign
  *                  if NULL is given, ECDSA will use random number as k 
*/
void ECDSA_Sign_SetK(INT8U *pK)
{
  pECDSA_Sign_K = pK;
}

/**
  * @brief Compute the signature
  * @param ecc_curve        select secp256r1 or secp384r1
  * @param pPrivKey          pointer to the private key for signing (raw data)
  * @param pMsgHash     pointer to the hash value (raw data)
  * @param pSig     pointer to the output signature (raw data)
  * @retval  0 if succeed, else failed 
*/
INT16U Generate_ECDSA_With_SHA_Hash_Value(INT8U ecc_curve, 
                                          INT8U *pPrivKey, 
                                          INT8U *pMsgHash, 
                                          INT8U *pSig)
{
  PKA_HandleTypeDef hpka;
  INT16U ret = HAL_ERROR;
  PKA_ECDSASignInTypeDef in = {0};
  PKA_ECDSASignOutTypeDef out = {0};  
  
  /* check input parameter */
  if( pPrivKey == NULL ||
      pMsgHash == NULL ||
      pSig == NULL )
  {
    return ret;
  }
  
  if ( ecc_curve != ECC_CURVE_SECP256R1 && 
       ecc_curve != ECC_CURVE_SECP384R1)
  {
    return ret;
  }
  
  __HAL_RCC_PKA_CLK_ENABLE();
  __HAL_RCC_PKA_FORCE_RESET();
  /* Release PKA from reset state */
  __HAL_RCC_PKA_RELEASE_RESET();
  
  hpka.Instance = PKA;
  ret = HAL_PKA_Init(&hpka);
  if ( ret != HAL_OK)
  {
    return ret;
  }
  
  if ( ecc_curve == ECC_CURVE_SECP256R1 )
  {
    in.primeOrderSize =  prime256v1_Order_len;
    in.modulusSize =     prime256v1_Prime_len;
    in.coefSign =        prime256v1_A_sign;
    in.coef =            prime256v1_absA;
    in.coefB =           prime256v1_B;
    in.modulus =         prime256v1_Prime;
    in.basePointX =      prime256v1_GeneratorX;
    in.basePointY =      prime256v1_GeneratorY;
    in.primeOrder =      prime256v1_Order;
  }
  else
  {
    in.primeOrderSize =  prime384v1_Order_len;
    in.modulusSize =     prime384v1_Prime_len;
    in.coefSign =        prime384v1_A_sign;
    in.coef =            prime384v1_absA;
    in.coefB =           prime384v1_B;
    in.modulus =         prime384v1_Prime;
    in.basePointX =      prime384v1_GeneratorX;
    in.basePointY =      prime384v1_GeneratorY;
    in.primeOrder =      prime384v1_Order;    
  }
  in.integer =         get_k((ecc_curve == ECC_CURVE_SECP256R1 ? 32 : 48));
  in.hash =            pMsgHash;
  in.privateKey =      pPrivKey;
  
  /* Launch the verification */
  ret = HAL_PKA_ECDSASign(&hpka, &in, PKA_OPERATION_TIMEOUT);
  if( ret == HAL_OK)
  {
    out.RSign = pSig;
    out.SSign = pSig + (ecc_curve == ECC_CURVE_SECP256R1 ? 32 : 48);
    
    /* Copy the result to allocated space */
    HAL_PKA_ECDSASign_GetResult(&hpka , &out, NULL);
  }
    
  HAL_PKA_DeInit(&hpka);
  __HAL_RCC_PKA_FORCE_RESET();
  /* Release PKA from reset state */
  __HAL_RCC_PKA_RELEASE_RESET();
  /* Peripheral clock disable */
  __HAL_RCC_PKA_CLK_DISABLE();  
  
  return ret;
}

/**
  * @brief Verify the signature
  * @param ecc_curve        select secp256r1 or secp384r1
  * @param pPubKey           pointer to the publick key (raw data)
  * @param pMsgHash     pointer to the hash value (raw data)
  * @param pSig          pointer to the signature (raw data)
  * @retval  0 if succeed, else failed 
*/
INT16U Verify_ECDSA_With_SHA_Hash_Value(INT8U ecc_curve, 
                                        INT8U *pPubKey, 
                                        INT8U *pMsgHash, 
                                        INT8U *pSig)
{
  PKA_ECDSAVerifInTypeDef in = {0};
  PKA_HandleTypeDef hpka;
  INT16U ret = HAL_ERROR;
  
  /* check input parameter */
  if( pPubKey == NULL ||
      pMsgHash == NULL ||
      pSig == NULL )
  {
    return ret;
  }
  
  if ( ecc_curve != ECC_CURVE_SECP256R1 && 
       ecc_curve != ECC_CURVE_SECP384R1)
  {
    return ret;
  }
  
  __HAL_RCC_PKA_CLK_ENABLE();
  __HAL_RCC_PKA_FORCE_RESET();
  /* Release PKA from reset state */
  __HAL_RCC_PKA_RELEASE_RESET();
  
  hpka.Instance = PKA;
  ret = HAL_PKA_Init(&hpka);
  if ( ret != HAL_OK)
  {
    return ret;
  }
  
  if ( ecc_curve == ECC_CURVE_SECP256R1 )
  {    
    /* Set input parameters */
    in.primeOrderSize =  prime256v1_Order_len;
    in.modulusSize =     prime256v1_Prime_len;
    in.coefSign =        prime256v1_A_sign;
    in.coef =            prime256v1_absA;
    in.modulus =         prime256v1_Prime;
    in.basePointX =      prime256v1_GeneratorX;
    in.basePointY =      prime256v1_GeneratorY;
    in.primeOrder =      prime256v1_Order;

    in.pPubKeyCurvePtX = pPubKey;
    in.pPubKeyCurvePtY = pPubKey+32;
    in.RSign =           pSig;
    in.SSign =           pSig+32;
    in.hash =            pMsgHash;
  }
  else
  {
    /* Set input parameters */
    in.primeOrderSize =  prime384v1_Order_len;
    in.modulusSize =     prime384v1_Prime_len;
    in.coefSign =        prime384v1_A_sign;
    in.coef =            prime384v1_absA;
    in.modulus =         prime384v1_Prime;
    in.basePointX =      prime384v1_GeneratorX;
    in.basePointY =      prime384v1_GeneratorY;
    in.primeOrder =      prime384v1_Order;

    in.pPubKeyCurvePtX = pPubKey;
    in.pPubKeyCurvePtY = pPubKey+48;
    in.RSign =           pSig;
    in.SSign =           pSig+48;
    in.hash =            pMsgHash;
  }

  /* Launch the verification */
  ret = HAL_PKA_ECDSAVerif(&hpka, &in, PKA_OPERATION_TIMEOUT);
  if ( ret == HAL_OK )
  {
    /* Get the signature verification result */
    if( HAL_PKA_ECDSAVerif_IsValidSignature(&hpka) == SET)
    {
      ret = 0; // Signature verfication OK
    }
    else
    {
      ret = 1; // Signature verification failed
    }
  }
  
  HAL_PKA_DeInit(&hpka);
  __HAL_RCC_PKA_FORCE_RESET();
  /* Release PKA from reset state */
  __HAL_RCC_PKA_RELEASE_RESET();
  /* Peripheral clock disable */
  __HAL_RCC_PKA_CLK_DISABLE();  
  
  return ret;
}

/**
  * @brief Generate ECC key pair
  * @param ecc_curve        select secp256r1 or secp384r1
  * @param pPrivKey          pointer to the output private key
  * @param pPubKey           pointer to the output public key
  * @retval  0 if succeed, else failed 
*/
INT8U ECDH_Generate_Key_Pair_Check_Key_Pair(INT8U ecc_curve, 
                                            INT8U *pPrivKey,
                                            INT8U *pPubKey)
{
  PKA_ECCMulInTypeDef in = {0};
  PKA_ECCMulOutTypeDef out = {0};
  PKA_HandleTypeDef hpka;
  INT16U ret = HAL_ERROR;
  int keysize = 0;
  
  /* check input parameter */
  if( pPubKey == NULL ||
      pPrivKey == NULL )
  {
    return ret;
  }
  
  if ( ecc_curve != ECC_CURVE_SECP256R1 && 
       ecc_curve != ECC_CURVE_SECP384R1)
  {
    return ret;
  }
  
  /* First generate random number for private key */
  if ( ecc_curve == ECC_CURVE_SECP256R1 )
  {
    keysize = 32;
    in.scalarMulSize =  prime256v1_Order_len;
    in.modulusSize =     prime256v1_Prime_len;
    in.coefSign =        prime256v1_A_sign;
    in.coefA =            prime256v1_absA;
    in.coefB =           prime256v1_B;
    in.modulus =         prime256v1_Prime;
    in.pointX =      prime256v1_GeneratorX;
    in.pointY =      prime256v1_GeneratorY;
    in.primeOrder =      prime256v1_Order;
    
    out.ptX = pPubKey;
    out.ptY = pPubKey + 32;
  }
  else
  {
    keysize = 48;
    
    in.scalarMulSize =  prime384v1_Order_len;
    in.modulusSize =     prime384v1_Prime_len;
    in.coefSign =        prime384v1_A_sign;
    in.coefA =            prime384v1_absA;
    in.coefB =           prime384v1_B;
    in.modulus =         prime384v1_Prime;
    in.pointX =      prime384v1_GeneratorX;
    in.pointY =      prime384v1_GeneratorY;
    in.primeOrder =      prime384v1_Order; 
    
    out.ptX = pPubKey;
    out.ptY = pPubKey + 48;
  }
  
  ret = fill_random(pPrivKey, keysize/4);
  if ( ret == 0 )
  {
    /* make sure the highest bit of the private key is 1 to ensure the key length */
    pPrivKey[0] |= 0x80;
    
    /* set the scalarMul k */
    in.scalarMul = pPrivKey;
    
    /* Compute public key from private key */
    
    __HAL_RCC_PKA_CLK_ENABLE();
    __HAL_RCC_PKA_FORCE_RESET();
    /* Release PKA from reset state */
    __HAL_RCC_PKA_RELEASE_RESET();
    
    hpka.Instance = PKA;
    ret = HAL_PKA_Init(&hpka);
    if ( ret != HAL_OK)
    {
      return ret;
    }
    
    ret = HAL_PKA_ECCMul(&hpka, &in, PKA_OPERATION_TIMEOUT);
    if( ret == HAL_OK)
    {      
      HAL_PKA_ECCMul_GetResult(&hpka, &out);
    }
    
    HAL_PKA_DeInit(&hpka);
    __HAL_RCC_PKA_FORCE_RESET();
    /* Release PKA from reset state */
    __HAL_RCC_PKA_RELEASE_RESET();
    /* Peripheral clock disable */
    __HAL_RCC_PKA_CLK_DISABLE();  
  }
  
  return ret;
}
/**
  * @brief Establish secret with ECDH
  * @param ecc_curve        select secp256r1 or secp384r1
  * @param pPrivKey          pointer to the private key
  * @param pPubKey           pointer to the public key of peer 
  * @param pSharedSec          pointer to the output established secret
  * @retval  0 if succeed, else failed 
*/
INT8U ECDH_Compute_Z(INT8U ecc_curve, 
                     INT8U *pPrivKey, 
                     INT8U *pPubKey, 
                     INT8U *pSharedSec)
{
}
