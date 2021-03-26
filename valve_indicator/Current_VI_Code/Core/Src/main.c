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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdarg.h> //for va_list var arg functions0
#include <stdlib.h>
#include <unistd.h>
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
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
//void myprintf(const char *fmt, ...);
FIL ApndThis(int valveCState[], int valveExState[], int batteryState, FRESULT fres, FIL fil);

//this is the main for this part that will call all other functions
//Will return 0 if no errors were found
int checkState(int valveCState[], int valveExState[], int valvesInError[]);	//input is a placeholder as I don't know how we get valve info into function

//A valve state didn't match, so now we here
int errorFound(int valvesInError[]);

//Take read in valve states and mak them usuable
void fillStruct(int valveCState[], int valveExState[]);

//Main function for the checking
//It is expected that this is what a higher function calls to initiate check
int mainCheck(int valveCState[], int valveExState[], int batteryState);

//Just realised that I was just copying code in each error level
int isError(int valvesInError[], int errorLevel);

//Just turn on Buzzer
void buzzOn();

//just turn  off Buzzer
void buzzOff();

//Turn on apecific binary (top is MSB) on LED lights
void ledOn(int whichOn);

//Turn off all LEDs
void ledOff();

//Power failure has occured
void powerFailureState();


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
void myprintf(const char *fmt, ...) {
  static char buffer[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  int len = strlen(buffer);
  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, -1);

}
*/

// Main function to run append
FIL ApndThis(int valveCState[], int valveExState[], int batteryState, FRESULT fres, FIL fil)
{
// SOV_F01		//0
// PV_F01		//1
// PV_F02		//2
// PV_L01		//3
// PV_L02		//4
// PV_L03		//5
// PV_P01		//6
// PV_P02		//7
// PV_LF01		//8

	  //Making a buffer to write of 128 bytes/characters
	  //Will overwrite for every line to write to card
	  BYTE readBuf[128];

//Unpacking can be removed if we run out of space on stm, it just makes it easier to read and debug
	  //unpacking actual states
	  int SOV_F01 = valveCState[0];
	  int PV_F01 = valveCState[1];
	  int PV_F02 = valveCState[2];
	  int PV_L01 = valveCState[3];
	  int PV_L02 = valveCState[4];
	  int PV_L03 = valveCState[5];
	  int PV_P01 = valveCState[6];
	  int PV_P02 = valveCState[7];
	  int PV_LF01 = valveCState[8];

	  //unpacking expected states
	  int SOV_F01ex = valveExState[0];
	  int PV_F01ex = valveExState[1];
	  int PV_F02ex = valveExState[2];
	  int PV_L01ex = valveExState[3];
	  int PV_L02ex = valveExState[4];
	  int PV_L03ex = valveExState[5];
	  int PV_P01ex = valveExState[6];
	  int PV_P02ex = valveExState[7];
	  int PV_LF01ex = valveExState[8];


	  //write dividing line
	  strncpy((char*)readBuf, "**********************************************************\n", 59);
	  UINT bytesWrote;
	  fres = f_write(&fil, readBuf, 59, &bytesWrote);
	  if(fres == FR_OK) {
//		myprintf("Wrote %i bytes to 'valvestate'!\r\n", bytesWrote);
	  } else {
//		myprintf("f_write error (%i)\r\n");
		//don't have UART2 (com port over usb to laptop) for myprint, currently don't have a way to communicate errors
		//Perhaps encode code errors to remaining LED light codes?
		//Say all on if SD card cannot be written to
	  }

	  //Timestamp
	  //Ticks are acting weird, working on figuring out RTC to catch hours/mins for func mode
	  int ticksSince = HAL_GetTick();
	  uint32_t theClock = HAL_RCC_GetHCLKFreq();
	  theClock = theClock/100;    //May just comment out and have straight ticks for test mode
	  int secondsTotal = (ticksSince/theClock);
	  int minutesPast = secondsTotal/60;
	  int secondsPast = secondsTotal%60;

	  snprintf((char*)readBuf, 40, "- %d Minutes | %d Seconds -                         \n", minutesPast, secondsPast);
	  fres = f_write(&fil, readBuf, 40, &bytesWrote);
	  if(fres == FR_OK) {
//		myprintf("Wrote %i bytes to 'valvestate.txt'!\r\n", bytesWrote);
	  } else {
//		myprintf("f_write error (%i)\r\n");
	  }

	  //write expected valves
	  snprintf((char*)readBuf, 119, "\n\nExpected Valve States:\nSOV_F01: %d\nPV_F01: %d\nPV_F02: %d\nPV_L01: %d\nPV_L02: %d\nPV_L03: %d\nPV_P01: %d\nPV_P02: %d\nPV_LF01: %d\n\n", SOV_F01ex, PV_F01ex, PV_F02ex, PV_L01ex, PV_L02ex, PV_L03ex, PV_P01ex, PV_P02ex, PV_LF01ex);
	  fres = f_write(&fil, readBuf, 119, &bytesWrote);
	  if(fres == FR_OK) {
//		myprintf("Wrote %i bytes to 'valvestate.txt'!\r\n", bytesWrote);
	  } else {
//		myprintf("f_write error (%i)\r\n");
	  }

	  //write current valves
	  snprintf((char*)readBuf, 118, "\nCurrent Valve States:\nSOV_F01: %d\nPV_F01: %d\nPV_F02: %d\nPV_L01: %d\nPV_L02: %d\nPV_L03: %d\nPV_P01: %d\nPV_P02: %d\nPV_LF01: %d\n\n", SOV_F01, PV_F01, PV_F02, PV_L01, PV_L02, PV_L03, PV_P01, PV_P02, PV_LF01);
	  fres = f_write(&fil, readBuf, 118, &bytesWrote);
	  if(fres == FR_OK) {
//		myprintf("Wrote %i bytes to 'valvestate.txt'!\r\n", bytesWrote);
	  } else {
//		myprintf("f_write error (%i)\r\n");
	  }

	  //if we are on battery power, we want to know
	  if (batteryState) //Battery state needs to either demount or not be an endless loop
	  {
		  strncpy((char*)readBuf, "*Currently on battery power!*\n", 30);
		  UINT bytesWrote;
		  fres = f_write(&fil, readBuf, 30, &bytesWrote);
		  if(fres == FR_OK) {
//			  myprintf("Wrote %i bytes to 'valvestate.txt'!\r\n", bytesWrote);
		  } else {
//			  myprintf("f_write error (%i)\r\n");
		  }
	  }



	  return fil;

};

