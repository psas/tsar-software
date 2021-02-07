// messages.c
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This file contains the command handler, uart communication and sending and receiving of data.
//  Also inclues the conversion of hexadecimal data into uint32_t variables for faster comparison
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#include "messages.h"

// Function to send the messages over serial communication
void UART_SendMessage(UART_HandleTypeDef *huart, char* message)
{
	  int len = 0;
	  uint32_t success = FALSE;
	  sprintf(buffer, message);
	  len =strlen(buffer);
	  HAL_UART_Transmit(&huart2, buffer, len, 1000);
	  return success;
}

//Receiving the command
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 uint32_t success = FALSE;
	 uint8_t i;
	    if (huart->Instance == USART2)
	     {

	        if (Rx_indx==0) { for (i=0;i<100;i++) Rx_Buffer[i]=0; }   //clear Rx_Buffer before receiving new data



	        if (Rx_data[0] != 13) //if received data different from ascii 13 (enter)
	            {
	            Rx_Buffer[Rx_indx++]=Rx_data[0];    //add data to Rx_Buffer
	            }
	        else            //if the receiving is complete
	            {
	                 Rx_indx=0;
	                 retransition = TRUE;
	                 RxTxFlags |= 0x1;
	                 success = TRUE;
	                 hexadecimalToDecimal(Rx_Buffer);
	                 // variable for the status command

	                 if(command == stat){
	                	 status= TRUE;
	                 }
	                 else if (command == power_off || stop||valve_check|| lox_fill || lox_chill || lox_pre_fill||\
	                		               pressurizing || safety || ignition || start_handler ){
	                	 cmd_flag = TRUE;

	                 }

	            }
	        HAL_UART_Receive_IT(&huart2, Rx_data, 1);   //activate UART receive interrupt every time

	}// end of if  (huart->Instance == USART2)
}

//HEX to decimal
void hexadecimalToDecimal(char* hexVal)
{
	uint32_t len = strlen(hexVal);

    // Initializing base value to 1 ( 16^0, 16^1, 16^2, etc )
     uint32_t base = 1;
     uint32_t dec_val = 0;

    // Extracting characters as digits from last character
    for (int i=len-1; i>=0; i--)
    {
        // if ASCII char is between (0-9)
        if (hexVal[i]>='0' && hexVal[i]<='9')
        {
            dec_val += (hexVal[i] - 48)*base;

            // incrementing the base by power
            base = base * 16;
        }

        // if ASCII char is between(A-F)
        else if (hexVal[i]>='A' && hexVal[i]<='F')
        {
            dec_val += (hexVal[i] - 55)*base;

            // incrementing the base by power
            base = base*16;
        }
    }
    command = dec_val;
}

