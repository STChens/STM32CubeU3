/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    CRC/CRC_Example/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32U3xx CRC HAL API
  *          to get a CRC code of a given buffer of data words (32-bit),
  *          based on a fixed generator polynomial (0x4C11DB7).
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
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE    (2100/4) //114
#define TEST_NB_BYTES  (BUFFER_SIZE*4)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

CRC_HandleTypeDef hcrc;

/* USER CODE BEGIN PV */

/* Used for storing CRC Value */
__IO uint32_t uwCRCValue = 0;

static const uint32_t aDataBuffer[BUFFER_SIZE] =
{
  0x00001021, 0x20423063, 0x408450a5, 0x60c670e7, 0x9129a14a, 0xb16bc18c,
  0xd1ade1ce, 0xf1ef1231, 0x32732252, 0x52b54294, 0x72f762d6, 0x93398318,
  0xa35ad3bd, 0xc39cf3ff, 0xe3de2462, 0x34430420, 0x64e674c7, 0x44a45485,
  0xa56ab54b, 0x85289509, 0xf5cfc5ac, 0xd58d3653, 0x26721611, 0x063076d7,
  0x569546b4, 0xb75ba77a, 0x97198738, 0xf7dfe7fe, 0xc7bc48c4, 0x58e56886,
  0x78a70840, 0x18612802, 0xc9ccd9ed, 0xe98ef9af, 0x89489969, 0xa90ab92b,
  0x4ad47ab7, 0x6a961a71, 0x0a503a33, 0x2a12dbfd, 0xfbbfeb9e, 0x9b798b58,
  0xbb3bab1a, 0x6ca67c87, 0x5cc52c22, 0x3c030c60, 0x1c41edae, 0xfd8fcdec,
  0xad2abd0b, 0x8d689d49, 0x7e976eb6, 0x5ed54ef4, 0x2e321e51, 0x0e70ff9f,
  0xefbedfdd, 0xcffcbf1b, 0x9f598f78, 0x918881a9, 0xb1caa1eb, 0xd10cc12d,
  0xe16f1080, 0x00a130c2, 0x20e35004, 0x40257046, 0x83b99398, 0xa3fbb3da,
  0xc33dd31c, 0xe37ff35e, 0x129022f3, 0x32d24235, 0x52146277, 0x7256b5ea,
  0x95a88589, 0xf56ee54f, 0xd52cc50d, 0x34e224c3, 0x04817466, 0x64475424,
  0x4405a7db, 0xb7fa8799, 0xe75ff77e, 0xc71dd73c, 0x26d336f2, 0x069116b0,
  0x76764615, 0x5634d94c, 0xc96df90e, 0xe92f99c8, 0xb98aa9ab, 0x58444865,
  0x78066827, 0x18c008e1, 0x28a3cb7d, 0xdb5ceb3f, 0xfb1e8bf9, 0x9bd8abbb,
  0x4a755a54, 0x6a377a16, 0x0af11ad0, 0x2ab33a92, 0xed0fdd6c, 0xcd4dbdaa,
  0xad8b9de8, 0x8dc97c26, 0x5c644c45, 0x3ca22c83, 0x1ce00cc1, 0xef1fff3e,
  0xdf7caf9b, 0xbfba8fd9, 0x9ff86e17, 0x7e364e55, 0x2e933eb2, 0x0ed11ef0
};

/* Expected CRC Value */
uint32_t uwExpectedCRCValue = 0x379E9F06;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_CRC_Init(uint32_t dataformat);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */
static void print_buf(char *str, unsigned char *buf, int len);
static uint32_t crc_test(uint32_t dataformat, int nbbytes, uint32_t *pdata);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//#define PRINT_DATA_BUF
static void print_buf(char *str, unsigned char *buf, int len)
{
#ifdef PRINT_DATA_BUF
        int i;
        if ( str != NULL )
                printf("%s\n", str);
        printf("=============================================\n");
        for (i = 0; i< len; i++)
        {
                printf("%02x ", buf[i]);
                if ((i+1)%16 == 0 )
                        printf("\r\n");
        }
        printf("\n=============================================\n");
#endif
}

