
#include "main.h"

#include "state_machine.h"
#include "states_def.h"
#include "init.h"


int main(void)
{
  uint8_t serialRxBuffer[PRINT_BUFFER_SIZE];
  uint32_t isSafetyGreen = FALSE;
  uint32_t isProgramRunning = TRUE;
  HAL_Init();

  SystemClock_Config();
  //MX_WWDG_Init();
  MX_GPIO_Init();
  MX_ADC1_Init();

  //HAL_UART_Receive_IT(&hlpuart1,(uint8_t *)serialRxBuffer, 9);
  //HAL_UART_Transmit_IT(&hlpuart1,(uint8_t *)serialRxBuffer,9);

  while(1)
  {
	  StateMachine(isProgramRunning,SAFETY, isSafetyGreen);
  }
}
