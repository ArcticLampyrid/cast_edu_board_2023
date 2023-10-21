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
#include "stm32f4xx_hal.h"

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
#define OLED_RESET_Pin GPIO_PIN_13
#define OLED_RESET_GPIO_Port GPIOC
#define OLED_DATA_CMD_Pin GPIO_PIN_14
#define OLED_DATA_CMD_GPIO_Port GPIOC
#define KEY_OK_Pin GPIO_PIN_15
#define KEY_OK_GPIO_Port GPIOC
#define KEY_USER_1_Pin GPIO_PIN_0
#define KEY_USER_1_GPIO_Port GPIOA
#define KEY_USER_2_Pin GPIO_PIN_1
#define KEY_USER_2_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_6
#define LED_R_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_0
#define LED_G_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_1
#define LED_B_GPIO_Port GPIOB
#define KB_C4_Pin GPIO_PIN_2
#define KB_C4_GPIO_Port GPIOB
#define KB_C3_Pin GPIO_PIN_12
#define KB_C3_GPIO_Port GPIOB
#define KB_C2_Pin GPIO_PIN_13
#define KB_C2_GPIO_Port GPIOB
#define KB_C1_Pin GPIO_PIN_14
#define KB_C1_GPIO_Port GPIOB
#define KB_R1_Pin GPIO_PIN_15
#define KB_R1_GPIO_Port GPIOB
#define IR_TX_Pin GPIO_PIN_8
#define IR_TX_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_11
#define BUZZER_GPIO_Port GPIOA
#define KB_R2_Pin GPIO_PIN_12
#define KB_R2_GPIO_Port GPIOA
#define IR_RX_Pin GPIO_PIN_15
#define IR_RX_GPIO_Port GPIOA
#define KB_R3_Pin GPIO_PIN_4
#define KB_R3_GPIO_Port GPIOB
#define KB_R4_Pin GPIO_PIN_5
#define KB_R4_GPIO_Port GPIOB
#define ENCODER_A_Pin GPIO_PIN_6
#define ENCODER_A_GPIO_Port GPIOB
#define ENCODER_A_EXTI_IRQn EXTI9_5_IRQn
#define ENCODER_B_Pin GPIO_PIN_7
#define ENCODER_B_GPIO_Port GPIOB
#define ENCODER_B_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