//Function to process the command messages
uint32_t ProcessCommands(struct StateVars * ctrl)
{
	uint32_t success = FALSE;
	// LOX_PRE_FILL command
	if((ctrl->currentState == START_HANDLER) && command == lox_pre_fill )
	{
		ctrl ->currentState = LOX_PRE_FILL;
		success = TRUE;
		RxTxFlags &= !RX_DATA_READY;
	}
    // TURN_OFF command
	else if((command == power_off) && (ctrl->currentState== VALVE_CHECK || \
			 ctrl->currentState == SAFETY))
	{
		ctrl ->currentState = TURN_OFF;
		success = TRUE;
		RxTxFlags &= !RX_DATA_READY;
	}
    // STOP command
	else if((command == stop) && ( ctrl->currentState== LOX_PRE_FILL || \
			  ctrl->currentState == LOX_CHILL|| \
			  ctrl->currentState == LOX_FILL 	|| \
			  ctrl->currentState == VALVE_CHECK|| \
			  ctrl->currentState == START_HANDLER) )
	{
		ctrl->currentState = SAFETY;
		success = TRUE;
		// Clear data read flag
		RxTxFlags &= !RX_DATA_READY;
	}
	// LOX_FILL command
	else if(( ctrl->currentState == LOX_PRE_FILL) && (command == lox_fill))
	{
		ctrl->currentState = LOX_FILL;
		success = TRUE;
		// Clear data read flag
		RxTxFlags &= !RX_DATA_READY;
	}
    // LOX_FILL command
	else if((ctrl->currentState == LOX_FILL) && (command == lox_chill))
	{
		ctrl->currentState = LOX_CHILL;
		success = TRUE;
		// Clear data read flag
		RxTxFlags &= !RX_DATA_READY;
	}
    // LOX_CHILL command
	else if(( ctrl->currentState == LOX_CHILL) && (command == safety))
	{
		ctrl->currentState = SAFETY;
		success = TRUE;
		// Clear data read flag
		RxTxFlags &= !RX_DATA_READY;
	}
    // VALVE_CHECK command
	else if(( ctrl->currentState == START_HANDLER ) && (command == valve_check))
	{
		ctrl->currentState = VALVE_CHECK;
		success = TRUE;
		// Clear data read flag
		RxTxFlags &= !RX_DATA_READY;
	}

    // PRESSURIZE command
	else if((ctrl->currentState == SAFETY) && (command == pressurizing))
	{
		ctrl->currentState = PRESSURIZE;
		success = TRUE;
		// Clear data read flag
		RxTxFlags &= !RX_DATA_READY;
	}
    // IGNITION command
	else if((ctrl->currentState == PRESSURIZE) && (command == ignition))
	{
		ctrl->currentState = IGNITION; //START of the FSM
		success = TRUE;
		// Clear data read flag
		RxTxFlags &= !RX_DATA_READY;
	}


	//State status
	else if((command == stat) && (
				  ctrl->currentState == START_HANDLER || \
				  ctrl->currentState == LOX_PRE_FILL || \
				  ctrl->currentState == LOX_CHILL|| \
				  ctrl->currentState == LOX_FILL 	|| \
				  ctrl->currentState == VALVE_CHECK|| \
				  ctrl->currentState == PRESSURIZE  ||   \
				  ctrl->currentState == TURN_OFF || \
				  ctrl->currentState == SAFETY \
				  ))
		{

		    StateStatus(ctrl);
			success = TRUE;
			// Clear data read flag
			RxTxFlags &= !RX_DATA_READY;
		}
	// Handling invalid transitions
	else if((command == lox_fill) && (
					  ctrl->currentState == START_HANDLER|| \
					  ctrl->currentState == LOX_CHILL	|| \
					  ctrl->currentState == VALVE_CHECK|| \
					  ctrl->currentState == PRESSURIZE  ||   \
					  ctrl->currentState == TURN_OFF || \
					  ctrl->currentState == SAFETY \
					  )){

					if (cmd_flag)
		 	 	 	 	 {
		 	 	 		 	 UART_SendMessage(&huart2, "Unauthorized transition, use the help command for valid transitions...\n");
		 	 	 		 	cmd_flag = FALSE;
		 	 	 	 	 }
				success = TRUE;
				// Clear data read flag
				RxTxFlags &= !RX_DATA_READY;
			}
	else if((command == lox_chill) && (
						  ctrl->currentState == START_HANDLER|| \
						  ctrl->currentState == LOX_PRE_FILL	|| \
						  ctrl->currentState == VALVE_CHECK|| \
						  ctrl->currentState == PRESSURIZE  ||   \
						  ctrl->currentState == TURN_OFF || \
						  ctrl->currentState == SAFETY \
						  ))
				{

							if (cmd_flag)
				 	 	 	 	 {
				 	 	 		 	 UART_SendMessage(&huart2, "Unauthorized transition, use the help command for valid transitions...\n");
				 	 	 		 	cmd_flag = FALSE;
				 	 	 	 	 }
					success = TRUE;
					// Clear data read flag
					RxTxFlags &= !RX_DATA_READY;
				}
	else if((command == safety) && (
						  ctrl->currentState == START_HANDLER|| \
						  ctrl->currentState == LOX_FILL	|| \
						  ctrl->currentState == VALVE_CHECK|| \
						  ctrl->currentState == PRESSURIZE  ||   \
						  ctrl->currentState == TURN_OFF ||\
						  ctrl->currentState == LOX_PRE_FILL \
						  ))
				{
						if (cmd_flag)
				 	 	 	 	 {
				 	 	 		 	 UART_SendMessage(&huart2, "Unauthorized transition, use the help command for valid transitions...\n");
				 	 	 		 	cmd_flag = FALSE;
				 	 	 	 	 }
					success = TRUE;
					// Clear data read flag
					RxTxFlags &= !RX_DATA_READY;
				}
	else if((command == valve_check) && (
						  ctrl->currentState == LOX_PRE_FILL|| \
						  ctrl->currentState == LOX_CHILL	|| \
						  ctrl->currentState == LOX_FILL|| \
						  ctrl->currentState == PRESSURIZE  ||   \
						  ctrl->currentState == TURN_OFF || \
						  ctrl->currentState == SAFETY \
						  ))
				{
						if (cmd_flag)
				 	 	 	 	 {
				 	 	 		 	 UART_SendMessage(&huart2, "Unauthorized transition, use the help command for valid transitions...\n");
				 	 	 		 	cmd_flag = FALSE;
				 	 	 	 	 }
					success = TRUE;
					// Clear data read flag
					RxTxFlags &= !RX_DATA_READY;

				}
	else if((command == lox_pre_fill) && (
							  ctrl->currentState == LOX_CHILL || \
							  ctrl->currentState == SAFETY|| \
							  ctrl->currentState == LOX_FILL	|| \
							  ctrl->currentState == VALVE_CHECK|| \
							  ctrl->currentState == PRESSURIZE  ||   \
							  ctrl->currentState == TURN_OFF ))
					{
							if (cmd_flag)
					 	 	 	 	 {
					 	 	 		 	 UART_SendMessage(&huart2, "Unauthorized transition, use the help command for valid transitions...\n");
					 	 	 		 	cmd_flag = FALSE;
					 	 	 	 	 }
						success = TRUE;
						// Clear data read flag
						RxTxFlags &= !RX_DATA_READY;
					}
	else if((command == pressurizing) && (
								  ctrl->currentState == LOX_PRE_FILL || \
								  ctrl->currentState == LOX_CHILL || \
								  ctrl->currentState == LOX_FILL	|| \
								  ctrl->currentState == VALVE_CHECK|| \
								  ctrl->currentState == TURN_OFF ))
						{
								if (cmd_flag)
						 	 	 	 	 {
						 	 	 		 	 UART_SendMessage(&huart2, "Unauthorized transition, use the help command for valid transitions...\n");
						 	 	 		 	cmd_flag = FALSE;
						 	 	 	 	 }
							success = TRUE;
							// Clear data read flag
							RxTxFlags &= !RX_DATA_READY;
						}

	//Handle trying to transition to the same state
	else if ((command == lox_pre_fill && ctrl->currentState == LOX_PRE_FILL)||\
			 (command == lox_fill && ctrl->currentState == LOX_FILL)|| \
			 (command == lox_chill && ctrl->currentState == LOX_CHILL)||\
			 (command == safety && ctrl->currentState == SAFETY)||\
			 (command == valve_check && ctrl->currentState == VALVE_CHECK)||\
			 (command == pressurizing && ctrl->currentState == PRESSURIZE) ||\
		     (command == stop && ctrl->currentState == SAFETY)||\
			 (command == start_handler && ctrl->currentState == START_HANDLER)){

								if (cmd_flag && retransition)
						 	 	 	 	 {
						 	 	 		 	 UART_SendMessage(&huart2, " Currently in the state you are intending to transition to...\n");
						 	 	 		 	cmd_flag = FALSE;
						 	 	 	 	 }
	}
	//StateMachine(ctrl);
	return success;
}

