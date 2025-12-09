/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* The size of the plaintext is in words */
#define PLAINTEXT_SIZE    4 /* Plaintext size in Words */

/* The key size must be 128 bits (16 bytes), 192 bits (24 bytes) or 256 bits (32 bytes) */
#define KEY_SIZE          4 /* Key size in word */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define BRIGHT_RED "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[96m"
#define RESET_COLOR "\033[0m"
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRYP_HandleTypeDef hcryp;
__ALIGN_BEGIN static const uint32_t pKeyAES[4] __ALIGN_END = {
                            0xC939CC13,0x397C1D37,0xDE6AE0E1,0xCB7C423C};
__ALIGN_BEGIN static const uint32_t pInitVectAES[4] __ALIGN_END = {
                            0xB3D8CC01,0x7CBB89B3,0x9E0F67E2,0x00000002};
__ALIGN_BEGIN static const uint32_t HeaderAES[4] __ALIGN_END = {
                            0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f};

/* USER CODE BEGIN PV */
uint32_t Plaintext[PLAINTEXT_SIZE] = {0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069};

uint32_t Ciphertext[4] = {0x93FE7D9E,0x9BFD1034,0x8A5606E5,0xCAFA7354};

uint32_t ExpectedTAG[4]={0x0032A1DC,0x85F1C978,0x6925A2E7,0x1D8272DD};

/* Used for storing the encrypted text */
uint32_t EncryptedText[PLAINTEXT_SIZE];

/* Used for storing the decrypted text */
uint32_t DecryptedText[PLAINTEXT_SIZE];

/* Used for storing the computed MAC (aTAG) */
uint32_t TAG[4];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_AES_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
 * @brief function to print data of a buffer
 */
static void print_buf(char* str, const uint8_t *buf, int size)
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

/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n,b,i)                            \
  do {                                                  \
    (n) = ( (uint32_t) (b)[(i)    ] << 24 )             \
          | ( (uint32_t) (b)[(i) + 1] << 16 )           \
          | ( (uint32_t) (b)[(i) + 2] <<  8 )           \
          | ( (uint32_t) (b)[(i) + 3]       );          \
  } while( 0 )
#endif /* !GET_UINT32_BE */

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n,b,i)                            \
  do {                                                  \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
  } while( 0 )
#endif /* !PUT_UINT32_BE */
    
#define ST_GCM_TIMEOUT    0xFFU
#define AES_GCM_MODE_ENCRYPTION 0x00
#define AES_GCM_MODE_DECRYPTION 0x01
    
/**
 * @brief function to compute AES GCM encryption/decryption and tag
 * @param mode          AES_GCM_MODE_ENCRYPTION for encryption, AES_GCM_MODE_DECRYPTION for decryption
 * @param pKey          pointer to the key buffer
 * @param key_size      key size in byte, must be 16 or 32
 * @param input         pointer to the input data for denc/dec
 * @param input_length  input data length in bytes
 * @param pIv           pointer to the iv buffer
 * @param iv_size       length of iv data in bytes, only 12 bytes IV is supported
 * @param pAuthData     pointer to additional data for authentication
 * @param auth_data_size        length of additional authentication data in bytes
 * @param output        pointer to the output data buffer
 *                      Assummption here is that the output buffer is big enough to hold the encrypted/decrypted data
 * @param pTag          pointer to the output tag buffer
 *                      Assumption here is that the tag buffer is big enough to hold 16 byte gcm tag
 */    
