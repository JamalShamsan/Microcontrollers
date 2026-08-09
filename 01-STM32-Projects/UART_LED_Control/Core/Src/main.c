/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Temporary byte filled directly by UART reception */
	uint8_t uartRxByte;

	/* Command copied from uartRxByte */
	volatile uint8_t receivedByte;

	/* Set by the interrupt callback when a new command arrives */
	volatile uint8_t uartCommandReady = 0;

	/* Controls automatic blinking of the internal LED */
	uint8_t blinkMode = 0;

	/* Time of the previous LED toggle */
	uint32_t previousTime = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
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
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  /*
   * Prepare USART2 to receive one byte using interrupts.
   * This function returns immediately and does not block the CPU.
   */
  HAL_UART_Receive_IT(&huart2, &uartRxByte, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  /* USER CODE END WHILE */

	     /* USER CODE BEGIN 3 */

	     /*
	      * The UART callback sets this flag when a byte arrives.
	      */
	     if (uartCommandReady == 1)
	     {
	         /* Process this command only once */
	         uartCommandReady = 0;

	         if (receivedByte == '1')
	         {
	             /* Stop blinking and turn internal LED on */
	             blinkMode = 0;
	             HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

	             uint8_t message[] = "Internal LED ON\r\n";
	             HAL_UART_Transmit(
	                 &huart2,
	                 message,
	                 sizeof(message) - 1,
	                 HAL_MAX_DELAY
	             );
	         }
	         else if (receivedByte == '0')
	         {
	             /* Stop blinking and turn internal LED off */
	             blinkMode = 0;
	             HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

	             uint8_t message[] = "Internal LED OFF\r\n";
	             HAL_UART_Transmit(
	                 &huart2,
	                 message,
	                 sizeof(message) - 1,
	                 HAL_MAX_DELAY
	             );
	         }
	         else if (receivedByte == '2')
	         {
	             /* Start automatic blinking */
	             blinkMode = 1;
	             previousTime = HAL_GetTick();

	             uint8_t message[] = "Internal LED BLINKING\r\n";
	             HAL_UART_Transmit(
	                 &huart2,
	                 message,
	                 sizeof(message) - 1,
	                 HAL_MAX_DELAY
	             );
	         }
	         else if (receivedByte == '3')
	         {
	             /* Turn external LED on */
	             HAL_GPIO_WritePin(
	                 EXT_LED_GPIO_Port,
	                 EXT_LED_Pin,
	                 GPIO_PIN_SET
	             );

	             uint8_t message[] = "External LED ON\r\n";
	             HAL_UART_Transmit(
	                 &huart2,
	                 message,
	                 sizeof(message) - 1,
	                 HAL_MAX_DELAY
	             );
	         }
	         else if (receivedByte == '4')
	         {
	             /* Turn external LED off */
	             HAL_GPIO_WritePin(
	                 EXT_LED_GPIO_Port,
	                 EXT_LED_Pin,
	                 GPIO_PIN_RESET
	             );

	             uint8_t message[] = "External LED OFF\r\n";
	             HAL_UART_Transmit(
	                 &huart2,
	                 message,
	                 sizeof(message) - 1,
	                 HAL_MAX_DELAY
	             );
	         }
	     }

	     /*
	      * This remains outside the UART command block so blinking
	      * continues even when no new command arrives.
	      */
	     if (blinkMode == 1)
	     {
	         if ((HAL_GetTick() - previousTime) >= 500)
	         {
	             HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	             previousTime = HAL_GetTick();
	         }
	     }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|EXT_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin EXT_LED_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|EXT_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
 * @brief Called automatically after USART2 receives one byte.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Confirm that USART2 caused the callback */
    if (huart->Instance == USART2)
    {
        /* Copy the received byte into the command variable */
        receivedByte = uartRxByte;

        /* Tell the main loop that a command is ready */
        uartCommandReady = 1;

        /*
         * Prepare reception for the next byte.
         * Reception must be restarted because we requested only one byte.
         */
        HAL_UART_Receive_IT(&huart2, &uartRxByte, 1);
    }
}

/**
 * @brief Called when the onboard button generates an EXTI interrupt.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static uint32_t lastButtonTime = 0;

    if (GPIO_Pin == B1_Pin)
    {
        /* Simple 200 ms debounce */
        if ((HAL_GetTick() - lastButtonTime) >= 200)
        {
            HAL_GPIO_TogglePin(
                EXT_LED_GPIO_Port,
                EXT_LED_Pin
            );

            lastButtonTime = HAL_GetTick();
        }
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
