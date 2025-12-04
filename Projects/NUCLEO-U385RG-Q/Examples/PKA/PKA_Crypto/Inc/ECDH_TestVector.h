/**
  ******************************************************************************
  * @file    PKA/PKA_ECDSA_Crypto/Inc/ECDH_TestVector.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of ECDH_TestVector.c .
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ECDH_TV_H
#define __ECDH_TV_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t ECDH_256_Private_Key[];
extern const uint8_t ECDH_256_Remote_Public_Key[];
extern const uint8_t ECDH_256_Expected_SecretX[];

extern const uint32_t ECDH_256_Private_Key_len;
extern const uint32_t ECDH_256_Remote_Public_Key_len;
extern const uint32_t ECDH_256_Expected_SecretX_len;

extern const uint8_t ECDH_384_Private_Key[];
extern const uint8_t ECDH_384_Remote_Public_Key[];
extern const uint8_t ECDH_384_Expected_SecretX[];

extern const uint32_t ECDH_384_Private_Key_len;
extern const uint32_t ECDH_384_Remote_Public_Key_len;
extern const uint32_t ECDH_384_Expected_SecretX_len;

#ifdef __cplusplus
}
#endif

#endif /* __ECDH_TV_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