int mainCheck(int valveCState[], int valveExState[], int batteryState)
{
	//For tracking error status, only says that we have an error
	int errorButton = 0;

	//Keeps trak of which state of error that we are in
	//0 = all good, 1 = minor error, 2 = bigger error, 3 = major error, 4 = power failure
	int errorState = 0;

	//array keeps track of which valves are in error
	int valvesInError[10]; //0 = good, 1 = error

	//initialising error array
	//10th index is for power
	int i = 0;
	for (i=0; i<9; ++i)
	{
		valvesInError[i] = 0;
	}
	valvesInError[9] = batteryState;

	errorButton = checkState(valveCState, valveExState, valvesInError);


	if(errorButton == 1)
	{
		errorState = errorFound(valvesInError);
		isError(valvesInError, errorState);
	}
	//clear any errors that may have been occuring
	if(errorButton == 0)
	{
		buzzOff();
		ledOff();
		errorState = 0;
	}

	return errorState;
}

//returns error level (0-4)
int checkState(int valveCState[], int valveExState[], int valvesInError[])
{
	//If 1, then an error has occured
	int errorButton = 0;

	//checking each valve for an error
	if(valveCState[0] != valveExState[0])
	{
		errorButton = 1;
		valvesInError[0] = 1;
	}
	if(valveCState[1] != valveExState[1])
	{
		errorButton = 1;
		valvesInError[1] = 1;
	}
	if(valveCState[2] != valveExState[2])
	{
		errorButton = 1;
		valvesInError[2] = 1;
	}
	if(valveCState[3] != valveExState[3])
	{
		errorButton = 1;
		valvesInError[3] = 1;
	}
	if(valveCState[4] != valveExState[4])
	{
		errorButton = 1;
		valvesInError[4] = 1;
	}
	if(valveCState[5] != valveExState[5])
	{
		errorButton = 1;
		valvesInError[5] = 1;
	}
	if(valveCState[6] != valveExState[6])
	{
		errorButton = 1;
		valvesInError[6] = 1;
	}
	if(valveCState[7] != valveExState[7])
	{
		errorButton = 1;
		valvesInError[7] = 1;
	}
	if(valveCState[8] != valveExState[8])
	{
		errorButton = 1;
		valvesInError[8] = 1;
	}
	if(valvesInError[9] == 1)
	{
		errorButton = 1;
	}

	return errorButton;
}

