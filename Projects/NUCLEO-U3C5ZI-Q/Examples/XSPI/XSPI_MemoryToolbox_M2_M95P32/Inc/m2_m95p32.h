/**
  ******************************************************************************
  * @file           : m2_m95p32.h
  * @brief          : Definitions for ST M95P32-I EEPROM memory.
  *                   This file contains the common defines of the application.
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
#ifndef __M2_M95P32_H
#define __M2_M95P32_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* M2 board LDO Setup time (ms) */
#define M2_M95P32_LDO_SETUP_TIME                     200U

/* Memory commands */

/* Write Enable command */
#define M2_M95P32_WRITE_ENABLE_CMD                   0x06U
#define M2_M95P32_WRITE_DISABLE_CMD                  0x04U
#define M2_M95P32_WRITE_ENABLE_DISABLE_ACCESS_MODE   (HAL_XSPI_INSTRUCTION_1_LINE | HAL_XSPI_INSTRUCTION_8_BITS)
/* Autopolling for WEL and WIP bits */
#define M2_M95P32_WRITE_ENABLE_MATCH_VALUE           0x02U
#define M2_M95P32_WRITE_ENABLE_MASK_VALUE            0x02U
#define M2_M95P32_WIP_STATUS_MATCH_VALUE             0x00U
#define M2_M95P32_WIP_STATUS_MASK_VALUE              0x01U
/* Status Register access commands */
#define M2_M95P32_REG_SR_READ_CMD                    0x05U
#define M2_M95P32_REG_SR_READ_DUMMY_CYCLE            0U
#define M2_M95P32_REG_SR_READ_ACCESS_MODE            (HAL_XSPI_INSTRUCTION_1_LINE | HAL_XSPI_INSTRUCTION_8_BITS | \
                                                      HAL_XSPI_DATA_1_LINE)
#define M2_M95P32_REG_SR_WRITE_CMD                   0x01U
#define M2_M95P32_REG_SR_WRITE_ACCESS_MODE           (HAL_XSPI_INSTRUCTION_1_LINE | HAL_XSPI_INSTRUCTION_8_BITS | \
                                                      HAL_XSPI_DATA_1_LINE)
/* Configuration Register access commands */
#define M2_M95P32_REG_CR_READ_CMD                    0x15U
#define M2_M95P32_REG_CR_READ_DUMMY_CYCLE            0U
#define M2_M95P32_REG_CR_READ_ACCESS_MODE            (HAL_XSPI_INSTRUCTION_1_LINE | HAL_XSPI_INSTRUCTION_8_BITS | \
                                                      HAL_XSPI_DATA_1_LINE)
#define M2_M95P32_REG_CR_WRITE_CMD                   0x01U
#define M2_M95P32_REG_CR_WRITE_ACCESS_MODE           (HAL_XSPI_INSTRUCTION_1_LINE | HAL_XSPI_INSTRUCTION_8_BITS | \
                                                      HAL_XSPI_DATA_1_LINE)
/* JEDEC ID and SFDP content reading commands */
#define M2_M95P32_JEDEC_READ_CMD                     0x9FU
#define M2_M95P32_READ_JEDEC_ID_ACCESS_MODE          (HAL_XSPI_INSTRUCTION_1_LINE | HAL_XSPI_INSTRUCTION_8_BITS | \
                                                      HAL_XSPI_DATA_1_LINE)
#define M2_M95P32_JEDEC_ID_VALUE                     {0x20U,0x00U,0x16U}
#define M2_M95P32_READ_SFDP_CMD                      0x5AU
#define M2_M95P32_READ_SFDP_ACCESS_MODE              (HAL_XSPI_INSTRUCTION_1_LINE | HAL_XSPI_INSTRUCTION_8_BITS | \
                                                      HAL_XSPI_ADDRESS_1_LINE     | HAL_XSPI_ADDRESS_24_BITS    | \
                                                      HAL_XSPI_DATA_1_LINE)
#define M2_M95P32_SFDP_DUMMY_CYCLES                  8U
/* Read and Page Program commands */
#define M2_M95P32_QUAD_IO_READ_CMD                   0x6BU
#define M2_M95P32_QUAD_IO_READ_ACCESS_MODE           (HAL_XSPI_INSTRUCTION_1_LINE | HAL_XSPI_INSTRUCTION_8_BITS | \
                                                      HAL_XSPI_ADDRESS_1_LINE     | HAL_XSPI_ADDRESS_24_BITS    | \
                                                      HAL_XSPI_DATA_4_LINES)
#define M2_M95P32_QUAD_IO_READ_DUMMY_CYCLE           8U
#define M2_M95P32_QUAD_PAGE_PROG_CMD                 0x0AU
#define M2_M95P32_QUAD_PAGE_PROG_ACCESS_MODE         (HAL_XSPI_INSTRUCTION_1_LINE | HAL_XSPI_INSTRUCTION_8_BITS | \
                                                      HAL_XSPI_ADDRESS_1_LINE     | HAL_XSPI_ADDRESS_24_BITS    | \
                                                      HAL_XSPI_DATA_1_LINE)
/* Sector Erase command */
#define M2_M95P32_SECTOR_ERASE_CMD                   0x20U
#define M2_M95P32_SECTOR_ERASE_ACCESS_MODE           (HAL_XSPI_INSTRUCTION_1_LINE | HAL_XSPI_INSTRUCTION_8_BITS | \
                                                      HAL_XSPI_ADDRESS_1_LINE     | HAL_XSPI_ADDRESS_24_BITS)
#define M2_M95P32_SECTOR_ERASE_TIMEOUT               100U    /* Max 5ms in spec */
/* Page Size */
#define M2_M95P32_PAGE_SIZE                          256U
/* Macro for retrieving xSPI peripheral frequency */
#define M2_M95P32_PERIPH_CLOCK_VALUE                 HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_OCTOSPI1);

/* Defines used to configure memory registers (operations performed in 1-Line) */
/*  CR register settings */
#define M2_M95P32_REG_CR_DRIVE_STRENGH_VALUE         0x20U
#define M2_M95P32_REG_CR_DRIVE_STRENGH_MASK          0x60U


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __M2_M95P32_H */
