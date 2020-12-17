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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
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
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

size_t accum = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

// Begin Katie's PFP

int mainCheck(int valveCState[], int valveExState[], int batteryState);

//this is the main for this part that will call all other functions
//Will return 0 if no errors were found
int checkState(int valveCState[], int valveExState[], int valvesInError[]);	//input is a placeholder as I don't know how we get valve info into function

//A valve state didn't match, so now we here
int errorFound(int valvesInError[]);

//Take read in valve states and make them usable
void fillStruct(int valveCState[], int valveExState[]);

//Main function for the checking
//It is expected that this is what a higher function calls to initiate check
int mainCheck(int valveCState[], int valveExState[], int batteryState);

//Just realized that I was just copying code in each error level
int isError(int valvesInError[], int errorLevel);

//Just turn on Buzzer
void buzzOn();

//just turn  off Buzzer
void buzzOff();

//Turn on a specific binary (top is MSB) on LED lights
void ledOn(int whichOn);

//Turn off all LEDs
void ledOff();

//Power failure has occured
void powerFailureState();

// End Katie's PFP

// Begin Tiffani's PFP

size_t SizeOfFile(char const *path);

void WriteFile(char const* fileName, char* buffer, size_t size);

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

	// Begin Tiffani's variable definitions

	int sensorState[10];

	char writeBuffer[11];

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

	int safetyState[] = {0, 0, 1, 1, 1, 1, 1, 1, 1};

	char *file = "valveStateData.txt";

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();

  /* USER CODE BEGIN 2 */

	HAL_Delay(1000); //a short delay is important to let the SD card settle

	//some variables for FatFs
	FATFS FatFs; 	//Fatfs handle

	//Open the file system
	f_mount(&FatFs, "", 1); // 1=mount now

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		// Begin Tiffani's main program

		sensorState[0] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_One_Pin);

		sensorState[1] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_Two_Pin);

		sensorState[2] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_Three_Pin);

		sensorState[3] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_Four_Pin);

		sensorState[4] = HAL_GPIO_ReadPin(GPIOB, Hall_Effect_Sensor_Five_Pin);

		sensorState[5] = HAL_GPIO_ReadPin(GPIOB, Hall_Effect_Sensor_Six_Pin);

		sensorState[6] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_Seven_Pin);

		sensorState[7] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_Eight_Pin);

		sensorState[8] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_Nine_Pin);

		sensorState[9] = 0;

		mainCheck(sensorState, safetyState, 0);

		for (int i = 0; i <= 9; i++)
		{
			writeBuffer[i] = (char)(sensorState[i] + 48);
		}

		WriteFile(file, writeBuffer, sizeof(writeBuffer));

		// End Tiffani's main program

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Buzzer_Pin|SD_CS_Pin|LED_Three_Pin|LED_Two_Pin
                          |LED_One_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : Hall_Effect_Sensor_Nine_Pin Hall_Effect_Sensor_One_Pin Hall_Effect_Sensor_Two_Pin Hall_Effect_Sensor_Three_Pin
                           Hall_Effect_Sensor_Four_Pin Hall_Effect_Sensor_Eight_Pin Hall_Effect_Sensor_Seven_Pin */
  GPIO_InitStruct.Pin = Hall_Effect_Sensor_Nine_Pin|Hall_Effect_Sensor_One_Pin|Hall_Effect_Sensor_Two_Pin|Hall_Effect_Sensor_Three_Pin
                          |Hall_Effect_Sensor_Four_Pin|Hall_Effect_Sensor_Eight_Pin|Hall_Effect_Sensor_Seven_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Buzzer_Pin LED_Three_Pin LED_Two_Pin LED_One_Pin */
  GPIO_InitStruct.Pin = Buzzer_Pin|LED_Three_Pin|LED_Two_Pin|LED_One_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_CS_Pin */
  GPIO_InitStruct.Pin = SD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Hall_Effect_Sensor_Six_Pin Hall_Effect_Sensor_Five_Pin */
  GPIO_InitStruct.Pin = Hall_Effect_Sensor_Six_Pin|Hall_Effect_Sensor_Five_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

// Begin Katie's PF's