//Function to separate sensor values
uint32_t getWords(char *base, char target[10][20])
{
	int n=0,i,j=0;

	for(i=0;TRUE;i++)
	{
		if(base[i]!=' '){
			target[n][j++]=base[i];
		}
		else{
			target[n][j++]='\0';//insert NULL
			n++;
			j=0;
		}
		if(base[i]=='\0')
		    break;
	}
	return n;
}
// Function in charge of determining that the Sensor Values are Nominal
// Need to know what is the Nominal Range
uint32_t NominalValueCheck(struct StateVars *ctrl, char marionette[])
{
	uint32_t nominal;
	/*
	int amount; //number of hex
	int i, j=0; // for loop counter


		//char str[]="AB4543E2 34562ED3 4342A34D 67AFE424";
		char arr[10][20];
	    char notice[10][20];
	    char nominal = 0; //FALSE
	    uint32_t sensor_value;

	    //Request the sensor values
	   	UART_SendMessage("0xA\r\n");
	   	// wait some time for BB_AI to respond
	   	HAL_Delay(500);

		amount = getWords(marionette,arr);

	    for(i=0;i<=n;i++)
	    {
	        value = hexadecimalToDecimal(arr[i]);

	        if ( sensor_value <= 60000 && sensor_value >= 10000){
	            // All good
	            //sprintf(notice[i], "\nSensor %d value is OK...",i+1);
	            // Update status
	            UART_SendMessage(&huart2,"%s\n\r",notice[i]);
	            //printf("%s",notice[i]);
	            j ++;
	        }
	        else {
	            sprintf(notice[i], "\nWARNING: Sensor %d value is not nominal...",i+1);
	            // Update status
	            UART_SendMessage(&huart2,"%s\n\r",notice[i]);
	           // DANGER Go to Automatic Shutdown
	        //printf("%s",notice[i]);
	        ctrl -> currentState= AUTOMATIC_SHUTDOWN;
	        }
	    }

	    if (j == 9){ nominal= TRUE;}//If all the values are correct
*/
	nominal = TRUE; // just for testing
 return nominal;
}


void Get_State_Disagree_Error_Msg(char *errorMessage, enum StateName expectedState, \
			enum StateName passedState)
{
	char state_disagree[VALVE_STATE_BUFFER_SIZE] = "ERROR: Expected State, Passed State Disagree. ";
	char temp[32];
	strcat(state_disagree, "Expected: ");
	sprintf(temp, "%d", expectedState);
	strcat(state_disagree, temp);
	strcat(state_disagree,"Passed: ");
	sprintf(temp, "%d", passedState);
	strcat(state_disagree, temp);
	memset(errorMessage, '\0', VALVE_STATE_BUFFER_SIZE);
	strcpy(errorMessage, state_disagree);
}

void Get_Invalid_State_Error_Msg(char *errorMessage, enum StateName state, enum StateName lastState)
{
	char state_invalid[VALVE_STATE_BUFFER_SIZE] = "Invalid State: ";
	char temp[32];
	sprintf(temp, "%d",state);
	strcat(state_invalid, temp);
	strcat(state_invalid, ", lastState: ");
	sprintf(temp, "%d", lastState);
	strcat(state_invalid, temp);
	memset(errorMessage, '\0', VALVE_STATE_BUFFER_SIZE);
	strcpy(errorMessage, state_invalid);
}

