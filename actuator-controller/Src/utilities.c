// utilities.c
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//  ... bad at logging, check GIT
//  -Added UART_RecieveMessage functionality 12/3/2020 [APJ]
//
//	This file contains methods for verification and
//  manipulation of state changes
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include "utilities.h"

/*
 * 	Function VerifyState()
 *	TODO: Finish this description
 *
 *  Params:
 *  	state <int>: State to be verified
 *
 *  Returns:
 *  	success <uint32_t>: TRUE | FALSE | 1 | 0
 *
 *  Notes:
 */
uint32_t VerifyState(int state)
{
	uint32_t success = FALSE;
    if(state <= TEST && state >= SAFETY)
    {
    	success = TRUE;
		switch(state)
		{
			case SAFETY:
				break;
			case VALVE_CHECK:
				break;
			case FUEL_FILL:
				break;
			case LOX_PRE_FILL:
				break;
			case LOX_CHILL:
				break;
			case LOX_FILL:
				break;
			case READY_STATE:
				break;
			case PRESSURIZE:
				break;
			case IGNITION:
				break;
			case LOX_START:
				break;
			case FUEL_START:
				break;
			case FIRING:
				break;
			case PURGE:
				break;
			case LOX_BOILOFF:
				break;
			case SHUTDOWN:
				break;
			case FUEL_DRAIN:
				break;
			case FAILURE:
				break;
			case TEST:
				break;


			default:
				success = FALSE;
				break;
		}
    }
	return success;
}

/*
 * 	Function StateConfiguration()
 *	TODO: Finish this description
 *
 *  Params:
 *  	NONE
 *
 *  Returns:
 *  	vc <uint32_t>: Current reported valve states
 *
 *  Notes:
 */

uint32_t StateConfiguration()
{
	uint32_t vc = 0;
	//SOV1
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOA, GPIO10_FAST_Pin));

	//SOV2
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOD, GPIO8D8_Pin) << 1);

	//SOV3
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOD, GPIO9_Pin) << 2);

	//SOV4
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOC, GPIO7_Pin) << 3);

	//SOV5
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOC, GPIO6_Pin) << 4);

	//SOV6
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOA, GPIO9_FAST_Pin) << 5);

	//SOV7
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOA, GPIO8_Pin) << 6);

	//SOV8
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, GPIO14_Pin) << 7);

	//XXX1
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, GPIO13_Pin) << 8);

	//XXX2
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, GPIO15_Pin) << 9);

	//XXX3
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOD, GPIO2_Pin) << 10);

	return vc;
}

/*
 * 	Function ValveSetter()
 *	TODO: Finish this description
 *
 *  Params:
 *  vs <uint32_t>: Valve settings to be set
 *
 *  Returns:
 *  success <uint32_t>: TRUE | FALSE | 1 | 0
 *
 *  Notes:
 */

uint32_t ValveStateSetter(uint32_t vs)
{
	uint32_t success = FALSE;
	//SOV1
	(vs & (uint32_t)SOV1) == (uint32_t)SOV1 \
			? HAL_GPIO_WritePin(GPIOA, GPIO10_FAST_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, GPIO10_FAST_Pin,GPIO_PIN_RESET);

	//SOV2
	(vs & (uint32_t)SOV2) == (uint32_t)SOV2 \
			? HAL_GPIO_WritePin(GPIOD, GPIO8D8_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOD, GPIO8D8_Pin,GPIO_PIN_RESET);

	//SOV3
	(vs & (uint32_t)SOV3) == (uint32_t)SOV3 \
			? HAL_GPIO_WritePin(GPIOD, GPIO9_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOD, GPIO9_Pin,GPIO_PIN_RESET);

	//SOV4
	(vs & (uint32_t)SOV4) == (uint32_t)SOV4 \
			? HAL_GPIO_WritePin(GPIOC, GPIO7_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOC, GPIO7_Pin,GPIO_PIN_RESET);

	//SOV5
	(vs & (uint32_t)SOV5) == (uint32_t)SOV5 \
			? HAL_GPIO_WritePin(GPIOC, GPIO6_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOC, GPIO6_Pin,GPIO_PIN_RESET);

	//SOV6
	(vs & (uint32_t)SOV6) == (uint32_t)SOV6 \
			? HAL_GPIO_WritePin(GPIOA, GPIO9_FAST_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, GPIO9_FAST_Pin,GPIO_PIN_RESET);

	//SOV7
	(vs & (uint32_t)SOV7) == (uint32_t)SOV7 \
			? HAL_GPIO_WritePin(GPIOA, GPIO8_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, GPIO8_Pin,GPIO_PIN_RESET);

	//SOV8
	(vs & (uint32_t)SOV8) == (uint32_t)SOV8 \
			? HAL_GPIO_WritePin(GPIOB, GPIO14_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, GPIO14_Pin,GPIO_PIN_RESET);

	//XXX1
	(vs & (uint32_t)XXX1) == (uint32_t)XXX1 \
			? HAL_GPIO_WritePin(GPIOB, GPIO13_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, GPIO13_Pin,GPIO_PIN_RESET);

	//XXX2
	(vs & (uint32_t)XXX2) == (uint32_t)XXX2 \
			? HAL_GPIO_WritePin(GPIOB, GPIO15_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, GPIO15_Pin,GPIO_PIN_RESET);

	//XXX3
	(vs & (uint32_t)XXX3) == (uint32_t)XXX3 \
			? HAL_GPIO_WritePin(GPIOD, GPIO2_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOD, GPIO2_Pin,GPIO_PIN_RESET);

	success = (vs == StateConfiguration() ? TRUE : FALSE);

	return success;
}

