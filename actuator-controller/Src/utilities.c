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
    if(state <= SAFETY && state >= SETUP_OPS)
    {
    	success = TRUE;
		switch(state)
		{
			case SETUP_OPS:
				break;
			//case LOX_FILL:
				break;
			case SITE_CLEAR_AND_START_UP:
				break;
			case PRESSURIZATION:
				break;
			case PRE_CHILL:
				break;
			case IGNITION:
				break;
			case LOX_INTRO:
				break;
			case BURN_INITIATED:
				break;
			case BURN_FEEDBACK:
				break;
			case BURN_TERMINATION_1:
				break;
			case BURN_TERMINATION_2:
				break;
			case BURN_TERMINATION_3:
				break;
			case PURGE:
				break;
			case POST_FIRE:
				break;
			//case SAFE_APPROACH:
				//break;
			case FAILURE:
				break;
			//case GROUNDSTATION:
				//break;
			//case TEST:
				//break;
			case SAFETY:
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
	//PV1
	vc |= (uint32_t)HAL_GPIO_ReadPin(GPIOA, GPIO8_Pin);

	//PV2
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOA, GPIO9_FAST_Pin) << 1);

	//PV3
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOA, GPIO10_FAST_Pin) << 2);

	//VV1
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, GPIO13_Pin) << 3);

	//VV2
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, GPIO14_Pin) << 4);

	//IV1
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, GPIO15_Pin) << 5);

	//IV2
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOD, GPIO8D8_Pin) << 6);

	//MV1
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOD, GPIO9_Pin) << 7);

	//MV2
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOD, GPIO2_Pin) << 8);

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
	//PV1
	(vs & (uint32_t)PV1) == (uint32_t)PV1 \
			? HAL_GPIO_WritePin(GPIOA, GPIO8_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, GPIO8_Pin,GPIO_PIN_RESET);

	//PV2
	(vs & (uint32_t)PV2) == (uint32_t)PV2 \
			? HAL_GPIO_WritePin(GPIOA, GPIO9_FAST_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, GPIO9_FAST_Pin,GPIO_PIN_RESET);

	//PV3
	(vs & (uint32_t)PV3) == (uint32_t)PV3 \
			? HAL_GPIO_WritePin(GPIOA, GPIO10_FAST_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, GPIO10_FAST_Pin,GPIO_PIN_RESET);

	//VV1
	(vs & (uint32_t)VV1) == (uint32_t)VV1 \
			? HAL_GPIO_WritePin(GPIOB, GPIO13_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, GPIO13_Pin,GPIO_PIN_RESET);

	//VV2
	(vs & (uint32_t)VV2) == (uint32_t)VV2 \
			? HAL_GPIO_WritePin(GPIOB, GPIO14_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, GPIO14_Pin,GPIO_PIN_RESET);

	//IV1
	(vs & (uint32_t)IV1) == (uint32_t)IV1 \
			? HAL_GPIO_WritePin(GPIOB, GPIO15_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, GPIO15_Pin,GPIO_PIN_RESET);

	//IV2
	(vs & (uint32_t)IV2) == (uint32_t)IV2 \
			? HAL_GPIO_WritePin(GPIOD, GPIO8D8_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOD, GPIO8D8_Pin,GPIO_PIN_RESET);

	//MV1
	(vs & (uint32_t)MV1) == (uint32_t)MV1 \
			? HAL_GPIO_WritePin(GPIOD, GPIO9_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOD, GPIO9_Pin,GPIO_PIN_RESET);

	//MV2
	(vs & (uint32_t)MV2) == (uint32_t)MV2 \
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

	volatile uint32_t success = FALSE;
	volatile uint8_t mask = 0xFF;

	volatile char rxb = (uint8_t)(hlpuart1->Instance->RDR & mask);

	if(RxMessageIdx < &RxMessageBuffer1[RX_BUFFER_SIZE])
	{
		*RxMessageIdx = rxb;
		RxMessageIdx+=1;
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