int mainCheck(int valveCState[], int valveExState[], int batteryState)
{
	//For tracking error status, only says that we have an error
	int errorButton = 0;

	//Keeps track of which state of error that we are in
	//0 = all good, 1 = minor error, 2 = bigger error, 3 = major error, 4 = power failure
	int errorState = 0;

	//array keeps track of which valves are in error
	int valvesInError[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //0 = good, 1 = error

	valvesInError[9] = batteryState;

	errorButton = checkState(valveCState, valveExState, valvesInError);

	if(errorButton == 1)
		{
			errorState = errorFound(valvesInError);
			isError(valvesInError, errorState);
		}
		//clear any errors that may have been occurring
		if(errorButton == 0)
		{
			buzzOff();
			ledOff();
			errorState = 0;
		}

		return errorState;
	}

//returns error level (0-4)
int checkState(int valveCState[], int valveExState[], int valvesInError[]) {
	//If 1, then an error has occured
	int errorButton = 0;

	//checking each valve for an error
	if (valveCState[0] != valveExState[0]) {
		errorButton = 1;
		valvesInError[0] = 1;
	}
	if (valveCState[1] != valveExState[1]) {
		errorButton = 1;
		valvesInError[1] = 1;
	}
	if (valveCState[2] != valveExState[2]) {
		errorButton = 1;
		valvesInError[2] = 1;
	}
	if (valveCState[3] != valveExState[3]) {
		errorButton = 1;
		valvesInError[3] = 1;
	}
	if (valveCState[4] != valveExState[4]) {
		errorButton = 1;
		valvesInError[4] = 1;
	}
	if (valveCState[5] != valveExState[5]) {
		errorButton = 1;
		valvesInError[5] = 1;
	}
	if (valveCState[6] != valveExState[6]) {
		errorButton = 1;
		valvesInError[6] = 1;
	}
	if (valveCState[7] != valveExState[7]) {
		errorButton = 1;
		valvesInError[7] = 1;
	}
	if (valveCState[8] != valveExState[8]) {
		errorButton = 1;
		valvesInError[8] = 1;
	}
	if (valvesInError[9] == 1) {
		errorButton = 1;
	}

	return errorButton;
}

//when in doubt, error out
//returns a value depending on which level of error is found
int errorFound(int valvesInError[]) {
	int errorLevel = 0;
	//index - priority
// PV_L02		//4 - 1
// PV_L03		//5 - 2
// PV_F02		//2 - 3
// PV_L01		//3 - 4
// PV_F01		//1 - 5
// SOV_F01		//0 - 6
// PV_LF01		//8 - 7
// PV_P01		//6 - 8
// PV_P02		//7 - 9
//Error level is set to highest prior valve
	if (valvesInError[7])
		errorLevel = 1;
	if (valvesInError[6])
		errorLevel = 1;
	if (valvesInError[8])
		errorLevel = 1;
	if (valvesInError[0])
		errorLevel = 1;
	if (valvesInError[1])
		errorLevel = 2;
	if (valvesInError[3])
		errorLevel = 2;
	if (valvesInError[2])
		errorLevel = 2;
	if (valvesInError[5])
		errorLevel = 3;
	if (valvesInError[4])
		errorLevel = 3;
	if (valvesInError[9]) //power failure on battery, nothing else matters
		errorLevel = 4;

	return errorLevel;
}

int isError(int valvesInError[], int errorLevel) {
	//The buzzer doesn't get turned off until error is cleared
	buzzOn();

	if (errorLevel == 4) {
		powerFailureState(); //It is currently not expected that we can return from this state without a restart
		return 1;
	}

	int i = 0;
	for (i = 0; i < 6; ++i) {
		ledOn(errorLevel);
		HAL_Delay(1000);
		ledOff();
		HAL_Delay(1000);
	}

	for (i = 0; i < 9; ++i) {
		if (valvesInError[i] == 1) {
			ledOn(i);
			HAL_Delay(1000);
			ledOff();
		}
	}
	ledOn(errorLevel);
	return 0;
}

void buzzOn() {
	HAL_GPIO_WritePin(GPIOB, Buzzer_Pin, GPIO_PIN_SET);
}

void buzzOff() {
	HAL_GPIO_WritePin(GPIOB, Buzzer_Pin, GPIO_PIN_RESET);
}

void ledOn(int whichOn) {
	//6,7,8 all have same light error
	if ((whichOn == 7) || (whichOn == 8))
		whichOn = 6;
	++whichOn; //change from index to actual number displayed in binary on LEDs

	//add a check for bottom 3 prio as they all get same code
	//printf for debugging outside of board
	//will comment out in board testing
	printf("LED %d ON\n", whichOn);
	if (whichOn == 7) {
		HAL_GPIO_WritePin(GPIOB, LED_Three_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, LED_Two_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, LED_One_Pin, GPIO_PIN_SET);
	}
	if (whichOn == 6) {
		HAL_GPIO_WritePin(GPIOB, LED_Three_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, LED_Two_Pin, GPIO_PIN_SET);
	}
	if (whichOn == 5) {
		HAL_GPIO_WritePin(GPIOB, LED_Three_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, LED_One_Pin, GPIO_PIN_SET);
	}
	if (whichOn == 4) {
		HAL_GPIO_WritePin(GPIOB, LED_Three_Pin, GPIO_PIN_SET);
	}
	if (whichOn == 3) {
		HAL_GPIO_WritePin(GPIOB, LED_Two_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, LED_One_Pin, GPIO_PIN_SET);
	}
	if (whichOn == 2) {
		HAL_GPIO_WritePin(GPIOB, LED_Two_Pin, GPIO_PIN_SET);
	}
	if (whichOn == 1) {
		HAL_GPIO_WritePin(GPIOB, LED_One_Pin, GPIO_PIN_SET);
	}
}

void ledOff() {
	HAL_GPIO_WritePin(GPIOB, LED_One_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, LED_Two_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, LED_Three_Pin, GPIO_PIN_RESET);

}

//goes down 3-2-1 continually
void powerFailureState() {
	while (1) {
		ledOn(2);
		HAL_Delay(1000);
		ledOff();

		ledOn(1);
		HAL_Delay(1000);
		ledOff();

		ledOn(0);
		HAL_Delay(1000);
		ledOff();
	}
}

// End Katie's PF's

// Begin Tiffani's PF's

size_t SizeOfFile(char const *path)
{
    FILE *fp = fopen(path, "r");
    fseek(fp, 0, SEEK_END);
    size_t fsize = (size_t) ftell(fp);
    fclose(fp);
    return fsize;
}

void WriteFile(char const* fileName, char* buffer, size_t size)
{
    char Buffer_logger[11];
    memcpy(Buffer_logger, buffer, size);

    unsigned int BytesWr;
    FIL file; 		//File handle
    f_open(&file, fileName, FA_CREATE_ALWAYS | FA_WRITE | FA_CREATE_ALWAYS);

    while (SizeOfFile(fileName) == 0)
    {
        // Open log for writing
        f_open(&file, fileName, FA_WRITE);

        // Point to the EOF
        f_lseek(&file, accum);

        // Write to log
        f_write(&file, (const void*) Buffer_logger, size, &BytesWr);

        accum += accum + strlen(Buffer_logger);

        //Close file.
        f_close(&file);
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
