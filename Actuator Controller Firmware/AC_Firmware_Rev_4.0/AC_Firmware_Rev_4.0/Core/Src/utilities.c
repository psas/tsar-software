// utilities.c
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This file contains methods for verification and
//  manipulation of state changes
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#include "utilities.h"
#include "messages.h"
#include "valve_check.h"

//Function that sends the current state status to the user
void StateStatus(struct StateVars *ctrl){

     int state = ctrl->currentState;

	const char* naming[] = { [0] = "State : Start Handler\n", [1] = "State : Valve Check\n" ,	\
		[2] = "State : Lox Pre Fill\n", [3] = "State : Lox Fill\n", [4] = "State : Lox Chill\n",	\
		[5] = "State : Safety\n", [6] = "State : Pressurize\n", [7] = "State : Ignition\n",	[8] = "State : Oxidizer Start\n",\
		[9] = "State : Firing\n", [10] = "State : Purge\n",	[11] = "State : Turn Off\n", [12] = "State : Automatic Shutdown\n"};

		if (ctrl->lastState  != ctrl->currentState){
			UART_SendMessage(&huart2, naming[state]);
			UART_SendMessage_VI(&huart3, naming[state]);

			// Flag for the transitioning to the same state
			retransition = FALSE;
	        }
		//for the status command
		else if(status)
		{
			UART_SendMessage(&huart2, naming[state]);
			//UART_SendMessage_VI(&huart3, naming[state]);
			status = FALSE;

		}
}// end of function



/*
 * 	Function VerifyState()
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
    if(state <= TURN_OFF && state >= START_HANDLER)
    {
    	success = TRUE;
		switch(state)
		{

			case START_HANDLER:
				break;
			case VALVE_CHECK:
				break;
			case LOX_PRE_FILL:
				break;
			case LOX_FILL:
				break;
			case LOX_CHILL:
				break;
			case SAFETY:
				break;
			case PRESSURIZE:
				break;
			case IGNITION:
				break;
			case OXIDIZER_START:
				break;
			case FIRING:
				break;
			case PURGE:
				break;
			case AUTOMATIC_SHUTDOWN:
				break;
			case TURN_OFF:
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
 *
 *  Params:
 *  	NONE
 *
 *  Returns:
 *  	vc <uint32_t>: Current reported valve states
 *
 *  Notes:
 */


// |= will perform bitwise OR on the two operands then 1 << 1 = 0010    0 << 1 = 0000
// store the result in the left  operator.

// 1 << 0 = 0001    0 << 0 = 0000
// 1 << 2 = 0100    0 << 2 = 0000

// EX- if vc = 0001 then 0001 | 0010 , vc = 0011


uint32_t StateConfiguration()
{
	uint32_t vc = 0;
	//SOV1 - Fuel Pressure (PA_11)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin) << 0);

	//SOV2 - Lox Pressure (PA_12)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin) << 1);

	//SOV3 - Fuel Purge N2 (PC_7)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin) << 2);

	//SOV4 - Fuel Vent (PC_6)
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOC, FUEL_VENT_Pin) << 3);

	//SOV5 - Fuel Main (PA_9)
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOA, FUEL_MAIN_Pin) << 4);

	//SOV6 - Lox Main (PA_8)
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOA, LOX_MAIN_Pin) << 5);

	//SOV7 - Lox Fill (PB_15)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin) << 6);

	//SOV8 - Lox Vent (PB_10)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin) << 7);

	//SOV9 - Lox Purge (PB_13)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin) << 8);

	//IG1 - Igniter (PA_10)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) << 9);

	return vc;
}