/*
 * 	Function uint32_t UART_SendMessage(UART_HandleTypeDef *hlpuart1, char *message)
 *	TODO: Finish this description
 *
 *  Params:
 *  hlpuart <pointer>: Pointer the the lpuart typedef created at initialization.
 *	message <point>: Pointer to TxMessageBufferX (if multiple buffers defined X = integer, default= 1)
 *
 *  Returns:
 *  	success <uint32_t>: TRUE | FALSE | 1 | 0
 *  		If message is shorter than the maximum length and created, return TRUE, else
*  			return FALSE and send appropriate error message.
 *
 *  Notes:
 */
uint32_t UART_SendMessage(UART_HandleTypeDef *hlpuart1, char *message)
{

	uint32_t ofc = 0;
	uint32_t msg_time = HAL_GetTick();
	uint32_t success = FALSE;
	char *invalidLengthMessage = "Error: Log Message Overflow";
	char *noMessage = "Error: Log Message Empty";
	char time_str[11];


	memset(time_str, '\0', 11);

	sprintf(time_str, "%d",(int)msg_time);

	ofc += strlen("\nA5A5 ") + strlen(time_str) + strlen(" : ") + strlen(message) ;

	char transmit[ofc];
	memset(transmit, '\0', ofc);

	strcat(transmit, "\nA5A5 ");
	strcat(transmit, time_str);
	strcat(transmit, " : ");
	if(message[0] != '\0')
	{
		if(ofc < VALVE_STATE_BUFFER_SIZE)
		{
			strcat(transmit, message);
			success = TRUE;
		}else
		{
			strcpy(transmit,invalidLengthMessage);
		}
	}else
	{
		strcpy(transmit,noMessage);
	}
	memset(message, '\0', TX_BUFFER_SIZE);
	strcpy(message, transmit);
	HAL_UART_Transmit_IT(hlpuart1,(uint8_t *)message, sizeof(transmit));

	return success;
}

uint32_t UART_RecieveMessage(UART_HandleTypeDef *hlpuart1)
{
	/*
	 *  Called during IRQ interrupts if USART_ISR RXFNE or RXNE flag is set.
	 *  Reads one byte from hlpuart1 RDR register and adds it to the Receiver Buffer.
	 *  Sets the Data in Buffer flag in RxTxFlags (0x1)
	 *
	 *  Params:
	 *  hlpuart <pointer>: Pointer the the lpuart typedef created at initialization.
	 *
	 *  Returns:
	 *  success <uint32_t>: TRUE | FALSE | 1 | 0
	 *
	 *  Notes:
	 * 		0x4000 8000 - 0x4000 83FF (ADDR Range for LPUART1)
	 * 		1C (offset for USART_ISR)
	 * 		USART_ISR_RXNE_RXFNE 0x20
	 * 		USART_ISR_RXNE_RXFNE_Msk !0x20
	 */

	uint32_t success = FALSE;
	uint8_t mask = 0xFF;

	char rxb = (uint8_t)(hlpuart1->Instance->RDR & mask);

	if(RxMessageIdx < &RxMessageBuffer1[RX_BUFFER_SIZE])
	{
		*RxMessageIdx = rxb;
		RxMessageIdx+=1;
		RxTxFlags |= 0x1;
		success = TRUE;
	}
	if(RxMessageIdx == &RxMessageBuffer1[RX_BUFFER_SIZE]) RxMessageIdx = RxMessageBuffer1;
	printf("%c",RxMessageBuffer1[0]);

    SET_BIT(hlpuart1->Instance->CR1, 0x20);
	return success;
}

/* Function: uint32_t StateInitialize(enum StateName new, struct StateVars *ctrl)
 *
 * - Initialize state: Resets stateCounter, Logs entry time, set's new state and alst state
 *   attempts to set valves
 *
 *  Params:
 *  hlpuart <pointer>: Pointer the the lpuart typedef created at initialization.
 *
 *  Returns:
 *  success <uint32_t>: TRUE | FALSE | 1 | 0
 *
 *  Notes:
 */
uint32_t StateInitialize(struct StateVars *ctrl)
{
	uint32_t success = FALSE;
	// Reset state counter, log start
	ctrl->stateCounter = 0;
	ctrl->timeStarted = HAL_GetTick();;

	// Change State conditions
	ctrl->lastState = ctrl->currentState;

	// Set Valve States
	ValveStateSetter(ctrl->valveTarget);
	ctrl->valveConfiguration = StateConfiguration();

	success = SendStatusMessage(ctrl);
	return success;
}