static uint32_t aes_gcm_encrypt(int mode,
                                uint8_t *pKey, size_t key_size, 
                                uint8_t *input, size_t input_length, 
                                uint8_t *pIv, size_t iv_size, 
                                uint8_t *pAuthData, size_t auth_data_size,
                                uint8_t *output,
                                uint8_t *pTag 
                                  )
{
  uint32_t ret = HAL_ERROR;
  CRYP_HandleTypeDef haes;
  int i;
  uint32_t keyword[8];
  uint32_t ivword[4];
  
  uint16_t wordnb = 0;          /* number of four data words */
  uint16_t wordlen = 0;         /* length (in bytes) of four data words */
  uint16_t in_datalen = 0;  /* length (in bytes) of processed data within input buffer */
  __ALIGN_BEGIN unsigned char work_buf[16] __ALIGN_END;
  uint16_t work_buf_len = 0;
  
  if(input == NULL || input_length == 0) return ret;
  if(pIv == NULL || iv_size != 12) return ret;
  if(pAuthData == NULL && auth_data_size > 0) return ret;
  if(pAuthData != NULL && auth_data_size == 0) return ret;
  if(output == NULL) return ret;
  
  /* check and set key */
  if(pKey == NULL) return ret;
  switch(key_size)
  {
  case 16: haes.Init.KeySize = CRYP_KEYSIZE_128B;
    break;
  case 32: haes.Init.KeySize = CRYP_KEYSIZE_256B;
    break;
  default: return ret;    
  }

  /* Format and fill AES key  */
  for (i = 0; i < (key_size / 4); i++)
  {
    GET_UINT32_BE(keyword[i], pKey, 4 * i);
  }
  
  haes.Init.pKey = keyword;
  haes.Init.KeyMode = CRYP_KEYMODE_NORMAL;
  
  /* Set IV with invert endianness */
  for (i = 0; i < iv_size / 4U; i++)
  {
    GET_UINT32_BE(ivword[i], pIv, 4 * i);
  }

  /* counter value must be set to 2 when processing the first block of payload */
  ivword[3] = 0x00000002;

  haes.Init.pInitVect = (uint32_t *)ivword;

  /* Do not Allow IV reconfiguration at every gcm update */
  haes.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ONCE;
  
  haes.Instance = AES;
  haes.Init.Algorithm = CRYP_AES_GCM_GMAC;  
  haes.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_BYTE;
  haes.Init.DataType = CRYP_BYTE_SWAP;
  
  if ( auth_data_size > 0 )
  {
    haes.Init.Header = (uint32_t *)pAuthData;
    haes.Init.HeaderSize = auth_data_size;
  }
  else
  {
    haes.Init.Header = NULL;
    haes.Init.HeaderSize = 0;
  }
  /* Additional Authentication Data in bytes unit */
  haes.Init.HeaderWidthUnit = CRYP_HEADERWIDTHUNIT_BYTE;
  
  HAL_CRYP_DeInit(&haes);
  /* Enable AES clock */
  __HAL_RCC_AES_CLK_ENABLE();
  ret = HAL_CRYP_Init(&haes);
  
  if ( ret != HAL_OK)
  {
    return ret;
  }   
    
  /* Calculate number of four data words */
  wordnb = input_length / 16U;

  /* if available, process them */
  if (wordnb)
  {
    /* Convert in bytes */
    wordlen = wordnb * 16U;

    if ( mode == AES_GCM_MODE_DECRYPTION )
    {
      ret = HAL_CRYP_Decrypt(&haes,
                           (uint32_t *)input,
                           wordlen,
                           (uint32_t *)output,
                           ST_GCM_TIMEOUT);
    }
    else
    {
      ret = HAL_CRYP_Encrypt(&haes,
                           (uint32_t *)input,
                           wordlen,
                           (uint32_t *)output,
                           ST_GCM_TIMEOUT);

    }
    if ( ret != HAL_OK)
    {
      goto exit;
    }    

    /* update total length */
    in_datalen += wordlen;

    if (in_datalen < input_length)
    {
      /* Process them into a last four data word */
      goto last_data_word;
    }
    else
    {
      goto finish;
    }
  }

last_data_word:
  /* Calculate remaining bytes */
  /* Can have a null length when payload is omitted (GMAC) */
  work_buf_len = (uint16_t)((input_length - in_datalen) % 16U);

  memset(work_buf, 0, sizeof(work_buf));
  memcpy(work_buf, input + in_datalen, work_buf_len);

  if ( mode == AES_GCM_MODE_DECRYPTION )
  {
    ret = HAL_CRYP_Decrypt(&haes,
                         (uint32_t *)work_buf,
                         work_buf_len,
                         (uint32_t *)(output + in_datalen),
                         ST_GCM_TIMEOUT);
  }
  else
  {
      ret = HAL_CRYP_Encrypt(&haes,
                       (uint32_t *)work_buf,
                       work_buf_len,
                       (uint32_t *)(output + in_datalen),
                       ST_GCM_TIMEOUT);
  }
  
  if ( ret != HAL_OK)
  {
    goto exit;
  }
  
finish:  
  if ( pTag != NULL )
  {
    ret = HAL_CRYPEx_AESGCM_GenerateAuthTAG(&haes,(uint32_t*)pTag, TIMEOUT_VALUE);
  }  
  
exit:
  HAL_CRYP_DeInit(&haes);
  
  return ret;
}
                                  
