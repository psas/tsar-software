/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "string.h"
#include <stdio.h>
#include<stdlib.h>

//#include "stm32f0xx_hal.h"
//#include <ctype.h>// for tolower()
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */



//This will let us call functions from the CDC library

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
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

int len, command;
uint8_t buffer[100];
char Rx_indx, Rx_data[2], Rx_Buffer[100], Transfer_cplt ;
int sov1 = 0, sov2= 0, sov3= 0, sov4= 0, sov5= 0, sov6= 0, sov7= 0, sov8= 0, ig1= 0, ig2 =0 ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

//Interrupt callback routine
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t i;
    if (huart->Instance == USART2)
        {
        if (Rx_indx==0) {for (i=0;i<100;i++) Rx_Buffer[i]=0;}   //clear Rx_Buffer before receiving new data

        if (Rx_data[0]!=13) //if received data different from ascii 13 (enter)
            {
            Rx_Buffer[Rx_indx++]=Rx_data[0];    //add data to Rx_Buffer
            }
        else            //if received data = 13
            {
            Rx_indx=0;
            Transfer_cplt = 1;//transfer complete, data is ready to read
            }

        HAL_UART_Receive_IT(&huart2, Rx_data, 1);   //activate UART receive interrupt every time
        }

}

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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  //***************************************************************
  //HAL_UART_Receive_IT(&huart2, &rxBuffer, 1); // start listening for incomming data
  HAL_UART_Receive_IT(&huart2,Rx_data, 1); // Asserts UART Interrupt every time 1 byte is received.
  //***************************************************************
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

     if (Transfer_cplt)
     {
    	 command = atoi(Rx_Buffer);

    	 switch (command){
    	 	 case 1:
    	 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
    	 		sprintf(buffer, "Valve SOV1 is ON..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//sov1 = 1;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 2:
    	 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 		sprintf(buffer, "Valve SOV1 is OFF..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//sov1 = 0;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 3:
                //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 		sprintf(buffer, "Valve SOV2 is ON..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//sov2 = 1;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 4:
    	 		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 		sprintf(buffer, "Valve SOV2 is OFF..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//sov2 = 0;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 5:
    	 		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 		sprintf(buffer, "Valve SOV3 is ON..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//sov3 = 1;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 6:
    	 		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 		sprintf(buffer, "Valve SOV3 is OFF..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//sov3 = 0;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 7:
    	   	     //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	    	 sprintf(buffer, "Valve SOV4 is ON..\r\n");
    	    	 len =strlen(buffer);
    	    	 HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	    	 //sov4 = 1;
    	    	 Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	    	 break;
    	 	 case 8:
    	   	 	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	    	 sprintf(buffer, "Valve SOV4 is OFF..\r\n");
    	    	 len =strlen(buffer);
    	    	 HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	    	 //sov4 = 0;
    	    	 Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	    	 break;
    	 	 case 9:
    	   	 	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	    	 sprintf(buffer, "Valve SOV5 is ON..\r\n");
    	    	 len =strlen(buffer);
    	    	 HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	    	 //sov5 = 1;
    	    	 Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	    	 break;
    	 	 case 10:
    	 		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 		sprintf(buffer, "Valve SOV5 is OFF..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//sov5 = 0;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 11:
    	 		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 		sprintf(buffer, "Valve SOV6 is ON..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//sov6 = 1;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 12:
    	 		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 	     sprintf(buffer, "Valve SOV6 is OFF..\r\n");
    	 	     len =strlen(buffer);
    	 	     HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 	     //sov6 = 0;
    	 		 Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		 break;
    	 	 case 13:
    	 		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
    	 		sprintf(buffer, "Valve SOV7 is ON..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//sov7 = 1;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 14:
    	 		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 		sprintf(buffer, "Valve SOV7 is OFF..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//sov7 =0;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 15:
                //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 		sprintf(buffer, "Valve SOV8 is ON..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//sov8 = 1;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 16:
    	 		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 		sprintf(buffer, "Valve SOV8 is OFF..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//sov8 = 0;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 17:
    	 		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 		sprintf(buffer, "IG1 is ON..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//ig1 = 1;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 18:
    	 		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	 		sprintf(buffer, "IG1 is OFF..\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		//ig1 = 0;
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;
    	 	 case 19:
    	   	     //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	    	 sprintf(buffer, "IG2 is ON..\r\n");
    	    	 len =strlen(buffer);
    	    	 HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	    	 //ig2 = 1;
    	    	 Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	    	 break;
    	 	 case 20:
    	   	 	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
    	    	 sprintf(buffer, "Valve IG2 is OFF..\r\n");
    	    	 len =strlen(buffer);
    	    	 HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	    	// ig2 = 0;
    	    	 Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	    	 break;


             /*
    	 	 case 21:
    	 		// Status
    	 		sprintf(buffer, "\n*** Valves and Igniters Status ***\r\n\n SOV1: %d\n SOV2: %d\n SOV3: %d\n SOV4: %d\n SOV5: %d\n SOV6: %d\n SOV7: %d\n SOV8: %d\n IG1:  %d\n IG2:  %d\r\n", sov1, sov2, sov3, sov4, sov5, sov6, sov7, sov8, ig1, ig2);
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
    	 		break;

    	 	 case 22:
                //Exiting the python script means turning off so turn off all the valves and igniters
    	 		sov1 = 0, sov2= 0, sov3= 0, sov4= 0, sov5= 0, sov6= 0, sov7= 0, sov8= 0, ig1= 0, ig2 =0;
    	 		//After setting the status we can proceed to turn low all the GPIOs used.
    	 	    break;*/

    	 	 default:
    	 		sprintf(buffer, "Please enter a valid command...\r\n");
    	 		len =strlen(buffer);
    	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);
    	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable

    	 }

        }// end of if(Transfer_cplt)

      }//end of while loop


  /* USER CODE END 3 */
}// end of main function

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/*
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	__NOP(); // nop is used to put a breakpoint if we are debugging
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

   __NOP();

}
*/
 //***********************************************************************************

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

	 //
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