static uint32_t crc_test(uint32_t dataformat, int nbbytes, uint32_t *pdata)
{
	uint32_t crcval = 0;
	uint32_t bufsize = nbbytes;

	printf("====================================================\n");
	printf("CRC test: number of bytes for CRC computation [%d]\r\n", nbbytes);
	printf("====================================================\n");

	switch (dataformat)
	{
	case CRC_INPUTDATA_FORMAT_BYTES:
		MX_CRC_Init(dataformat);
		//MODIFY_REG(CRC->CR, (CRC_CR_RTYPE_IN | CRC_CR_REV_IN), CRC_INPUTDATA_INVERSION_BYTE_BYWORD);
		printf("Data format: FORMAT_BYTES, %lu bytes\r\n", bufsize);
		break;
	case CRC_INPUTDATA_FORMAT_HALFWORDS:
		MX_CRC_Init(dataformat);
		//MODIFY_REG(CRC->CR, (CRC_CR_RTYPE_IN | CRC_CR_REV_IN), CRC_INPUTDATA_INVERSION_HALFWORD_BYWORD);
		bufsize /= 2;
		printf("Data format: FORMAT_HALFWORDS, %lu halfwords\r\n", bufsize);
		break;
	case CRC_INPUTDATA_FORMAT_WORDS:
		MX_CRC_Init(dataformat);
		bufsize /= 4;
		printf("Data format: FORMAT_WORDS, %lu words\r\n", bufsize);
		break;
	default:
		printf("Invalid data format! End the test.\r\n");
		return crcval;
	}

	uint32_t end = 0;
	uint32_t sysclkfreq = 0;

	print_buf("Data for CRC computation:", (uint8_t*)pdata, nbbytes);

	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk | CoreDebug_DEMCR_MON_EN_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	crcval = HAL_CRC_Calculate(&hcrc, (uint32_t *)pdata, bufsize);

	end = DWT->CYCCNT;
	DWT->CTRL &= (~DWT_CTRL_CYCCNTENA_Msk);

	HAL_CRC_DeInit(&hcrc);

	sysclkfreq = HAL_RCC_GetSysClockFreq();
	sysclkfreq /= 1000;
	sysclkfreq /= 1000;
	printf("System clock frequency: %luMHz\r\n", sysclkfreq);
	printf("Number of clock cycle to compute CRC: %lu\r\n", end);
	printf("Time consumed to compute CRC: %lu us\r\n", end/sysclkfreq);

	printf("Computed CRC value: %08lx\r\n", crcval);

	return crcval;
}

static uint32_t CRC_Calculate(uint32_t *pdata, int nbytes);
static uint32_t CRC_Calculate(uint32_t *pdata, int nbytes)
{
	int words = nbytes/4;
	int remaining;
	int i;
	uint8_t *pBuffer = (uint8_t *)&pdata[0];

	uint16_t data;
	__IO uint16_t *pReg;

	// reset CRC DR register
	CRC->CR |= CRC_CR_RESET;

	remaining = nbytes - words*4;
	// need to handle byte case
	// write the words first
	MODIFY_REG(CRC->CR, (CRC_CR_RTYPE_IN | CRC_CR_REV_IN), CRC_INPUTDATA_INVERSION_BYTE_BYWORD);
	for (i = 0; i < words; i++ )
	{
		CRC->DR = pdata[i];
	}

	MODIFY_REG(CRC->CR, (CRC_CR_RTYPE_IN | CRC_CR_REV_IN), CRC_INPUTDATA_INVERSION_NONE);
	// handle the remainig bytes
	if ((remaining) == 1U)
	{
	  *(__IO uint8_t *)(__IO void *)(&CRC->DR) = pBuffer[4U * i];         /* Derogation MisraC2012 R.11.5 */
	}
	if ((remaining) == 2U)
	{
	  data = ((uint32_t)(pBuffer[4U * i]) << 8U) | ((uint32_t)(pBuffer[(4U * i) + 1U]));
      pReg = (__IO uint16_t *)(__IO void *)(&CRC->DR);                    /* Derogation MisraC2012 R.11.5 */
      *pReg = data;
	}
	if ((remaining) == 3U)
	{
	  data = ((uint16_t)(pBuffer[4U * i]) << 8U) | (uint16_t)pBuffer[(4U * i) + 1U];
	  pReg = (__IO uint16_t *)(__IO void *)(&CRC->DR);                    /* Derogation MisraC2012 R.11.5 */
	  *pReg = data;

	  *(__IO uint8_t *)(__IO void *)(&CRC->DR) = pBuffer[(4U * i) + 2U];  /* Derogation MisraC2012 R.11.5 */
	}
	return CRC->DR;
}