//when in doubt, error out
//returns a value depending on which level of error is found
int errorFound(int valvesInError[])
{
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
//Error level is set to highest prio valve
	if(valvesInError[7])
		errorLevel = 1;
	if(valvesInError[6])
		errorLevel = 1;
	if(valvesInError[8])
		errorLevel = 1;
	if(valvesInError[0])
		errorLevel = 1;
	if(valvesInError[1])
		errorLevel = 2;
	if(valvesInError[3])
		errorLevel = 2;
	if(valvesInError[2])
		errorLevel = 2;
	if(valvesInError[5])
		errorLevel = 3;
	if(valvesInError[4])
		errorLevel = 3;
	if(valvesInError[9]) //power failure and on battery, fix this first
		errorLevel = 4;

	return errorLevel;
}

int isError(int valvesInError[], int errorLevel)
{
	//The buzzer doesn't get turned off until error is cleared
	buzzOn();

	if (errorLevel == 4)
	{
		powerFailureState();  //It is currently not expected that we can return from this state without a restart
		return 1;
	}
	//add a loss of power error with higher priority later

  int i = 0;
	for(i=0; i<6; ++i)
	{
		ledOn(errorLevel);
		HAL_Delay(1000);
//		sleep(1);
		ledOff();
		HAL_Delay(1000);
//		sleep(1);
	}

	for(i=0; i<9; ++i)
	{
		if(valvesInError[i] == 1)
		{
			ledOn(i);
			HAL_Delay(1000);
//			sleep(1);
			ledOff();
		}
	}
 return 0;
}

void buzzOn()
{
	//printf for debugging outside of board
	//will comment out in board testing
//	myprintf("\nBuzzer ON\n");
	HAL_GPIO_WritePin(GPIOB, Buzzer_Pin, GPIO_PIN_SET);
}

void buzzOff()
{
	//printf for debugging outside of board
	//will comment out in board testing
//	myprintf("\nBuzzer OFF\n");
	HAL_GPIO_WritePin(GPIOB, Buzzer_Pin, GPIO_PIN_RESET);
}

//need to add pin pushing, for now just print
void ledOn(int whichOn)
{
	int led3 = 0;
	int led2 = 0;
	int led1 = 0;
	int led0 = 0;

	++whichOn; //change from index to actual number displayed in binary on LEDs

	//decoding into binary values on leds
	if(whichOn == 1)
	{
		led0 = 1;
	}
	if(whichOn == 2)
	{
		led1 = 1;
	}
	if(whichOn == 3)
	{
		led0 = 1;
		led1 = 1;
	}
	if(whichOn == 4)
	{
		led2 = 1;
	}
	if(whichOn == 5)
	{
		led0 = 1;
		led2 = 1;
	}
	if(whichOn == 6)
	{
		led1 = 1;
		led2 = 1;
	}
	if(whichOn == 7)
	{
		led0 = 1;
		led1 = 1;
		led2 = 1;
	}
	if(whichOn == 8)
	{
		led3 = 1;
	}
	if(whichOn == 9)
	{
		led0 = 1;
		led3 = 1;
	}
	if(whichOn == 15) //All LEDS ON
	{
		led0 = 1;
		led1 = 1;
		led2 = 1;
		led3 = 1;
	}

	//will be changed to pin pushing
//	myprintf(" LED %d ON\n", whichOn);
//	myprintf(" LED3 : [%d] |", led3);
//	myprintf(" LED2 : [%d] |", led2);
//	myprintf(" LED1 : [%d] |", led1);
//	myprintf(" LED0 : [%d]\n", led0);
	if(led0) HAL_GPIO_WritePin(GPIOB, LED0_Pin, GPIO_PIN_SET);
	if(led1) HAL_GPIO_WritePin(GPIOB, LED1_Pin, GPIO_PIN_SET);
	if(led2) HAL_GPIO_WritePin(GPIOB, LED2_Pin, GPIO_PIN_SET);
	if(led3) HAL_GPIO_WritePin(GPIOB, LED3_Pin, GPIO_PIN_SET);

}