/**
 * @brief function to run AES GCM encryption test uisng NIST test vector
 */
static void aes_gcm_test1(void)
{
  /** Extract from NIST Special Publication 800-38D
    * gcmEncryptExtIV256.rsp
  [Keylen = 128]
  [IVlen = 96]
  [PTlen = 408]
  [AADlen = 384]
  [Taglen = 128]

  Count = 0
  Key = 463b412911767d57a0b33969e674ffe7845d313b88c6fe312f3d724be68e1fca
  IV = 611ce6f9a6880750de7da6cb
  PT = e7d1dcf668e2876861940e012fe52a98dacbd78ab63c08842cc9801ea581682ad54af0c34d0d7f6f59e8ee0bf4900e0fd85042
  AAD = 0a682fbc6192e1b47a5e0868787ffdafe5a50cead3575849990cdd2ea9b3597749403efb4a56684f0c6bde352d4aeec5
  CT = 8886e196010cb3849d9c1a182abe1eeab0a5f3ca423c3669a4a8703c0f146e8e956fb122e0d721b869d2b6fcd4216d7d4d3758
  Tag = 2469cecd70fd98fec9264f71df1aee9a
    */
  const uint8_t Key[] =
  {
    0x46, 0x3b, 0x41, 0x29, 0x11, 0x76, 0x7d, 0x57, 0xa0, 0xb3, 0x39, 0x69, 0xe6, 0x74, 0xff, 0xe7,
    0x84, 0x5d, 0x31, 0x3b, 0x88, 0xc6, 0xfe, 0x31, 0x2f, 0x3d, 0x72, 0x4b, 0xe6, 0x8e, 0x1f, 0xca
  };
  const uint8_t IV[] =
  {
    0x61, 0x1c, 0xe6, 0xf9, 0xa6, 0x88, 0x07, 0x50, 0xde, 0x7d, 0xa6, 0xcb
  };
  const uint8_t InputData[] =
  {
    0xe7, 0xd1, 0xdc, 0xf6, 0x68, 0xe2, 0x87, 0x68, 0x61, 0x94, 0x0e, 0x01, 0x2f, 0xe5, 0x2a, 0x98,
    0xda, 0xcb, 0xd7, 0x8a, 0xb6, 0x3c, 0x08, 0x84, 0x2c, 0xc9, 0x80, 0x1e, 0xa5, 0x81, 0x68, 0x2a,
    0xd5, 0x4a, 0xf0, 0xc3, 0x4d, 0x0d, 0x7f, 0x6f, 0x59, 0xe8, 0xee, 0x0b, 0xf4, 0x90, 0x0e, 0x0f,
    0xd8, 0x50, 0x42
  };
  const uint8_t AddData[] =
  {
    0x0a, 0x68, 0x2f, 0xbc, 0x61, 0x92, 0xe1, 0xb4, 0x7a, 0x5e, 0x08, 0x68, 0x78, 0x7f, 0xfd, 0xaf,
    0xe5, 0xa5, 0x0c, 0xea, 0xd3, 0x57, 0x58, 0x49, 0x99, 0x0c, 0xdd, 0x2e, 0xa9, 0xb3, 0x59, 0x77,
    0x49, 0x40, 0x3e, 0xfb, 0x4a, 0x56, 0x68, 0x4f, 0x0c, 0x6b, 0xde, 0x35, 0x2d, 0x4a, 0xee, 0xc5
  };
  const uint8_t Expected_Output[] =
  {
    0x88, 0x86, 0xe1, 0x96, 0x01, 0x0c, 0xb3, 0x84, 0x9d, 0x9c, 0x1a, 0x18, 0x2a, 0xbe, 0x1e, 0xea,
    0xb0, 0xa5, 0xf3, 0xca, 0x42, 0x3c, 0x36, 0x69, 0xa4, 0xa8, 0x70, 0x3c, 0x0f, 0x14, 0x6e, 0x8e,
    0x95, 0x6f, 0xb1, 0x22, 0xe0, 0xd7, 0x21, 0xb8, 0x69, 0xd2, 0xb6, 0xfc, 0xd4, 0x21, 0x6d, 0x7d,
    0x4d, 0x37, 0x58,
  };
  const uint8_t Expected_Tag[] =
  {
    0x24, 0x69, 0xce, 0xcd, 0x70, 0xfd, 0x98, 0xfe, 0xc9, 0x26, 0x4f, 0x71, 0xdf, 0x1a, 0xee, 0x9a
  };
  uint8_t output_data[51] = {0};
  uint8_t tag[16] = {0};
    
  printf(BRIGHT_YELLOW"\r\n\r\nAES GCM TEST CASE 1 ========> [Encryption test]\r\n\r\n"RESET_COLOR);

  print_buf("AES Key:", (uint8_t *)Key, 32);
  print_buf("IV data:", (uint8_t *)IV, sizeof(IV));
  print_buf("Auth data:", (uint8_t *)AddData, sizeof(AddData));
  print_buf("Plain text:", (uint8_t *)InputData, sizeof(InputData));

  aes_gcm_encrypt(AES_GCM_MODE_ENCRYPTION,
                  (uint8_t *)Key, sizeof(Key), (uint8_t *)InputData, sizeof(InputData), 
                  (uint8_t *)IV, sizeof(IV), (uint8_t *)AddData, sizeof(AddData), 
                  output_data, tag);
  
  print_buf("Expected cipher text:", (uint8_t *)Expected_Output, sizeof(Expected_Output));
  print_buf("Computed Cipher text:", (uint8_t *)output_data, sizeof(Expected_Output));
    
  /*Compare results with expected buffer*/
  if(memcmp(Expected_Output, output_data, sizeof(Expected_Output)) != 0)
  {
    /* Processing Error */
    printf(BRIGHT_RED"Computed output is NOT the same as expected output!\r\n"RESET_COLOR);
    printf(BRIGHT_RED"Test failed!\r\n"RESET_COLOR);
  }
  else
  {
    printf(BRIGHT_GREEN"Computed output is the same as expected output!\r\n"RESET_COLOR);
    printf(BRIGHT_GREEN"Test OK!\r\n"RESET_COLOR);    
  }
  print_buf("Expected GCM tag:", (uint8_t *)Expected_Tag, sizeof(Expected_Tag));
  print_buf("Computed GCM tag:", (uint8_t *)tag, sizeof(Expected_Tag));
  
  /*Compare results with expected buffer*/
  if(memcmp(Expected_Tag, tag, sizeof(Expected_Tag)) != 0)
  {
    /* Processing Error */
    printf(BRIGHT_RED"Computed tag is NOT the same as expected tag!\r\n"RESET_COLOR);
    printf(BRIGHT_RED"Test failed!\r\n"RESET_COLOR);
  }
  else
  {
    printf(BRIGHT_GREEN"Computed tag is the same as expected tag!\r\n"RESET_COLOR);
    printf(BRIGHT_GREEN"Test OK!\r\n"RESET_COLOR);    
  }
}

                                  
/**
 * @brief function to run AES GCM decryption test uisng CTM test vector
 */