static uint32_t crc_test_reg(int nbbytes, uint32_t *pdata)
{
	uint32_t crcval = 0;
	uint32_t bufsize = nbbytes;
	uint32_t end = 0;
	uint32_t sysclkfreq = 0;

	printf("====================================================\n");
	printf("CRC test: number of bytes for CRC computation [%d]\r\n", nbbytes);
	printf("CRC register write without calling HAL API\r\n");
	printf("====================================================\n");

	//MODIFY_REG(CRC->CR, (CRC_CR_RTYPE_IN | CRC_CR_REV_IN), CRC_INPUTDATA_INVERSION_BYTE_BYWORD);
	print_buf("Data for CRC computation:", (uint8_t*)pdata, nbbytes);

	MX_CRC_Init(CRC_INPUTDATA_FORMAT_BYTES);

	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk | CoreDebug_DEMCR_MON_EN_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	crcval = CRC_Calculate((uint32_t *)pdata, bufsize);

	end = DWT->CYCCNT;
	DWT->CTRL &= (~DWT_CTRL_CYCCNTENA_Msk);

	HAL_CRC_DeInit(&hcrc);

	sysclkfreq = HAL_RCC_GetSysClockFreq();
	sysclkfreq /= 1000;
	sysclkfreq /= 1000;
	printf("System clock frequency: %luMHz\r\n", sysclkfreq);
	printf("Number of clock cycle to compute CRC: %lu\r\n", end);
	printf("Time consumed to compute CRC: %lu us\r\n", end/sysclkfreq);

	printf("Computed CRC value: %08lx\r\n", crcval);

	return crcval;
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */

  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  COM_InitTypeDef COM_Init;
  COM_Init.BaudRate = 115200;
  COM_Init.WordLength = COM_WORDLENGTH_8B;
  COM_Init.StopBits = COM_STOPBITS_1;
  COM_Init.Parity = COM_PARITY_NONE;
  COM_Init.HwFlowCtl = COM_HWCONTROL_NONE;

  BSP_COM_Init(COM1, &COM_Init);
  printf("COM Init done.\r\n");

  //uwCRCValue = crc_test(CRC_INPUTDATA_FORMAT_HALFWORDS, TEST_NB_BYTES, (uint32_t *)aDataBuffer);
  //uwCRCValue = crc_test(CRC_INPUTDATA_FORMAT_WORDS, TEST_NB_BYTES, (uint32_t *)aDataBuffer);

  uwCRCValue = crc_test(CRC_INPUTDATA_FORMAT_BYTES, TEST_NB_BYTES, (uint32_t *)aDataBuffer);
  uwCRCValue = crc_test_reg(TEST_NB_BYTES, (uint32_t *)aDataBuffer);

  uwCRCValue = crc_test(CRC_INPUTDATA_FORMAT_BYTES, TEST_NB_BYTES-1, (uint32_t *)aDataBuffer);
  uwCRCValue = crc_test_reg(TEST_NB_BYTES-1, (uint32_t *)aDataBuffer);

  uwCRCValue = crc_test(CRC_INPUTDATA_FORMAT_BYTES, TEST_NB_BYTES-2, (uint32_t *)aDataBuffer);
  uwCRCValue = crc_test_reg(TEST_NB_BYTES-2, (uint32_t *)aDataBuffer);

  uwCRCValue = crc_test(CRC_INPUTDATA_FORMAT_BYTES, TEST_NB_BYTES-3, (uint32_t *)aDataBuffer);
  uwCRCValue = crc_test_reg(TEST_NB_BYTES-3, (uint32_t *)aDataBuffer);

  /* Configure LD2 */
  BSP_LED_Init(LD2);

  printf("\r\nTest word mode CRC computation of 114 words\r\n");
  MX_CRC_Init(CRC_INPUTDATA_FORMAT_WORDS);
  /* Compute the CRC of "aDataBuffer" */
  uwCRCValue = HAL_CRC_Calculate(&hcrc, (uint32_t *)aDataBuffer, 114);

  /* Compare the CRC value to the Expected one */
  if (uwCRCValue != uwExpectedCRCValue)
  {
    /* Wrong CRC value: enter Error_Handler */
	printf("Wrong CRC value: enter Error_Handler!\r\n");
    Error_Handler();
  }
  else
  {
    /* Right CRC value: Turn LD2 on */
	printf("\r\nRight CRC value: %08lx\r\n", uwCRCValue);
    BSP_LED_On(LD2);
  }

  /* USER CODE END 2 */

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV2; // set clock to 48MHz
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
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(uint32_t dataformat)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = dataformat;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

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
  while (1)
  {
    /* Error if LD2 is slowly blinking (1 sec. period) */
    BSP_LED_Toggle(LD2);
    HAL_Delay(500);
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
