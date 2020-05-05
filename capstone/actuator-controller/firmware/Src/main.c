
#include "main.h"

#include "state_machine.h"
#include "states_def.h"
#include "init.h"


int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_LPTIM1_Init();
  MX_LPUART1_UART_Init();

  while (1)
  {
	  StateMachine(TRUE,SAFETY);
  }
}