//Set all leds off when switching to prevent sticking on
void ledOff()
{
	//printf for debugging outside of board
	//change to pin pushing to set all 4 LEDs OFF
//	myprintf("\nLEDs OFF\n\n");
	HAL_GPIO_WritePin(GPIOB, LED0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, LED1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, LED2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, LED3_Pin, GPIO_PIN_RESET);
}

//goes down 4-3-2-1 continually
void powerFailureState()
{
	while(1)
	{
		ledOn(1);
		HAL_Delay(1000);
//		sleep(1);
		ledOff();

		ledOn(3);
		HAL_Delay(1000);
//		sleep(1);
		ledOff();

		ledOn(7);
		HAL_Delay(1000);
//		sleep(1);
		ledOff();

		ledOn(15);
		HAL_Delay(1000);
//		sleep(1);
		ledOff();
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
	int sensorState[10];

//	char writeBuffer[11];
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
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  HAL_Delay(1000); //a short delay is important to let the SD card settle

  //some variables for FatFs
  FATFS FatFs; 	//Fatfs handle
  FIL fil; 		//File handle
  FRESULT fres; //Result after operations

  //Open the file system
  fres = f_mount(&FatFs, "", 1); //1=mount now
  if (fres != FR_OK) {
//	myprintf("f_mount error (%i)\r\n", fres);
	while(1);
  }

  //Let's get some statistics from the SD card
  DWORD free_clusters, free_sectors, total_sectors;

  FATFS* getFreeFs;

  fres = f_getfree("", &free_clusters, &getFreeFs);
  if (fres != FR_OK) {
//	myprintf("f_getfree error (%i)\r\n", fres);
	while(1);
  }

  //Formula comes from ChaN's documentation
  total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
  free_sectors = free_clusters * getFreeFs->csize;

//  myprintf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
//  myprintf("Hello, I push and pull stuff into the sd cards\n:D\n\n");

//  myprintf("SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);

  //Create (rewrite) a file to hold all the state stuff
  fres = f_open(&fil, "valve.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
  if(fres == FR_OK) {
//	myprintf("\nWas able to open/create valvestate.txt\r\n");
  } else {
//	myprintf("f_open error (%i)\r\n", fres);
  }

//***START TESTING***//
//Have a diffrent file open here so that we can write a test file before a seperate functional file
  //This is where we actually write to sd and run states
  //for now this is just a testbench
  //funtional veriosn will be a loop that checks actual valve states
  int batteryState = 0;
  int valveCState[9];
  int valveExState[9];

  //initialise and check everything good
  int i = 0;
  	for (i=0; i<9; ++i)
  	{
  		valveCState[i] = 0;
  	}

  	i = 0;
  	for (i=0; i<9; ++i)
  	{
  		valveExState[i] = 0;
  	}

  	fil = ApndThis(valveCState,valveExState,batteryState, fres, fil);
  	mainCheck(valveCState,valveExState,batteryState);

  	//check low prio valve wrong
  	valveCState[7] = 1;

  	fil = ApndThis(valveCState,valveExState,batteryState, fres, fil);
  	mainCheck(valveCState,valveExState,batteryState);

  	//check two wrong valves
    valveCState[2] = 1;

  	fil = ApndThis(valveCState,valveExState,batteryState, fres, fil);
  	mainCheck(valveCState,valveExState,batteryState);

  	//check all wrong valves
  	i = 0;
  	for (i=0; i<9; ++i)
  	{
  		valveCState[i] = 1;
  	}

  	fil = ApndThis(valveCState,valveExState,batteryState, fres, fil);
  	mainCheck(valveCState,valveExState,batteryState);

  	//check battery state set with any wrong valves
//  	batteryState = 1;

//  	ApndThis(valveCState,valveExState,batteryState, fres, fil);
//  	mainCheck(valveCState,valveExState,batteryState);

  	//checking all errors now cleared
  	batteryState = 0;
  	i = 0;
  	for (i=0; i<9; ++i)
  	{
  		valveCState[i] = 0;
  	}

  	fil = ApndThis(valveCState,valveExState,batteryState, fres, fil);
  	mainCheck(valveCState,valveExState,batteryState);

//***END TESTING***//

  //We're done, so de-mount the drive

	//Close the file
  f_close(&fil);
  f_mount(NULL, "", 0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int safetyState[] = {0,0,0,0,0,0,0,0,0}; //Just here for testing
  while (1)
  {
//functional mode, still in work (right now can be used to check HES, no SD card func)
//**NOTE**//
//If you wait for SD card test to finish, this can then be tested (Is not different from practicum version)
	  //Make a function to do this, block in main no good
	  sensorState[0] = HAL_GPIO_ReadPin(GPIOA, SOV_F01_Pin);
	  sensorState[1] = HAL_GPIO_ReadPin(GPIOA, PV_F01_Pin);
	  sensorState[2] = HAL_GPIO_ReadPin(GPIOA, PV_F02_Pin);
	  sensorState[3] = HAL_GPIO_ReadPin(GPIOA, PV_L01_Pin);
	  sensorState[4] = HAL_GPIO_ReadPin(GPIOB, PV_L02_Pin);
	  sensorState[5] = HAL_GPIO_ReadPin(GPIOB, PV_L03_Pin);
	  sensorState[6] = HAL_GPIO_ReadPin(GPIOA, PV_P01_Pin);
	  sensorState[7] = HAL_GPIO_ReadPin(GPIOA, PV_P02_Pin);
	  sensorState[8] = HAL_GPIO_ReadPin(GPIOA, PV_LF01_Pin);
	  sensorState[9] = 0; //Will later be used to check battery state

	  mainCheck(sensorState, safetyState, 0);

//**WORKING FUNC MODE ALGORITHM**//
	  //Wait for UART message of state change (Act like interrupt) or power failure
	  //If no interrupt, check ever minute or so (easy to change with HALdelay)
	  //Grab either valve states or decode given states to ex valve states (don't currently know what is coming over UART)
	  //Wait 2-3 seconds for valves to change (depending on how long valves take on average or slowest)
	  //Catch c valve states
	  //Write SD card (incase of power failure)
	  //Run mainCheck (catch errorButton)
	  //If errorButton, then loop from catch c valve states
	  //Else, all good, restart whole loop
	  //Need a signal over UART for when to break loop and demount SD card
//**END ALGORITHM**//

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		 HAL_Delay(1000);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
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
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_MultiProcessor_Init(&huart1, 0, UART_WAKEUPMETHOD_IDLELINE) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Buzzer_Pin|SD_CS_Pin|LED3_Pin|LED2_Pin
                          |LED1_Pin|LED0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PV_LF01_Pin SOV_F01_Pin PV_F01_Pin PV_F02_Pin
                           PV_L01_Pin PV_P02_Pin PV_P01_Pin */
  GPIO_InitStruct.Pin = PV_LF01_Pin|SOV_F01_Pin|PV_F01_Pin|PV_F02_Pin
                          |PV_L01_Pin|PV_P02_Pin|PV_P01_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Buzzer_Pin SD_CS_Pin LED3_Pin LED2_Pin
                           LED1_Pin LED0_Pin */
  GPIO_InitStruct.Pin = Buzzer_Pin|SD_CS_Pin|LED3_Pin|LED2_Pin
                          |LED1_Pin|LED0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PV_L03_Pin PV_L02_Pin */
  GPIO_InitStruct.Pin = PV_L03_Pin|PV_L02_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
