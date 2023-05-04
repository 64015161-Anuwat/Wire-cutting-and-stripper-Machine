/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

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

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define T_IRQ_Pin GPIO_PIN_2
#define T_IRQ_GPIO_Port GPIOE
#define T_CLK_Pin GPIO_PIN_3
#define T_CLK_GPIO_Port GPIOE
#define T_MISO_Pin GPIO_PIN_4
#define T_MISO_GPIO_Port GPIOE
#define T_MOSI_Pin GPIO_PIN_5
#define T_MOSI_GPIO_Port GPIOE
#define T_CS_Pin GPIO_PIN_6
#define T_CS_GPIO_Port GPIOE
#define DST2_Pin GPIO_PIN_0
#define DST2_GPIO_Port GPIOF
#define STEP2_Pin GPIO_PIN_1
#define STEP2_GPIO_Port GPIOF
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define BTN_PAUSE_Pin GPIO_PIN_2
#define BTN_PAUSE_GPIO_Port GPIOG
#define BTN_PAUSE_EXTI_IRQn EXTI2_IRQn
#define BTN_CANCEL_Pin GPIO_PIN_3
#define BTN_CANCEL_GPIO_Port GPIOG
#define BTN_CANCEL_EXTI_IRQn EXTI3_IRQn
#define CS_Pin GPIO_PIN_8
#define CS_GPIO_Port GPIOC
#define DC_Pin GPIO_PIN_9
#define DC_GPIO_Port GPIOC
#define RST_Pin GPIO_PIN_10
#define RST_GPIO_Port GPIOC
#define STEP1_Pin GPIO_PIN_0
#define STEP1_GPIO_Port GPIOD
#define DST1_Pin GPIO_PIN_1
#define DST1_GPIO_Port GPIOD
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