/*
 * 	Function ValveSetter()
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
			? HAL_GPIO_WritePin(GPIOA, FUEL_PRESSURE_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, FUEL_PRESSURE_Pin,GPIO_PIN_RESET);

	//SOV2
	(vs & (uint32_t)SOV2) == (uint32_t)SOV2 \
			? HAL_GPIO_WritePin(GPIOA, LOX_PRESSURE_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, LOX_PRESSURE_Pin,GPIO_PIN_RESET);

	//SOV3
	(vs & (uint32_t)SOV3) == (uint32_t)SOV3 \
			? HAL_GPIO_WritePin(GPIOC, FUEL_PURGE_N2_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOC, FUEL_PURGE_N2_Pin,GPIO_PIN_RESET);

	//SOV4
	(vs & (uint32_t)SOV4) == (uint32_t)SOV4 \
			? HAL_GPIO_WritePin(GPIOC, FUEL_VENT_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOC, FUEL_VENT_Pin,GPIO_PIN_RESET);

	//SOV5
	(vs & (uint32_t)SOV5) == (uint32_t)SOV5 \
			? HAL_GPIO_WritePin(GPIOA, FUEL_MAIN_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, FUEL_MAIN_Pin,GPIO_PIN_RESET);

	//SOV6
	(vs & (uint32_t)SOV6) == (uint32_t)SOV6 \
			? HAL_GPIO_WritePin(GPIOA, LOX_MAIN_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, LOX_MAIN_Pin,GPIO_PIN_RESET);

	//SOV7
	(vs & (uint32_t)SOV7) == (uint32_t)SOV7 \
			? HAL_GPIO_WritePin(GPIOB, LOX_FILL_Pin ,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, LOX_FILL_Pin,GPIO_PIN_RESET);

	//SOV8
	(vs & (uint32_t)SOV8) == (uint32_t)SOV8 \
			? HAL_GPIO_WritePin(GPIOB, LOX_VENT_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, LOX_VENT_Pin,GPIO_PIN_RESET);

	//SOV9
	(vs & (uint32_t)SOV9) == (uint32_t)SOV9 \
			? HAL_GPIO_WritePin(GPIOB, LOX_PURGE_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, LOX_PURGE_Pin,GPIO_PIN_RESET);

	//IG1
	(vs & (uint32_t)IG1) == (uint32_t)IG1 \
			? HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,GPIO_PIN_RESET);


	success = (vs == StateConfiguration() ? TRUE : FALSE);

	return success;
}


/* Function: uint32_t StateInitialize(enum StateName new, struct StateVars *ctrl)
 *
 * - Initialize state: Resets stateCounter, Logs entry time, set's new state and last state
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

	///success = SendStatusMessage(ctrl);
	return success;
}


//Function to handle individual valves (Part of the Valve check state)
void ValveCheckCmd(struct StateVars *ctrl){

	uint32_t success = TRUE;

    	   if (command == IGON){
				if (cmd_flag && !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)){
					valves_selected |= (uint16_t)IG1;
					UART_SendMessage(&huart2, " Igniter is ON...\n");
					UART_SendMessage_VI(&huart3, " Igniter is ON...\n");
					cmd_flag = FALSE;
					ignit = 1;
				}
				else if (cmd_flag && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10))
				{
					UART_SendMessage(&huart2, " Igniter is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == IGOFF){
				if (cmd_flag && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)){
					valves_selected &= 0xFF;
					UART_SendMessage(&huart2, " Igniter is OFF...\n");
					UART_SendMessage_VI(&huart3, " Igniter is OFF...\n");
					cmd_flag = FALSE;
					ignit = 0;

				}
				else if (cmd_flag && !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)){
					UART_SendMessage(&huart2, " Igniter is already OFF...\n");
					cmd_flag = FALSE;
				}

			}
			else if(command == SOV1ON){
				if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin))){
					valves_selected |= (uint16_t)SOV1;
					UART_SendMessage(&huart2, " SOV1 is ON...\n");
					UART_SendMessage_VI(&huart3, " SOV1 is ON...\n");
					cmd_flag = FALSE;
					v1 =1;
				}
				else if (cmd_flag && (HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin))){
					UART_SendMessage(&huart2, " SOV1 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == SOV1OFF){
				if (cmd_flag && (HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin))){
					valves_selected &= 0x3FE;
					UART_SendMessage(&huart2, " SOV1 is OFF...\n");
					UART_SendMessage_VI(&huart3, " SOV1 is OFF...\n");
					cmd_flag = FALSE;
					v1=0;
				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin))){
					UART_SendMessage(&huart2, " SOV1 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == SOV2ON){
				if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin))){
					valves_selected |= (uint16_t)SOV2;
					UART_SendMessage(&huart2, " SOV2 is ON...\n");
					UART_SendMessage_VI(&huart3, " SOV2 is ON...\n");
					cmd_flag = FALSE;
					v2=1;

				}
				else if (cmd_flag && (HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin))){
					UART_SendMessage(&huart2, " SOV2 is already ON...\n");
					cmd_flag = FALSE;
				}

			}
			else if (command == SOV2OFF){
				if (cmd_flag && (HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin))){
					valves_selected &= 0x1FD;
					UART_SendMessage(&huart2, " SOV2 is OFF...\n");
					UART_SendMessage_VI(&huart3, " SOV2 is OFF...\n");
					cmd_flag = FALSE;
					v2=0;
				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin))){
					UART_SendMessage(&huart2, " SOV2 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == SOV3ON){
				if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin))){
					valves_selected |= (uint16_t)SOV3;
					UART_SendMessage(&huart2, " SOV3 is ON...\n");
					UART_SendMessage_VI(&huart3, " SOV3 is ON...\n");
					cmd_flag = FALSE;
					v3=1;
				}
				else if (cmd_flag && HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin)){
					UART_SendMessage(&huart2, " SOV3 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == SOV3OFF){
				if (cmd_flag && HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin)){
					valves_selected &= 0x3FB;
					UART_SendMessage(&huart2, " SOV3 is OFF...\n");
					UART_SendMessage_VI(&huart3, " SOV3 is OFF...\n");
					cmd_flag = FALSE;
					v3=0;

				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin))) {
					UART_SendMessage(&huart2, " SOV3 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == SOV4ON){
				if (cmd_flag && !(HAL_GPIO_ReadPin (GPIOC, FUEL_VENT_Pin))){
					valves_selected |= (uint16_t)SOV4;
					UART_SendMessage(&huart2, " SOV4 is ON...\n");
					UART_SendMessage_VI(&huart3, " SOV4 is ON...\n");
					cmd_flag = FALSE;
					v4=1;

				}
				else if (cmd_flag && HAL_GPIO_ReadPin (GPIOC, FUEL_VENT_Pin)){
					UART_SendMessage(&huart2, " SOV4 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == SOV4OFF){


				if (cmd_flag && HAL_GPIO_ReadPin (GPIOC, FUEL_VENT_Pin)){
					valves_selected &= 0x3F7;
					UART_SendMessage(&huart2, " SOV4 is OFF...\n");
					UART_SendMessage_VI(&huart3, " SOV4 is OFF...\n");
					cmd_flag = FALSE;
					v4=0;

				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin (GPIOC, FUEL_VENT_Pin))){
					UART_SendMessage(&huart2, " SOV4 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == SOV5ON){
				if (cmd_flag && !(HAL_GPIO_ReadPin (GPIOA, FUEL_MAIN_Pin))){
					valves_selected |= (uint16_t)SOV5;
					UART_SendMessage(&huart2, " SOV5 is ON...\n");
					UART_SendMessage_VI(&huart3, " SOV5 is ON...\n");
					cmd_flag = FALSE;
					v5=1;
				}
				else if(cmd_flag && HAL_GPIO_ReadPin (GPIOA, FUEL_MAIN_Pin)){
					UART_SendMessage(&huart2, " SOV5 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == SOV5OFF){


				if (cmd_flag && HAL_GPIO_ReadPin (GPIOA, FUEL_MAIN_Pin)){
					valves_selected &= 0x3EF;
					UART_SendMessage(&huart2, " SOV5 is OFF...\n");
					UART_SendMessage_VI(&huart3, " SOV5 is OFF...\n");
					cmd_flag = FALSE;
					v5=0;

				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin (GPIOA, FUEL_MAIN_Pin))) {
					UART_SendMessage(&huart2, " SOV5 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == SOV6ON){
				if (cmd_flag && !(HAL_GPIO_ReadPin (GPIOA, LOX_MAIN_Pin))){
				valves_selected |= (uint16_t)SOV6;
				UART_SendMessage(&huart2, " SOV6 is ON...\n");
				UART_SendMessage_VI(&huart3, " SOV6 is ON...\n");
				cmd_flag = FALSE;
				v6=1;

				}
				else if (cmd_flag && HAL_GPIO_ReadPin (GPIOA, LOX_MAIN_Pin)){
					UART_SendMessage(&huart2, " SOV6 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == SOV6OFF){

				if (cmd_flag && HAL_GPIO_ReadPin (GPIOA, LOX_MAIN_Pin)){
					valves_selected &= 0x3DF;
					UART_SendMessage(&huart2, " SOV6 is OFF...\n");
					UART_SendMessage_VI(&huart3, " SOV6 is OFF...\n");
					cmd_flag = FALSE;
					v6=0;

				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin (GPIOA, LOX_MAIN_Pin))){
					UART_SendMessage(&huart2, " SOV6 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == SOV7ON){


				if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin))){
					valves_selected |= (uint16_t)SOV7;
					UART_SendMessage(&huart2, " SOV7 is ON...\n");
					UART_SendMessage_VI(&huart3, " SOV7 is ON...\n");
					cmd_flag = FALSE;
					v7=1;
				}
				else if (cmd_flag && HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin)){
					UART_SendMessage(&huart2, " SOV7 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == SOV7OFF){


				if (cmd_flag && HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin)){

					valves_selected &= 0x3BF;
					UART_SendMessage(&huart2, " SOV7 is OFF...\n");
					UART_SendMessage_VI(&huart3, " SOV7 is OFF...\n");
					cmd_flag = FALSE;
					v7=0;

				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin))) {
					UART_SendMessage(&huart2, " SOV7 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == SOV8ON){


				if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin))){
					valves_selected |= (uint16_t)SOV8;
					UART_SendMessage(&huart2, " SOV8 is ON...\n");
					UART_SendMessage_VI(&huart3, " SOV8 is ON...\n");
					cmd_flag = FALSE;
					v8=1;
				}
				else if (cmd_flag && HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin)){
					UART_SendMessage(&huart2, " SOV8 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == SOV8OFF){

				if (cmd_flag && HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin)){
					valves_selected &= 0X37F;
					UART_SendMessage(&huart2, " SOV8 is OFF...\n");
					UART_SendMessage_VI(&huart3, " SOV8 is OFF...\n");
					cmd_flag = FALSE;
					v8=0;
				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin))){
					UART_SendMessage(&huart2, " SOV8 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == SOV9ON){


				if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin))){
					valves_selected |= (uint16_t)SOV9;
					UART_SendMessage(&huart2, " SOV9 is ON...\n");
					UART_SendMessage_VI(&huart3, " SOV9 is ON...\n");
					cmd_flag = FALSE;
					v9=1;
				}
				else if(cmd_flag && HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin)){
					UART_SendMessage(&huart2, " SOV9 is already ON...\n");
					cmd_flag = FALSE;

				}
			}
			else if (command == SOV9OFF){


				if (cmd_flag && HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin)){
					valves_selected &= 0X2FF;
					UART_SendMessage(&huart2, " SOV9 is OFF...\n");
					UART_SendMessage_VI(&huart3, " SOV9 is OFF...\n");
					cmd_flag = FALSE;
					v9=0;
				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin))){
					UART_SendMessage(&huart2, " SOV9 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == valve_status && cmd_flag){

				 int len = 0;
				  sprintf(buffer,"\n*** Valves and Igniters Status ***\r\n\n SOV1: %d\r\n"
						                   " SOV2: %d\r\n SOV3: %d\r\n SOV4: %d\r\n SOV5: %d\r\n SOV6: %d\r\n"
						                   " SOV7: %d\r\n SOV8: %d\r\n SOV9: %d\r\n IG:   %d\r\n",v1, v2, v3,\
										   v4,v5,v6,v7,v8,v9,ignit);
				  len =strlen(buffer);
				  HAL_UART_Transmit(&huart2, buffer, len, 1000);
				  cmd_flag = FALSE;
			}
            /*********************************************/
            // Auto cycling valves
			else if (command == cycle_valves && cmd_flag ){
				    UART_SendMessage(&huart2, "\r ...Auto cycling valves...\n");
			    	cmd_flag = FALSE;

                 //Set all the valves off before starting
			    	valves_selected = 0;
			    	ctrl->valveTarget  = valves_selected;

			    	for(int i =  0 ; i <=9 ; i ++){
			    			success &= CycleValves(ctrl);
			    		}// cycle through sov1 to sov9
			}

            /*
			else if(command == cycle_voff && cmd_flag){
			    		//Set all the valves off before starting
			    		valves_selected = 0;
			    		ctrl->valveTarget  = valves_selected;
			    		UART_SendMessage(&huart2, " Auto cycling valves OFF...\n");
			    		cmd_flag = FALSE;
			   } // end of else if for cycle_valves*/
            /*************************************************************/

    	   // Set valves
			ctrl->valveTarget = valves_selected;
			success &= ValveStateSetter(ctrl->valveTarget);
			ctrl->valveConfiguration = StateConfiguration();
}

