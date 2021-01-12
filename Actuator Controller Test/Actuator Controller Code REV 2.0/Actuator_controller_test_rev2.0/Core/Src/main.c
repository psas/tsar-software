/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
int len, command;
char buffer[100], Tx_buffer[100];
char Rx_indx, Rx_data[2], Rx_Buffer[100], Transfer_cplt ;
int sov1 = 0, sov2= 0, sov3= 0, sov4= 0, sov5= 0, sov6= 0, sov7= 0, sov8= 0, sov9=0, ig1= 0, ig2 =0 ;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
//Interrupt callback routine
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t i;
    if (huart->Instance == USART2)
        {
        if (Rx_indx==0) {for (i=0;i<100;i++) Rx_Buffer[i]=0;}   //clear Rx_Buffer before receiving new data

        if (Rx_data[0] != 13) //if received data different from ascii 13 (enter)
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
// Transmit function
void Tx_serial(Tx_buffer)
{
	  sprintf(buffer, Tx_buffer);
	  len =strlen(buffer);
	  HAL_UART_Transmit(&huart2, buffer, len, 1000);
	  Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_UART_Receive_IT(&huart2,Rx_data, 1); // Asserts UART Interrupt every time 1 byte is received.

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
	     	 		//Turn ON           //For testing with LD2
	     	 			     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);

	     	 			     	 	  //Checking if the GPIO pin is high in case of errors
	     	 			     	 	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_SET)
	     	 			     	 	  	  {
	     	 			     	 	  	     	 //Tx_serial("Valve SOV1 is ON..\r\n");
	     	 			     	 	  	     	 sov1 = 1;
	     	 			     	 	  	   }
	     	 			     	 	  	else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
	     	 			     	 	  	{
	     	 			     	 	  	    // GPIO pin was low reattempting to set high
	     	 			     	 	  	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
	     	 			     	 	  	    sov1 = 1;
	     	 			     	 	  	}




	     	 	    Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
	     	 		break;

	     	 	 case 2:
	     	 		 //Turn OFF
	     	 			     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);

	     	 			     	 		//Checking if the GPIO pin is high
	     	 			     	 	    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_SET)
	     	 			     	 	    {
	     	 			     	 	    	      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
	     	 			     	 			      sov1 = 0;
	     	 			     	 	     }
	     	 			     	 	    else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
	     	 			     	 		{
	     	 			     	 			  sov1 = 0;

	     	 			     	 		}

	     	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
	     	 		break;
	     	 	 case 3:
	     	 		//Turn ON
	     	 			     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);

	     	 			     	 	  //Checking if the GPIO pin is high in case of errors
	     	 			     	 	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_SET)
	     	 			     	 	  	  {
	     	 			     	 	  	     	 //Tx_serial("Valve SOV1 is ON..\r\n");
	     	 			     	 	  	     	 sov2 = 1;
	     	 			     	 	  	   }
	     	 			     	 	  	else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET)
	     	 			     	 	  	{
	     	 			     	 	  	    // GPIO pin was low reattempting to set high
	     	 			     	 	  	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	     	 			     	 	  	    sov2 = 1;
	     	 			     	 	  	}


	     	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
	     	 		break;
	     	 	 case 4:
	     	 		 //Turn OFF
	     	 			     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);

	     	 			     	 		//Checking if the GPIO pin is high
	     	 			     	 	    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_SET)
	     	 			     	 	    {
	     	 			     	 	    	      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	     	 			     	 			      sov2 = 0;
	     	 			     	 	     }
	     	 			     	 	    else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET)
	     	 			     	 		{
	     	 			     	 			  sov2 = 0;

	     	 			     	 		}

	     	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
	     	 		break;
	     	 	 case 5:
	     	 		                    //Turn ON
	     	 			     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

	     	 			     	 	  //Checking if the GPIO pin is high in case of errors
	     	 			     	 	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET)
	     	 			     	 	  	  {
	     	 			     	 	  	     	 //Tx_serial("Valve SOV1 is ON..\r\n");
	     	 			     	 	  	     	 sov3 = 1;
	     	 			     	 	  	   }
	     	 			     	 	  	else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_RESET)
	     	 			     	 	  	{
	     	 			     	 	  	    // GPIO pin was low reattempting to set high
	     	 			     	 	  	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
	     	 			     	 	  	    sov3 = 1;
	     	 			     	 	  	}


	     	 		Transfer_cplt=0;
	     	 		break;
	     	 	 case 6:
	     	 		                     //Turn OFF
	     	 			     	 		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);

	     	 			     	 		//Checking if the GPIO pin is high
	     	 			     	 		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET)
	     	 			     	 		{
	     	 			     	 			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
	     	 			     	 			 sov3 = 0;
	     	 			     	 		}
	     	 			     	 		else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_RESET)
	     	 			     	 		{
	     	 			     	 			  sov3 = 0;

	     	 			     	 		}
	     	 		Transfer_cplt=0;
	     	 		break;
	     	 	 case 7:
	     	 		                   //Turn ON
	     	 			     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

	     	 			     	 		//Checking if the GPIO pin is high in case of errors
	     	 			     	 		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_SET)
	     	 			     	 			{
	     	 			     	 			     	 sov4 = 1;
	     	 			     	 			 }
	     	 			     	 		else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET)
	     	 			     	 			  {
	     	 			     	 			      // GPIO pin was low reattempting to set high
	     	 			     	 			     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	     	 			     	 			     sov4 = 1;
	     	 			     	 			    }
	     	    	Transfer_cplt=0;
	     	    	 break;
	     	 	 case 8:
	     	 		                     //Turn OFF
	     	 			     	 		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);

	     	 			     	 		//Checking if the GPIO pin is high
	     	 			     	 		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_SET)
	     	 			     	 		{
	     	 			     	 			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	     	 			     	 			 sov4 = 0;
	     	 			     	 		}
	     	 			     	 		else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET)
	     	 			     	 		{
	     	 			     	 			  sov4 = 0;

	     	 			     	 		}


	     	    	 Transfer_cplt=0;
	     	    	 break;
	     	 	 case 9:
	     	 		//Turn ON
	     	 			     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

	     	 			     	 	  //Checking if the GPIO pin is high in case of errors
	     	 			     	 	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == GPIO_PIN_SET)
	     	 			     	 	  	  {
	     	 			     	 	  	     	 //Tx_serial("Valve SOV1 is ON..\r\n");
	     	 			     	 	  	     	 sov5 = 1;
	     	 			     	 	  	   }
	     	 			     	 	  	else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == GPIO_PIN_RESET)
	     	 			     	 	  	{
	     	 			     	 	  	    // GPIO pin was low reattempting to set high
	     	 			     	 	  	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	     	 			     	 	  	    sov5 = 1;
	     	 			     	 	  	}

	     	    	 Transfer_cplt=0;
	     	    	 break;
	     	 	 case 10:
	     	 		 //Turn OFF
	     	 			     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

	     	 			     	 		//Checking if the GPIO pin is high
	     	 			     	 	    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == GPIO_PIN_SET)
	     	 			     	 	    {
	     	 			     	 	    	      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	     	 			     	 			      sov5 = 0;
	     	 			     	 	     }
	     	 			     	 	    else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == GPIO_PIN_RESET)
	     	 			     	 		{
	     	 			     	 			  sov5 = 0;

	     	 			     	 		}
	     	 		Transfer_cplt=0;
	     	 		break;
	     	 	 case 11:
	     	 		//Turn ON
	     	 			     	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);

	     	 			     	 	  //Checking if the GPIO pin is high in case of errors
	     	 			     	 	  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_SET)
	     	 			     	 	  	  {
	     	 			     	 	  	     	 //Tx_serial("Valve SOV1 is ON..\r\n");
	     	 			     	 	  	     	 sov6 = 1;
	     	 			     	 	  	   }
	     	 			     	 	  	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_RESET)
	     	 			     	 	  	{
	     	 			     	 	  	    // GPIO pin was low reattempting to set high
	     	 			     	 	  	    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
	     	 			     	 	  	    sov6 = 1;
	     	 			     	 	  	}

	     	 		Transfer_cplt=0;
	     	 		break;
	     	 	 case 12:
	     	 		 //Turn OFF
	     	 			     	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

	     	 			     	 		//Checking if the GPIO pin is high
	     	 			     	 	    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_SET)
	     	 			     	 	    {
	     	 			     	 	    	      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	     	 			     	 			      sov6 = 0;
	     	 			     	 	     }
	     	 			     	 	    else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_RESET)
	     	 			     	 		{
	     	 			     	 			  sov6 = 0;

	     	 			     	 		}
	     	 	     Transfer_cplt=0;
	     	 		 break;
	     	 	 case 13:
	     	 		//Turn ON
	     	 			     	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

	     	 			     	 	  //Checking if the GPIO pin is high in case of errors
	     	 			     	 	  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_SET)
	     	 			     	 	  	  {
	     	 			     	 	  	     	 //Tx_serial("Valve SOV1 is ON..\r\n");
	     	 			     	 	  	     	 sov7 = 1;
	     	 			     	 	  	   }
	     	 			     	 	  	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_RESET)
	     	 			     	 	  	{
	     	 			     	 	  	    // GPIO pin was low reattempting to set high
	     	 			     	 	  	    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	     	 			     	 	  	    sov7 = 1;
	     	 			     	 	  	}

	     	 		Transfer_cplt=0;
	     	 		break;
	     	 	 case 14:
	     	 		 //Turn OFF
	     	 			     	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);

	     	 			     	 		//Checking if the GPIO pin is high
	     	 			     	 	    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_SET)
	     	 			     	 	    {
	     	 			     	 	    	      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	     	 			     	 			      sov7 = 0;
	     	 			     	 	     }
	     	 			     	 	    else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_RESET)
	     	 			     	 		{
	     	 			     	 			  sov7 = 0;

	     	 			     	 		}
	     	 		Transfer_cplt=0;
	     	 		break;
	     	 	 case 15:
	     	 		//Turn ON
	     	 			     	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);

	     	 			     	 	  //Checking if the GPIO pin is high in case of errors
	     	 			     	 	  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == GPIO_PIN_SET)
	     	 			     	 	  	  {
	     	 			     	 	  	     	 //Tx_serial("Valve SOV1 is ON..\r\n");
	     	 			     	 	  	     	 sov8 = 1;
	     	 			     	 	  	   }
	     	 			     	 	  	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == GPIO_PIN_RESET)
	     	 			     	 	  	{
	     	 			     	 	  	    // GPIO pin was low reattempting to set high
	     	 			     	 	  	    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
	     	 			     	 	  	    sov8 = 1;
	     	 			     	 	  	}

	     	 		Transfer_cplt=0;
	     	 		break;
	     	 	 case 16:
	     	 		 //Turn OFF
	     	 			     	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

	     	 			     	 		//Checking if the GPIO pin is high
	     	 			     	 	    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == GPIO_PIN_SET)
	     	 			     	 	    {
	     	 			     	 	    	      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
	     	 			     	 			      sov8 = 0;
	     	 			     	 	     }
	     	 			     	 	    else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == GPIO_PIN_RESET)
	     	 			     	 		{
	     	 			     	 			  sov8 = 0;

	     	 			     	 		}
	     	 		Transfer_cplt=0;
	     	 		break;
	     	 	 case 17:
	     	 		//Turn ON
	     	 			     	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);

	     	 			     	 	  //Checking if the GPIO pin is high in case of errors
	     	 			     	 	  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_SET)
	     	 			     	 	  	  {
	     	 			     	 	  	     	 //Tx_serial("Valve SOV1 is ON..\r\n");
	     	 			     	 	  	     	 sov9 = 1;
	     	 			     	 	  	   }
	     	 			     	 	  	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_RESET)
	     	 			     	 	  	{
	     	 			     	 	  	    // GPIO pin was low reattempting to set high
	     	 			     	 	  	    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	     	 			     	 	  	    sov9 = 1;
	     	 			     	 	  	}

	     	 		Transfer_cplt=0;
	     	 		break;
	     	 	 case 18:
	     	 		 //Turn OFF
	     	 			     	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);

	     	 			     	 		//Checking if the GPIO pin is high
	     	 			     	 	    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_SET)
	     	 			     	 	    {
	     	 			     	 	    	      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	     	 			     	 			      sov9 = 0;
	     	 			     	 	     }
	     	 			     	 	    else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_RESET)
	     	 			     	 		{
	     	 			     	 			  sov9 = 0;

	     	 			     	 		}
	     	 		Transfer_cplt=0;
	     	 		break;
	     	 	 case 19:
	     	 		//Turn ON
	     	 			     	 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);

	     	 			     	 	  //Checking if the GPIO pin is high in case of errors
	     	 			     	 	  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_SET)
	     	 			     	 	  	  {
	     	 			     	 	  	     	 //Tx_serial("Valve SOV1 is ON..\r\n");
	     	 			     	 	  	     	 ig1 = 1;
	     	 			     	 	  	   }
	     	 			     	 	  	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET)
	     	 			     	 	  	{
	     	 			     	 	  	    // GPIO pin was low reattempting to set high
	     	 			     	 	  	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
	     	 			     	 	  	    ig1 = 1;
	     	 			     	 	  	}

	     	    	Transfer_cplt=0;
	     	    	 break;
	     	 	 case 20:
	     	 		 //Turn OFF
	     	 			     	 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);

	     	 			     	 		//Checking if the GPIO pin is high
	     	 			     	 	    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_SET)
	     	 			     	 	    {
	     	 			     	 	    	      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	     	 			     	 			      ig1 = 0;
	     	 			     	 	     }
	     	 			     	 	    else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET)
	     	 			     	 		{
	     	 			     	 			  ig1 = 0;

	     	 			     	 		}
	     	    	Transfer_cplt=0;
	     	    	 break;


	     	 	 case 21:
	     	 		//Turn ON
	     	 			     	 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);

	     	 			     	 	  //Checking if the GPIO pin is high in case of errors
	     	 			     	 	  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_SET)
	     	 			     	 	  	  {
	     	 			     	 	  	     	 //Tx_serial("Valve SOV1 is ON..\r\n");
	     	 			     	 	  	     	 ig2 = 1;
	     	 			     	 	  	   }
	     	 			     	 	  	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET)
	     	 			     	 	  	{
	     	 			     	 	  	    // GPIO pin was low reattempting to set high
	     	 			     	 	  	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	     	 			     	 	  	    ig2 = 1;
	     	 			     	 	  	}

	     	    	Transfer_cplt=0;
	     	    	 break;
	     	 	 case 22:
	     	 		 //Turn OFF
	     	 			     	 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);

	     	 			     	 		//Checking if the GPIO pin is high
	     	 			     	 	    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_SET)
	     	 			     	 	    {
	     	 			     	 	    	      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	     	 			     	 			      ig2 = 0;
	     	 			     	 	     }
	     	 			     	 	    else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET)
	     	 			     	 		{
	     	 			     	 			  ig2 = 0;

	     	 			     	 		}
	     	    	Transfer_cplt=0;
	     	    	 break;






	     	 	 case 23:
	     	 		// Status
	     	 		sprintf(buffer,"\n*** Valves and Igniters Status ***\r\n\n SOV1: %d\r\n SOV2: %d\r\n SOV3: %d\r\n SOV4: %d\r\n SOV5: %d\r\n SOV6: %d\r\n SOV7: %d\r\n SOV8: %d\r\n SOV9: %d\r\n IG1:  %d\r\n IG2:  %d\r\n", sov1, sov2, sov3, sov4, sov5, sov6, sov7, sov8, sov9, ig1, ig2);
	     	 		len =strlen(buffer);
	     	 		HAL_UART_Transmit(&huart2, buffer, len, 1000);

	     	 		//HAL_Delay(600);

	     	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
	     	 		break;

	     	 	 case 24:
	                 //Exiting the python script means turning off all the valves and igniters
	     	 		sov1 = 0, sov2= 0, sov3= 0, sov4= 0, sov5= 0, sov6= 0, sov7= 0, sov8= 0,sov9= 0, ig1= 0, ig2 =0;

	     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
	     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
	     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
	     	 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
	     	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);
	     	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);
	     	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 0);
	     	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 0);
	     	 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
	     	 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);

	     	 		Transfer_cplt=0; //Set to 0 to reset the transfer_complete variable
	     	 		//After setting the status we can proceed to turn low all the GPIOs used.
	     	 	    break;

	     	 	 default:
	     	 		Tx_serial("Please enter a valid command...\r\n");

	     	 }

	         }// end of if(Transfer_cplt)

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|SOV5_Pin|SOV4_Pin|SOV3_Pin
                          |SOV2_Pin|SOV1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, IG2_Pin|IG1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SOV9_Pin|SOV8_Pin|SOV7_Pin|SOV6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin SOV5_Pin SOV4_Pin SOV3_Pin
                           SOV2_Pin SOV1_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|SOV5_Pin|SOV4_Pin|SOV3_Pin
                          |SOV2_Pin|SOV1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : IG2_Pin IG1_Pin */
  GPIO_InitStruct.Pin = IG2_Pin|IG1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SOV9_Pin SOV8_Pin SOV7_Pin SOV6_Pin */
  GPIO_InitStruct.Pin = SOV9_Pin|SOV8_Pin|SOV7_Pin|SOV6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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
  __disable_irq();
  while (1)
  {
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
