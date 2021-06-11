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

	const char* naming[] = { [0] = "State : Safety\n", [1] = "State : Valve Check\n" ,	\
		[2] = "State : Lox Pre Fill\n", [3] = "State : Lox Fill\n", [4] = "State : Lox Chill\n",	\
		[5] = "State : Pressurize\n", [6] = "State : Ignition\n",	[7] = "State : Oxidizer Start\n",\
		[8] = "State : Firing\n", [9] = "State : Purge\n",	[10] = "State : Shutdown\n", [11] = "State : Turn Off\n" };

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
    if(state <= TURN_OFF && state >= SAFETY)
    {
    	success = TRUE;
		switch(state)
		{

			case SAFETY:
				break;
			case VALVE_CHECK:
				break;
			case LOX_PRE_FILL:
				break;
			case LOX_FILL:
				break;
			case LOX_CHILL:
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
			case SHUTDOWN:
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

void ValveStatus(struct StateVars *ctrl)
{
	//This function will give us the binary addition so that we know which valves are Open and
	//doublecheck that data with the state config.
	uint32_t status = 22;
	uint32_t valve_config = 0, valve_test = 0;
	//arr is a 2d char array that contains all the solenoid valves positions starting from arr[22] to arr[30]
	for(int i=0; i<=8; i++){
		valve_config +=(int)arr[i+status]*(2^i);
	}
	//Compare valve_config with state_config
	if (!(valve_config == ctrl->valveConfiguration)){
		//Then we need to know which valves differ from the valveConfiguration

		//Then XOR valve_config and valveConfiguration
		// By Xoring both values we will abotain the valves that mistmatch
		valve_test = valve_config^ctrl->valveConfiguration;

        //  if pvf01 is within the mistmatched valves lets check if it should be ON or OFF.
		if ((valve_test & (uint32_t)pvf01) == (uint32_t)pvf01)
		{
			//Since we know that valve pvf01 is problematic and if pvf01 is not set to be ON in the
			// valveConfiguration then the valve should be OFF other wise
			((ctrl->valveConfiguration & (uint32_t)pvf01) != (uint32_t)pvf01 \
					? sprintf(notice_valve_error,"ERROR: Valve pvf01 should be OFF\n" )\
					: sprintf( notice_valve_error,"ERROR: Valve pvf01 should be ON\n"));

			UART_SendMessage(&huart2, notice_valve_error);

		}

		if ((valve_test & (uint32_t)pvl01) == (uint32_t)pvl01)
				{
					//Since we know that valve pvf01 is problematic and if pvf01 is not set to be ON in the
					// valveConfiguration then the valve should be OFF other wise
					((ctrl->valveConfiguration & (uint32_t)pvl01) != (uint32_t)pvl01 \
							?sprintf( notice_valve_error,"ERROR: Valve pvl01 should be OFF\n") \
							:sprintf( notice_valve_error,"ERROR: Valve pvl01 should be ON\n"));

					UART_SendMessage(&huart2, notice_valve_error);

				}

		if ((valve_test & (uint32_t)pvp01) == (uint32_t)pvp01)
						{
							//Since we know that valve pvf01 is problematic and if pvf01 is not set to be ON in the
							// valveConfiguration then the valve should be OFF other wise
							((ctrl->valveConfiguration & (uint32_t)pvp01) != (uint32_t)pvp01 \
									? sprintf(notice_valve_error,"ERROR: Valve pvp01 should be OFF\n") \
									: sprintf(notice_valve_error,"ERROR: Valve pvp01 should be ON\n"));

							UART_SendMessage(&huart2, notice_valve_error);

						}
		if ((valve_test & (uint32_t)sovf01) == (uint32_t)sovf01)
						{
							//Since we know that valve pvf01 is problematic and if pvf01 is not set to be ON in the
							// valveConfiguration then the valve should be OFF other wise
							((ctrl->valveConfiguration & (uint32_t)sovf01) != (uint32_t)sovf01 \
									? sprintf(notice_valve_error,"ERROR: Valve sovf01 should be OFF\n") \
									: sprintf(notice_valve_error,"ERROR: Valve sovf01 should be ON\n"));

							UART_SendMessage(&huart2, notice_valve_error);

						}
		if ((valve_test & (uint32_t)pvf02) == (uint32_t)pvf02)
						{
							//Since we know that valve pvf01 is problematic and if pvf01 is not set to be ON in the
							// valveConfiguration then the valve should be OFF other wise
							((ctrl->valveConfiguration & (uint32_t)pvf02) != (uint32_t)pvf02 \
									? sprintf(notice_valve_error,"ERROR: Valve pvf02 should be OFF\n") \
									: sprintf(notice_valve_error,"ERROR: Valve pvf02 should be ON\n"));

							UART_SendMessage(&huart2, notice_valve_error);

						}
		if ((valve_test & (uint32_t)pvl03) == (uint32_t)pvl03)
						{
							//Since we know that valve pvf01 is problematic and if pvf01 is not set to be ON in the
							// valveConfiguration then the valve should be OFF other wise
							((ctrl->valveConfiguration & (uint32_t)pvl03) != (uint32_t)pvl03 \
									? sprintf(notice_valve_error,"ERROR: Valve pvl03 should be OFF\n") \
									: sprintf(notice_valve_error,"ERROR: Valve pvl03 should be ON\n"));

							UART_SendMessage(&huart2, notice_valve_error);

						}
		if ((valve_test & (uint32_t)pvlf01) == (uint32_t)pvlf01)
						{
							//Since we know that valve pvf01 is problematic and if pvf01 is not set to be ON in the
							// valveConfiguration then the valve should be OFF other wise
							((ctrl->valveConfiguration & (uint32_t)pvlf01) != (uint32_t)pvlf01 \
									? sprintf(notice_valve_error,"ERROR: Valve pvlf01 should be OFF\n") \
									: sprintf(notice_valve_error,"ERROR: Valve pvlf01 should be ON\n"));

							UART_SendMessage(&huart2, notice_valve_error);

						}
		if ((valve_test & (uint32_t)pvl02) == (uint32_t)pvl02)
						{
							//Since we know that valve pvf01 is problematic and if pvf01 is not set to be ON in the
							// valveConfiguration then the valve should be OFF other wise
							((ctrl->valveConfiguration & (uint32_t)pvl02) != (uint32_t)pvl02 \
									? sprintf(notice_valve_error,"ERROR: Valve pvl02 should be OFF\n") \
									: sprintf(notice_valve_error,"ERROR: Valve pvl02 should be ON\n"));

							UART_SendMessage(&huart2, notice_valve_error);

						}
		if ((valve_test & (uint32_t)pvp02) == (uint32_t)pvp02)
						{
							//Since we know that valve pvf01 is problematic and if pvf01 is not set to be ON in the
							// valveConfiguration then the valve should be OFF other wise
							((ctrl->valveConfiguration & (uint32_t)pvp02) != (uint32_t)pvp02 \
									? sprintf(notice_valve_error, "ERROR: Valve pvp02 should be OFF\n") \
									: sprintf(notice_valve_error, "ERROR: Valve pvp02 should be ON\n"));

							UART_SendMessage(&huart2, notice_valve_error);

						}

	}



}//end of function  ValveStatus()

// |= will perform bitwise OR on the two operands then 1 << 1 = 0010    0 << 1 = 0000
// store the result in the left  operator.

// 1 << 0 = 0001    0 << 0 = 0000
// 1 << 2 = 0100    0 << 2 = 0000

// EX- if vc = 0001 then 0001 | 0010 , vc = 0011
uint32_t StateConfiguration()
{
	uint32_t vc = 0;
	//pvf01 - Fuel Pressure (PA_11)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin) << 0);

	//pvl01 - Lox Pressure (PA_12)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin) << 1);

	//pvp01 - Fuel Purge N2 (PC_7)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin) << 2);

	//sovf01 - Fuel Vent (PC_6)
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOC, FUEL_VENT_Pin) << 3);

	//pvf02 - Fuel Main (PA_9)
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOA, FUEL_MAIN_Pin) << 4);

	//pvl03 - Lox Main (PA_8)
	vc |= (uint32_t)(HAL_GPIO_ReadPin (GPIOA, LOX_MAIN_Pin) << 5);

	//pvlf01 - Lox Fill (PB_15)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin) << 6);

	//pvl02 - Lox Vent (PB_10)
	vc |= (uint32_t)(HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin) << 7);

	//pvp02 - Lox Purge (PB_13)
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
	//pvf01
	(vs & (uint32_t)pvf01) == (uint32_t)pvf01 \
			? HAL_GPIO_WritePin(GPIOA, FUEL_PRESSURE_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, FUEL_PRESSURE_Pin,GPIO_PIN_RESET);

	//pvl01
	(vs & (uint32_t)pvl01) == (uint32_t)pvl01 \
			? HAL_GPIO_WritePin(GPIOA, LOX_PRESSURE_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, LOX_PRESSURE_Pin,GPIO_PIN_RESET);

	//pvp01
	(vs & (uint32_t)pvp01) == (uint32_t)pvp01 \
			? HAL_GPIO_WritePin(GPIOC, FUEL_PURGE_N2_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOC, FUEL_PURGE_N2_Pin,GPIO_PIN_RESET);

	//sovf01
	(vs & (uint32_t)sovf01) == (uint32_t)sovf01 \
			? HAL_GPIO_WritePin(GPIOC, FUEL_VENT_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOC, FUEL_VENT_Pin,GPIO_PIN_RESET);

	//pvf02
	(vs & (uint32_t)pvf02) == (uint32_t)pvf02 \
			? HAL_GPIO_WritePin(GPIOA, FUEL_MAIN_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, FUEL_MAIN_Pin,GPIO_PIN_RESET);

	//pvl03
	(vs & (uint32_t)pvl03) == (uint32_t)pvl03 \
			? HAL_GPIO_WritePin(GPIOA, LOX_MAIN_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOA, LOX_MAIN_Pin,GPIO_PIN_RESET);

	//pvlf01
	(vs & (uint32_t)pvlf01) == (uint32_t)pvlf01 \
			? HAL_GPIO_WritePin(GPIOB, LOX_FILL_Pin ,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, LOX_FILL_Pin,GPIO_PIN_RESET);

	//pvl02
	(vs & (uint32_t)pvl02) == (uint32_t)pvl02 \
			? HAL_GPIO_WritePin(GPIOB, LOX_VENT_Pin,GPIO_PIN_SET) \
			: HAL_GPIO_WritePin(GPIOB, LOX_VENT_Pin,GPIO_PIN_RESET);

	//pvp02
	(vs & (uint32_t)pvp02) == (uint32_t)pvp02 \
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
	uint32_t success = TRUE;
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
			else if(command == pvf01on){
				if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin))){
					valves_selected |= (uint16_t)pvf01;
					UART_SendMessage(&huart2, " pvf01 is ON...\n");
					UART_SendMessage_VI(&huart3, " pvf01 is ON...\n");
					cmd_flag = FALSE;
					v1 =1;
				}
				else if (cmd_flag && (HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin))){
					UART_SendMessage(&huart2, " pvf01 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == pvf01off){
				if (cmd_flag && (HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin))){
					valves_selected &= 0x3FE;
					UART_SendMessage(&huart2, " pvf01 is OFF...\n");
					UART_SendMessage_VI(&huart3, " pvf01 is OFF...\n");
					cmd_flag = FALSE;
					v1=0;
				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin))){
					UART_SendMessage(&huart2, " pvf01 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == pvl01on){
				if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin))){
					valves_selected |= (uint16_t)pvl01;
					UART_SendMessage(&huart2, " pvl01 is ON...\n");
					UART_SendMessage_VI(&huart3, " pvl01 is ON...\n");
					cmd_flag = FALSE;
					v2=1;

				}
				else if (cmd_flag && (HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin))){
					UART_SendMessage(&huart2, " pvl01 is already ON...\n");
					cmd_flag = FALSE;
				}

			}
			else if (command == pvl01off){
				if (cmd_flag && (HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin))){
					valves_selected &= 0x1FD;
					UART_SendMessage(&huart2, " pvl01 is OFF...\n");
					UART_SendMessage_VI(&huart3, " pvl01 is OFF...\n");
					cmd_flag = FALSE;
					v2=0;
				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin))){
					UART_SendMessage(&huart2, " pvl01 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == pvp01on){
				if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin))){
					valves_selected |= (uint16_t)pvp01;
					UART_SendMessage(&huart2, " pvp01 is ON...\n");
					UART_SendMessage_VI(&huart3, " pvp01 is ON...\n");
					cmd_flag = FALSE;
					v3=1;
				}
				else if (cmd_flag && HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin)){
					UART_SendMessage(&huart2, " pvp01 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == pvp01off){
				if (cmd_flag && HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin)){
					valves_selected &= 0x3FB;
					UART_SendMessage(&huart2, " pvp01 is OFF...\n");
					UART_SendMessage_VI(&huart3, " pvp01 is OFF...\n");
					cmd_flag = FALSE;
					v3=0;

				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin))) {
					UART_SendMessage(&huart2, " pvp01 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == sovf01on){
				if (cmd_flag && !(HAL_GPIO_ReadPin (GPIOC, FUEL_VENT_Pin))){
					valves_selected |= (uint16_t)sovf01;
					UART_SendMessage(&huart2, " sovf01 is ON...\n");
					UART_SendMessage_VI(&huart3, " sovf01 is ON...\n");
					cmd_flag = FALSE;
					v4=1;

				}
				else if (cmd_flag && HAL_GPIO_ReadPin (GPIOC, FUEL_VENT_Pin)){
					UART_SendMessage(&huart2, " sovf01 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == sovf01off){


				if (cmd_flag && HAL_GPIO_ReadPin (GPIOC, FUEL_VENT_Pin)){
					valves_selected &= 0x3F7;
					UART_SendMessage(&huart2, " sovf01 is OFF...\n");
					UART_SendMessage_VI(&huart3, " sovf01 is OFF...\n");
					cmd_flag = FALSE;
					v4=0;

				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin (GPIOC, FUEL_VENT_Pin))){
					UART_SendMessage(&huart2, " sovf01 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == pvf02on){
				if (cmd_flag && !(HAL_GPIO_ReadPin (GPIOA, FUEL_MAIN_Pin))){
					valves_selected |= (uint16_t)pvf02;
					UART_SendMessage(&huart2, " pvf02 is ON...\n");
					UART_SendMessage_VI(&huart3, " pvf02 is ON...\n");
					cmd_flag = FALSE;
					v5=1;
				}
				else if(cmd_flag && HAL_GPIO_ReadPin (GPIOA, FUEL_MAIN_Pin)){
					UART_SendMessage(&huart2, " pvf02 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == pvf02off){


				if (cmd_flag && HAL_GPIO_ReadPin (GPIOA, FUEL_MAIN_Pin)){
					valves_selected &= 0x3EF;
					UART_SendMessage(&huart2, " pvf02 is OFF...\n");
					UART_SendMessage_VI(&huart3, " pvf02 is OFF...\n");
					cmd_flag = FALSE;
					v5=0;

				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin (GPIOA, FUEL_MAIN_Pin))) {
					UART_SendMessage(&huart2, " pvf02 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == pvl03on){
				if (cmd_flag && !(HAL_GPIO_ReadPin (GPIOA, LOX_MAIN_Pin))){
				valves_selected |= (uint16_t)pvl03;
				UART_SendMessage(&huart2, " pvl03 is ON...\n");
				UART_SendMessage_VI(&huart3, " pvl03 is ON...\n");
				cmd_flag = FALSE;
				v6=1;

				}
				else if (cmd_flag && HAL_GPIO_ReadPin (GPIOA, LOX_MAIN_Pin)){
					UART_SendMessage(&huart2, " pvl03 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == pvl03off){

				if (cmd_flag && HAL_GPIO_ReadPin (GPIOA, LOX_MAIN_Pin)){
					valves_selected &= 0x3DF;
					UART_SendMessage(&huart2, " pvl03 is OFF...\n");
					UART_SendMessage_VI(&huart3, " pvl03 is OFF...\n");
					cmd_flag = FALSE;
					v6=0;

				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin (GPIOA, LOX_MAIN_Pin))){
					UART_SendMessage(&huart2, " pvl03 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == pvlf01on){


				if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin))){
					valves_selected |= (uint16_t)pvlf01;
					UART_SendMessage(&huart2, " pvlf01 is ON...\n");
					UART_SendMessage_VI(&huart3, " pvlf01 is ON...\n");
					cmd_flag = FALSE;
					v7=1;
				}
				else if (cmd_flag && HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin)){
					UART_SendMessage(&huart2, " pvlf01 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == pvlf01off){


				if (cmd_flag && HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin)){

					valves_selected &= 0x3BF;
					UART_SendMessage(&huart2, " pvlf01 is OFF...\n");
					UART_SendMessage_VI(&huart3, " pvlf01 is OFF...\n");
					cmd_flag = FALSE;
					v7=0;

				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin))) {
					UART_SendMessage(&huart2, " pvlf01 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == pvl02on){


				if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin))){
					valves_selected |= (uint16_t)pvl02;
					UART_SendMessage(&huart2, " pvl02 is ON...\n");
					UART_SendMessage_VI(&huart3, " pvl02 is ON...\n");
					cmd_flag = FALSE;
					v8=1;
				}
				else if (cmd_flag && HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin)){
					UART_SendMessage(&huart2, " pvl02 is already ON...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == pvl02off){

				if (cmd_flag && HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin)){
					valves_selected &= 0X37F;
					UART_SendMessage(&huart2, " pvl02 is OFF...\n");
					UART_SendMessage_VI(&huart3, " pvl02 is OFF...\n");
					cmd_flag = FALSE;
					v8=0;
				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin))){
					UART_SendMessage(&huart2, " pvl02 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if(command == pvp02on){


				if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin))){
					valves_selected |= (uint16_t)pvp02;
					UART_SendMessage(&huart2, " pvp02 is ON...\n");
					UART_SendMessage_VI(&huart3, " pvp02 is ON...\n");
					cmd_flag = FALSE;
					v9=1;
				}
				else if(cmd_flag && HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin)){
					UART_SendMessage(&huart2, " pvp02 is already ON...\n");
					cmd_flag = FALSE;

				}
			}
			else if (command == pvp02off){


				if (cmd_flag && HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin)){
					valves_selected &= 0X2FF;
					UART_SendMessage(&huart2, " pvp02 is OFF...\n");
					UART_SendMessage_VI(&huart3, " pvp02 is OFF...\n");
					cmd_flag = FALSE;
					v9=0;
				}
				else if (cmd_flag && !(HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin))){
					UART_SendMessage(&huart2, " pvp02 is already OFF...\n");
					cmd_flag = FALSE;
				}
			}
			else if (command == valve_status && cmd_flag){

				 int len = 0;
				  sprintf(buffer,"\n*** Valves and Igniters Status ***\r\n\n pvf01: %d\r\n"
						                   " pvl01: %d\r\n pvp01: %d\r\n sovf01: %d\r\n pvf02: %d\r\n pvl03: %d\r\n"
						                   " pvlf01: %d\r\n pvl02: %d\r\n pvp02: %d\r\n IG:   %d\r\n",v1, v2, v3,\
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
			    		}// cycle through pvf01 to pvp02
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
	HAL_Delay(1500);
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
		UART_SendMessage(&huart2, " pvf01 ON...\n");
	}
	else if( ctrl-> valveTarget == 2 && HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin)){
		UART_SendMessage(&huart2, " pvf01 OFF...pvl01 ON\n");
	}
	else if(ctrl-> valveTarget == 4 && HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin)){
		UART_SendMessage(&huart2, " pvl01 OFF...pvp01 ON\n");
	}
	else if(ctrl-> valveTarget == 8 && HAL_GPIO_ReadPin(GPIOC, FUEL_VENT_Pin)){
		UART_SendMessage(&huart2, " pvp01 OFF...sovf01 ON\n");
	}
	else if(ctrl-> valveTarget == 16 && HAL_GPIO_ReadPin(GPIOA, FUEL_MAIN_Pin)){
		UART_SendMessage(&huart2, " sovf01 OFF...pvf02 ON\n");
	}
	else if(ctrl-> valveTarget == 32 && HAL_GPIO_ReadPin(GPIOA, LOX_MAIN_Pin)){
			UART_SendMessage(&huart2, " pvf02 OFF...pvl03 ON\n");
	}
	else if(ctrl-> valveTarget == 64 && HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin)){
				UART_SendMessage(&huart2, " pvl03 OFF...pvlf01 ON\n");
	}
	else if(ctrl-> valveTarget == 128 && HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin)){
					UART_SendMessage(&huart2, " pvlf01 OFF...pvl02 ON\n");
		}
	else if(ctrl-> valveTarget == 256 && HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin)){
						UART_SendMessage(&huart2, " pvl02 OFF...pvp02 ON\n");
			}
	else if (ctrl-> valveTarget == 0 && !( HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin)) ){
		UART_SendMessage(&huart2, " pvp02 OFF...\n");
	}




	//Todo ADD  a handler for commands that can be tryin to access while the automatic sequence is on
    // Auto cycling GPIO check for messaging to VI and printing on screen
    //pvf01
	//HAL_Delay(2);//allow time to read
	if(HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin)){
		// UART_SendMessage(&huart2, " pvf01 ON...");
		 UART_SendMessage_VI(&huart3, " pvf01 ON...\n");
	}
	else if ( !(HAL_GPIO_ReadPin(GPIOA, FUEL_PRESSURE_Pin))){
		 //UART_SendMessage(&huart2, " pvf01 OFF...");
		 UART_SendMessage_VI(&huart3, " pvf01 OFF...\n");

	}
	//pvl01

	if (HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin)){
		 //UART_SendMessage(&huart2, " pvl01 ON...");
		 UART_SendMessage_VI(&huart3, " pvl01 ON...\n");
	}
	else if (!( HAL_GPIO_ReadPin(GPIOA, LOX_PRESSURE_Pin))){
		 //UART_SendMessage(&huart2, " pvl01 OFF...");
		 UART_SendMessage_VI(&huart3, " pvl01 OFF...\n");
	}
    //pvp01

	if (HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin)){
		 //UART_SendMessage(&huart2, " pvp01 ON...");
		 UART_SendMessage_VI(&huart3, " pvp01 ON...\n");
	}
	else if (!( HAL_GPIO_ReadPin(GPIOC, FUEL_PURGE_N2_Pin))){
		 //UART_SendMessage(&huart2, " pvp01 OFF...");
		 UART_SendMessage_VI(&huart3, " pvp01 OFF...\n");
	}

    //sovf01
	if (HAL_GPIO_ReadPin(GPIOC, FUEL_VENT_Pin)){
		// UART_SendMessage(&huart2, " sovf01 ON...");
		 UART_SendMessage_VI(&huart3, " sovf01 ON...\n");
	}
	else if(!( HAL_GPIO_ReadPin(GPIOC, FUEL_VENT_Pin))){
		 //UART_SendMessage(&huart2, " sovf01 OFF...");
		 UART_SendMessage_VI(&huart3, " sovf01 OFF...\n");
	}

    //pvf02

	if (HAL_GPIO_ReadPin(GPIOA, FUEL_MAIN_Pin)){
		// UART_SendMessage(&huart2, " pvf02 ON...");
		 UART_SendMessage_VI(&huart3, " pvf02 ON...\n");
	}
	else if(!( HAL_GPIO_ReadPin(GPIOA, FUEL_MAIN_Pin))){
		// UART_SendMessage(&huart2, " pvf02 OFF...");
		 UART_SendMessage_VI(&huart3, " pvf02 OFF...\n");
	}

    //pvl03
	if (HAL_GPIO_ReadPin(GPIOA, LOX_MAIN_Pin)){
		// UART_SendMessage(&huart2, " pvl03 ON...");
		 UART_SendMessage_VI(&huart3, " pvl03 ON...\n");
	}
	else if (!( HAL_GPIO_ReadPin(GPIOA, LOX_MAIN_Pin))){
		// UART_SendMessage(&huart2, " pvl03 OFF...");
		 UART_SendMessage_VI(&huart3, " pvl03 OFF...\n");
	}


    //pvlf01
	if (HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin)){
		// UART_SendMessage(&huart2, " pvlf01 ON...");
		 UART_SendMessage_VI(&huart3, " pvlf01 ON...\n");
	}
	else if(!( HAL_GPIO_ReadPin(GPIOB, LOX_FILL_Pin))){
		// UART_SendMessage(&huart2, " pvlf01 OFF...");
		 UART_SendMessage_VI(&huart3, " pvlf01 OFF...\n");
	}

    //pvl02
	if (HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin)){
		// UART_SendMessage(&huart2, " pvl02 ON...");
		 UART_SendMessage_VI(&huart3, " pvl02 ON...\n");
	}
	else if(!( HAL_GPIO_ReadPin(GPIOB, LOX_VENT_Pin))){
		// UART_SendMessage(&huart2, " pvl02 OFF...");
		 UART_SendMessage_VI(&huart3, " pvl02 OFF...\n");
	}
	//pvp02
	if (HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin)){
		// UART_SendMessage(&huart2, " pvp02 ON... \n");
		 UART_SendMessage_VI(&huart3, " pvp02 ON...\n");

	}
	else if(!( HAL_GPIO_ReadPin(GPIOB, LOX_PURGE_Pin))){
		 //UART_SendMessage(&huart2, " pvp02 OFF...\n");
		 UART_SendMessage_VI(&huart3, " pvp02 OFF...\n");
	}



	return success;
}