uint32_t CycleValves(struct StateVars *ctrl)
{
	uint32_t success = TRUE;
	HAL_Delay(300);
    if (ctrl->valveTarget == 0){
    	ctrl->valveTarget = 0x1;
    }
	else if (ctrl->valveTarget < 256){

		ctrl->valveTarget *= 2;


	}
	else if (ctrl->valveTarget == 256){
		ctrl->valveTarget = 0 ;
	}



	success &= ValveStateSetter(ctrl->valveTarget);
	ctrl->valveConfiguration = StateConfiguration();

	if (ctrl -> valveTarget == 1 && HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin)){
		UART_SendMessage(&huart2, " SOV1 ON...\n");
	}
	else if( ctrl-> valveTarget == 2 && HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin)){
		UART_SendMessage(&huart2, " SOV1 OFF...SOV2 ON\n");
	}
	else if(ctrl-> valveTarget == 4 && HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin)){
		UART_SendMessage(&huart2, " SOV2 OFF...SOV3 ON\n");
	}
	else if(ctrl-> valveTarget == 8 && HAL_GPIO_ReadPin(GPIOC, FUEL_VENT_Pin)){
		UART_SendMessage(&huart2, " SOV3 OFF...SOV4 ON\n");
	}
	else if(ctrl-> valveTarget == 16 && HAL_GPIO_ReadPin(GPIOA, FUEL_MAIN_Pin)){
		UART_SendMessage(&huart2, " SOV4 OFF...SOV5 ON\n");
	}
	else if(ctrl-> valveTarget == 32 && HAL_GPIO_ReadPin(GPIOA, LOX_MAIN_Pin)){
			UART_SendMessage(&huart2, " SOV5 OFF...SOV6 ON\n");
	}
	else if(ctrl-> valveTarget == 64 && HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin)){
				UART_SendMessage(&huart2, " SOV6 OFF...SOV7 ON\n");
	}
	else if(ctrl-> valveTarget == 128 && HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin)){
					UART_SendMessage(&huart2, " SOV7 OFF...SOV8 ON\n");
		}
	else if(ctrl-> valveTarget == 256 && HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin)){
						UART_SendMessage(&huart2, " SOV8 OFF...SOV9 ON\n");
			}
	else if (ctrl-> valveTarget == 0 && !( HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin)) ){
		UART_SendMessage(&huart2, " SOV9 OFF...\n");
	}




	//Todo ADD  a handler for commands that can be tryin to access while the automatic sequence is on
    // Auto cycling GPIO check for messaging to VI and printing on screen
    //SOV1
	//HAL_Delay(2);//allow time to read
	if(HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin)){
		// UART_SendMessage(&huart2, " SOV1 ON...");
		 UART_SendMessage_VI(&huart3, " SOV1 ON...\n");
	}
	else if ( !(HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin))){
		 //UART_SendMessage(&huart2, " SOV1 OFF...");
		 UART_SendMessage_VI(&huart3, " SOV1 OFF...\n");

	}
	//SOV2

	if (HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin)){
		 //UART_SendMessage(&huart2, " SOV2 ON...");
		 UART_SendMessage_VI(&huart3, " SOV2 ON...\n");
	}
	else if (!( HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin))){
		 //UART_SendMessage(&huart2, " SOV2 OFF...");
		 UART_SendMessage_VI(&huart3, " SOV2 OFF...\n");
	}
    //SOV3

	if (HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin)){
		 //UART_SendMessage(&huart2, " SOV3 ON...");
		 UART_SendMessage_VI(&huart3, " SOV3 ON...\n");
	}
	else if (!( HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin))){
		 //UART_SendMessage(&huart2, " SOV3 OFF...");
		 UART_SendMessage_VI(&huart3, " SOV3 OFF...\n");
	}

    //SOV4
	if (HAL_GPIO_ReadPin(GPIOC, FUEL_VENT_Pin)){
		// UART_SendMessage(&huart2, " SOV4 ON...");
		 UART_SendMessage_VI(&huart3, " SOV4 ON...\n");
	}
	else if(!( HAL_GPIO_ReadPin(GPIOC, FUEL_VENT_Pin))){
		 //UART_SendMessage(&huart2, " SOV4 OFF...");
		 UART_SendMessage_VI(&huart3, " SOV4 OFF...\n");
	}

    //SOV5

	if (HAL_GPIO_ReadPin(GPIOA, FUEL_MAIN_Pin)){
		// UART_SendMessage(&huart2, " SOV5 ON...");
		 UART_SendMessage_VI(&huart3, " SOV5 ON...\n");
	}
	else if(!( HAL_GPIO_ReadPin(GPIOA, FUEL_MAIN_Pin))){
		// UART_SendMessage(&huart2, " SOV5 OFF...");
		 UART_SendMessage_VI(&huart3, " SOV5 OFF...\n");
	}

    //SOV6
	if (HAL_GPIO_ReadPin(GPIOA, LOX_MAIN_Pin)){
		// UART_SendMessage(&huart2, " SOV6 ON...");
		 UART_SendMessage_VI(&huart3, " SOV6 ON...\n");
	}
	else if (!( HAL_GPIO_ReadPin(GPIOA, LOX_MAIN_Pin))){
		// UART_SendMessage(&huart2, " SOV6 OFF...");
		 UART_SendMessage_VI(&huart3, " SOV6 OFF...\n");
	}


    //SOV7
	if (HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin)){
		// UART_SendMessage(&huart2, " SOV7 ON...");
		 UART_SendMessage_VI(&huart3, " SOV7 ON...\n");
	}
	else if(!( HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin))){
		// UART_SendMessage(&huart2, " SOV7 OFF...");
		 UART_SendMessage_VI(&huart3, " SOV7 OFF...\n");
	}

    //SOV8
	if (HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin)){
		// UART_SendMessage(&huart2, " SOV8 ON...");
		 UART_SendMessage_VI(&huart3, " SOV8 ON...\n");
	}
	else if(!( HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin))){
		// UART_SendMessage(&huart2, " SOV8 OFF...");
		 UART_SendMessage_VI(&huart3, " SOV8 OFF...\n");
	}
	//SOV9
	if (HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin)){
		// UART_SendMessage(&huart2, " SOV9 ON... \n");
		 UART_SendMessage_VI(&huart3, " SOV9 ON...\n");

	}
	else if(!( HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin))){
		 //UART_SendMessage(&huart2, " SOV9 OFF...\n");
		 UART_SendMessage_VI(&huart3, " SOV9 OFF...\n");
	}



	return success;
}

