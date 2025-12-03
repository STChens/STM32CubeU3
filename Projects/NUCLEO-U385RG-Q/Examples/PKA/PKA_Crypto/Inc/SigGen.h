/**
  ******************************************************************************
  * @file    PKA/PKA_ECDSA_Sign/Inc/SigGen.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of SigGen.c .
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIGGEN_H
#define __SIGGEN_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t  SigGen256_Msg[];
extern const uint32_t SigGen256_Msg_len;
extern const uint8_t  SigGen256_Hash_Msg[];
extern const uint32_t SigGen256_Hash_Msg_len;
extern const uint8_t  SigGen256_d[];
extern const uint32_t SigGen256_d_len;
extern const uint8_t  SigGen256_Qx[];
extern const uint32_t SigGen256_Qx_len;
extern const uint8_t  SigGen256_Qy[];
extern const uint32_t SigGen256_Qy_len;
extern const uint8_t  SigGen256_k[];
extern const uint32_t SigGen256_k_len;
extern const uint8_t  SigGen256_R[];
extern const uint32_t SigGen256_R_len;
extern const uint8_t  SigGen256_S[];
extern const uint32_t SigGen256_S_len;

extern const uint8_t  SigGen384_Msg[];
extern const uint32_t SigGen384_Msg_len;
extern const uint8_t  SigGen384_Hash_Msg[];
extern const uint32_t SigGen384_Hash_Msg_len;
extern const uint8_t  SigGen384_d[];
extern const uint32_t SigGen384_d_len;
extern const uint8_t  SigGen384_Qx[];
extern const uint32_t SigGen384_Qx_len;
extern const uint8_t  SigGen384_Qy[];
extern const uint32_t SigGen384_Qy_len;
extern const uint8_t  SigGen384_k[];
extern const uint32_t SigGen384_k_len;
extern const uint8_t  SigGen384_R[];
extern const uint32_t SigGen384_R_len;
extern const uint8_t  SigGen384_S[];
extern const uint32_t SigGen384_S_len;
#ifdef __cplusplus
}
#endif

#endif /* __SIGGEN_H */


