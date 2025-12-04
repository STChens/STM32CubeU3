/**
  ******************************************************************************
  * @file    PKA/PKA_ECDSA_Crypto/Inc/SigVer_TestVector.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of SigVer_TestVector.c .
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
#ifndef __SIGVER_TV_H
#define __SIGVER_TV_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t SigVer256_Msg[];
extern const uint32_t SigVer256_Msg_len;
extern const uint8_t SigVer256_Hash_Msg[];
extern const uint8_t SigVer256_Hash_Msg_False[];
extern const uint32_t SigVer256_Hash_Msg_len;
extern const uint8_t SigVer256_Qx[];
extern const uint32_t SigVer256_Qx_len;
extern const uint8_t SigVer256_Qy[];
extern const uint32_t SigVer256_Qy_len;
extern const uint8_t SigVer256_R[];
extern const uint32_t SigVer256_R_len;
extern const uint8_t SigVer256_S[];
extern const uint32_t SigVer256_S_len;
extern const uint32_t SigVer256_Result;

extern const uint8_t SigVer384_Msg[];
extern const uint32_t SigVer384_Msg_len;
extern const uint8_t SigVer384_Hash_Msg[];
extern const uint8_t SigVer384_Hash_Msg_False[];
extern const uint32_t SigVer384_Hash_Msg_len;
extern const uint8_t SigVer384_Qx[];
extern const uint32_t SigVer384_Qx_len;
extern const uint8_t SigVer384_Qy[];
extern const uint32_t SigVer384_Qy_len;
extern const uint8_t SigVer384_R[];
extern const uint32_t SigVer384_R_len;
extern const uint8_t SigVer384_S[];
extern const uint32_t SigVer384_S_len;
extern const uint32_t SigVer384_Result;
#ifdef __cplusplus
}
#endif

#endif /* __SIGVER_TV_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