static void aes_gcm_test2(void)
{
  const uint8_t Key[] =
  {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
  };
  const uint8_t IV[] =
  {
    0x57, 0x53, 0x45, 0x30, 0x30, 0x30, 0x30, 0x31, 0x00, 0x00, 0x00, 0x01
  };
  const uint8_t InputData[] =
  {
    0xFE, 0x64, 0x8D, 0x05, 0x03, 0xA5, 0x35, 0xAA, 0x18, 0x62, 0xBB, 0x58, 0x3A, 0x79
  };
  const uint8_t AddData[] =
  {
    0x30, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF
  };
  const uint8_t Expected_Output[] =
  {
    0x01, 0x00, 0x00, 0x00, 0x06, 0x5F, 0x1F, 0x04, 0x00, 0xFF, 0xFF, 0xFF, 0x04, 0xFD
  };
  const uint8_t Expected_Tag[] =
  {
    0x6A, 0x42, 0xEF, 0xA1, 0x61, 0x8B, 0x0F, 0x8F, 0x95, 0xC8, 0x6E, 0x2A, 0x85, 0x4F, 0x3B, 0xB5
  };
  uint8_t output_data[14] = {0};
  uint8_t tag[16] = {0};
  
  printf(BRIGHT_BLUE"\r\n\r\nAES GCM TEST CASE 2 ========> [Decryption test]\r\n\r\n"RESET_COLOR);
  
  print_buf("AES Key:", (uint8_t *)Key, 16);
  print_buf("IV data:", (uint8_t *)IV, sizeof(IV));
  print_buf("Auth data:", (uint8_t *)AddData, sizeof(AddData));
  print_buf("Cipher text:", (uint8_t *)InputData, sizeof(InputData));

  aes_gcm_encrypt(AES_GCM_MODE_DECRYPTION,
                  (uint8_t *)Key, sizeof(Key), (uint8_t *)InputData, sizeof(InputData), 
                  (uint8_t *)IV, sizeof(IV), (uint8_t *)AddData, sizeof(AddData), 
                  output_data, tag);
  
  print_buf("Expected plain text:", (uint8_t *)Expected_Output, sizeof(Expected_Output));
  print_buf("Computed plain text:", (uint8_t *)output_data, sizeof(Expected_Output));
  /*Compare results with expected buffer*/
  if(memcmp(Expected_Output, output_data, sizeof(Expected_Output)) != 0)
  {
    /* Processing Error */
    printf(BRIGHT_RED"Computed output is NOT the same as expected output!\r\n"RESET_COLOR);
    printf(BRIGHT_RED"Test failed!\r\n"RESET_COLOR);
  }
  else
  {
    printf(BRIGHT_GREEN"Computed output is the same as expected output!\r\n"RESET_COLOR);
    printf(BRIGHT_GREEN"Test OK!\r\n"RESET_COLOR);    
  }
  print_buf("Expected GCM tag:", (uint8_t *)Expected_Tag, sizeof(Expected_Tag));
  print_buf("Computed GCM tag:", (uint8_t *)tag, sizeof(Expected_Tag));
  
  /*Compare results with expected buffer*/
  if(memcmp(Expected_Tag, tag, sizeof(Expected_Tag)) != 0)
  {
    /* Processing Error */
    printf(BRIGHT_RED"Computed tag is NOT the same as expected tag!\r\n"RESET_COLOR);
    printf(BRIGHT_RED"Test failed!\r\n"RESET_COLOR);
  }
  else
  {
    printf(BRIGHT_GREEN"Computed tag is the same as expected tag!\r\n"RESET_COLOR);
    printf(BRIGHT_GREEN"Test OK!\r\n"RESET_COLOR);    
  }
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
  BSP_LED_Init(LD2); /* Green LED */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_AES_Init();
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
#if 0  
  print_buf("AES Key:", (uint8_t *)pKeyAES, 16);
  print_buf("IV data:", (uint8_t *)pInitVectAES, sizeof(pInitVectAES));
  print_buf("Auth data:", (uint8_t *)HeaderAES, sizeof(HeaderAES));
  print_buf("Plain text:", (uint8_t *)Plaintext, PLAINTEXT_SIZE*4);
  /*##-2- Encryption Phase #################################################*/
  if (HAL_CRYP_Encrypt(&hcryp, Plaintext, PLAINTEXT_SIZE, EncryptedText, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }
  print_buf("Expected cipher text:", (uint8_t *)Ciphertext, 16);
  print_buf("Computed cipher text:", (uint8_t *)EncryptedText, 16);
  /*Compare results with expected buffer*/
  if(memcmp(EncryptedText, Ciphertext, 16) != 0)
  {
    /* Processing Error */
    Error_Handler();
  }
  /* Compute the authentication TAG */
  if (HAL_CRYPEx_AESGCM_GenerateAuthTAG(&hcryp,TAG, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }
  print_buf("Expected GCM tag:", (uint8_t *)ExpectedTAG, 16);
  print_buf("Computed GCM tag:", (uint8_t *)TAG, 16);
  /*Compare results with expected buffer*/
  if(memcmp(TAG, ExpectedTAG, 16) != 0)
  {
    /* Processing Error */
    Error_Handler();
  }
  /*##-3- Decryption Phase #################################################*/
  if (HAL_CRYP_Decrypt(&hcryp,Ciphertext , PLAINTEXT_SIZE, DecryptedText, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }
  /*Compare results with expected buffer*/
  if(memcmp(DecryptedText, Plaintext, 16) != 0)
  {
    /* Processing Error */
    Error_Handler();
  }
  else
  {
    /* Right Encryption : Turn LD2 on */
    BSP_LED_On(LD2);
  }
  /* Compute the authentication TAG */
  if (HAL_CRYPEx_AESGCM_GenerateAuthTAG(&hcryp,TAG, TIMEOUT_VALUE) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }
  /*Compare results with expected buffer*/
  if(memcmp(TAG, ExpectedTAG, 16) != 0)
  {
    /* Processing Error */
    Error_Handler();
  }
#endif  
  aes_gcm_test1();
  aes_gcm_test2();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* Encrypted/Decrypted text and computed Tag are correct : Turn LD2 on */
    BSP_LED_On(LD2);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS;
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
  * @brief AES Initialization Function
  * @param None
  * @retval None
  */
static void MX_AES_Init(void)
{

  /* USER CODE BEGIN AES_Init 0 */

  /* USER CODE END AES_Init 0 */

  /* USER CODE BEGIN AES_Init 1 */

  /* USER CODE END AES_Init 1 */
  hcryp.Instance = AES;
  hcryp.Init.DataType = CRYP_NO_SWAP;
  hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey = (uint32_t *)pKeyAES;
  hcryp.Init.pInitVect = (uint32_t *)pInitVectAES;
  hcryp.Init.Algorithm = CRYP_AES_GCM_GMAC;
  hcryp.Init.Header = (uint32_t *)HeaderAES;
  hcryp.Init.HeaderSize = 4;
  hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_WORD;
  hcryp.Init.HeaderWidthUnit = CRYP_HEADERWIDTHUNIT_WORD;
  hcryp.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;
  hcryp.Init.KeyMode = CRYP_KEYMODE_NORMAL;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN AES_Init 2 */

  /* USER CODE END AES_Init 2 */

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
   while (1)
  {
    BSP_LED_Toggle(LD2);
    HAL_Delay(250);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }

  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
