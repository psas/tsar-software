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
#include "states_def.h"
#include "ring_buffer.h"

//troubleshooting
int x = 0;

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
	    	//clear Rx_Buffer before receiving new data
	        if (Rx_indx==0) { for (i=0;i<100;i++) Rx_Buffer[i]=0; }

	        //if received data different from ascii 13 (Carriage Return) continue storing data.
	        if (Rx_data[0] != 13)
	            {
	        		Rx_Buffer[Rx_indx++]=Rx_data[0];    //add data to Rx_Buffer
	            }


	        else  //the receiving is complete!
	            {
	        		 // Flag variables
	        	     Rx_indx=0;          //Reset the Rx index to start reading data from the beginning.
	        	     retransition = TRUE;//For message in case usr tries to transition to the current state.
					 inv_flag = TRUE;    //For message about invalid command.
					 RxTxFlags |= 0x1;
					 success = TRUE;
					 // Data packet from Marionette or a command from the user ?
	        		 // Pass Rx_Buffer to splitValues and look for spaces
	        		 // if data contains a command then checker = 1,
					 // if data contains a Marionette data packet then checker = 8
					 // MAR_DATA_CHECK contains the number of values for the data packet.
					 // EX: Testing MAR_DATA_CHECK = 8 (3F99 and 7 sensor values)
	        		  uint32_t checker = splitValues(Rx_Buffer, NULL);
	        		  if (checker == MAR_DATA_CHECK){

	        			  //send data packet to ring buffer
	        			  ringBuffer(Rx_Buffer);

	        	     }
	        		  else{
	        		  //Otherwise we have an user's command
	        		 // convert the hex command received into uint32_t
	        		 command =  hexadecimalTouint32(Rx_Buffer);

	                 // command variable contains the uint32 conversion from the hex command
	                 if(command ==state_status){
	                	 status= TRUE;
	                 }
	                 else if (command ==  power_off || stop || valve_check || lox_fill || lox_chill || pressurizing || lox_pre_fill || ignition ||start_handler \
	             			|| cycle_valves || cycle_voff || IGON || IGOFF || SOV1ON || SOV1OFF || SOV2ON || SOV2OFF || SOV3ON || SOV3OFF || SOV4ON || SOV4OFF || SOV5ON || SOV5OFF ||\
	             			 SOV6ON || SOV6OFF || SOV7ON || SOV7OFF || SOV8ON || SOV8OFF || SOV9ON || SOV9OFF ){

	                	 cmd_flag = TRUE;// so that the transition to a state occur once and not re transition continuosly

	                 }
	              }//end of else for user command

	            }// end of else for receive complete

	        HAL_UART_Receive_IT(&huart2, Rx_data, 1);   //activate UART receive interrupt for ascii byte every time.

	}// end of if  (huart->Instance == USART2)
}

//Convert HEX to uint32_t
uint32_t hexadecimalTouint32(char* hexVal)
{
	uint32_t len = strlen(hexVal);

    // Initializing base value to 1 ( 16^0, 16^1, 16^2, etc )
     uint32_t base = 1;
     uint32_t val = 0;

    // Extracting characters as digits from last character
    for (int i=len-1; i>=0; i--)
    {
        // if ASCII char is between (0-9)
        if (hexVal[i]>='0' && hexVal[i]<='9')
        {
            val += (hexVal[i] - 48)*base;

            // incrementing the base by power
            base = base * 16;
        }

        // if ASCII char is between(A-F)
        else if (hexVal[i]>='A' && hexVal[i]<='F')
        {
            val += (hexVal[i] - 55)*base;

            // incrementing the base by power
            base = base*16;
        }
    }
    return val;
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
			  /*ctrl->currentState == VALVE_CHECK|| \*/ //dont want valve_check to end up in safety
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
	else if((command ==state_status) && (
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
	else if ((command == stop) && (ctrl->currentState==VALVE_CHECK || ctrl->currentState == START_HANDLER)){
					if (cmd_flag)
				 	  {
				 	 	  UART_SendMessage(&huart2, "Unauthorized transition, use the help command for valid transitions...\n");
				 	 	  cmd_flag = FALSE;
				 	 }
						success = TRUE;
						// Clear data read flag
						RxTxFlags &= !RX_DATA_READY;
		}
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

	//Handle the case of an unknown command
	else if (!(command == power_off || stop || valve_check || lox_fill || lox_chill || pressurizing || lox_pre_fill || ignition ||start_handler \
			|| cycle_valves || cycle_voff || IGON || IGOFF || SOV1ON || SOV1OFF || SOV2ON || SOV2OFF || SOV3ON || SOV3OFF || SOV4ON || SOV4OFF || SOV5ON || SOV5OFF ||\
			 SOV6ON || SOV6OFF || SOV7ON || SOV7OFF || SOV8ON || SOV8OFF || SOV9ON || SOV9OFF )){

		if (inv_flag){
		UART_SendMessage(&huart2, " Invalid command !...\n");
		 inv_flag = FALSE;
		}

	}

	//Handle the case of commands outside valve check state
	else if (command == valve_status || command == cycle_valves ||command == cycle_voff ||command == IGON ||command ==IGOFF \
			||command == SOV1ON ||command == SOV1OFF ||command == SOV2ON ||command == SOV2OFF || command ==SOV3ON \
			||command ==SOV3OFF ||command == SOV4ON ||command == SOV4OFF ||command == SOV5ON ||command ==SOV5OFF \
			||command ==SOV6ON ||command == SOV6OFF ||command == SOV7ON ||command == SOV7OFF ||command == SOV8ON \
			||command ==SOV8OFF || command ==SOV9ON || command ==SOV9OFF){

			if (cmd_flag && !(ctrl->currentState == VALVE_CHECK)){
				UART_SendMessage(&huart2, " This command is only valid under the valve check state...\n");
				cmd_flag = FALSE;
			}

	}


		return success;
}

//Function to separate sensor values
uint32_t splitValues(char *marionette_data, char separated[10][20])
{
	int n=0,j=0;
	//Initialize the storage 2d array to 0
	for(int a=0; a<10;a ++){
		for(int b=0; b<20; b++){
			separated[a][b] = 0;
		}
	}

	for(int i=0;TRUE;i++)// Process the whole marionette data packet
	{
		if(marionette_data[i]!=' '){
			separated[n][j++]=marionette_data[i];
		}
		else{// sensor value processed
			separated[n][j++]='\0';//insert NULL
			n++; // increase by one for every sensor value
			j=0;
		}
		if(marionette_data[i]=='\0')
		    break;
	}
	// n + 1 will give you how many items in the data you want to split when there is a space.
	return n+1;
}
// Function in charge of determining that the Sensor Values are Nominal.
void NominalValueCheck(struct StateVars *ctrl, char marionette[]){

		uint32_t amount, counter, sensor_value = 0; 	//number of hex and counter of valves.
		char arr[10][20] = {0}; 						//to hold the separated sensor values.
	    char notice[100] = {0}; 						//for the output message.

        //amount holds the amount of sensor values.
	    //this helps to know how many sensor values need to be processed.
		amount = splitValues(marionette,arr);

		//Check for valid marionette data packet key (3F99)
		if (hexadecimalTouint32(arr[0]) == marionette_cmd){
            //If the arr[0] contains the key then a sensor value will start at arr[1]
			for(int i=1; i<=amount; i++){
				sensor_value = hexadecimalTouint32(arr[i]);

			// Check if the sensor value is in the nominal range
			if ( sensor_value <= 99 && sensor_value >= 0){
	            	// All good
	            	// sprintf(notice, "\nSensor %d value = %s is OK...",i, arr[i]);
	            	// Update status
					// UART_SendMessage(&huart2,"%s\n\r",notice);
	            	// counter ++;
				}
			//If the sensor value is not in the nominal range.
	        else {
	        	init_time = HAL_GetTick();; //Get time.

	        	//Issue a Warning Message.
	            sprintf(notice, "\nWARNING: Sensor %d value = %s is outside nominal range...",i, arr[i]);
	            // Ok send the warning message
	            UART_SendMessage(&huart2, notice);


	           // DANGER Go to Automatic Shutdown
	           // We only want to go into Automatic Shutdown in particular states after some time


	            //Todo add the states and work with time
	            //********************************************************************************
	            /*
	            if (ctrl->currentState == START_HANDLER|| ctrl->currentState == LOX_FILL	||\
				    ctrl->currentState == VALVE_CHECK  || ctrl->currentState == PRESSURIZE  ||\
					ctrl->currentState == TURN_OFF     || ctrl->currentState == LOX_PRE_FILL||\
					ctrl->currentState == SAFETY       || ctrl->currentState == LOX_CHILL ){

	            	later_time  = HAL_GetTick();;
	            	if(later_time - init_time == sometime ){

                         }// time is withing a range then no need to go to automatic shutdown

	            	else{//is outside the valid time specified go into automatic shutdown}
	            		nominal = FALSE;
	            		ctrl->currentState = AUTOMATIC_SHUTDOWN;
	            	}

	            }
	            //other states
	            else if (ctrl->currentState == START_HANDLER|| ctrl->currentState == LOX_FILL	||\
					    ctrl->currentState == VALVE_CHECK  || ctrl->currentState == PRESSURIZE  ||\
						ctrl->currentState == TURN_OFF     || ctrl->currentState == LOX_PRE_FILL||\
						ctrl->currentState == SAFETY       || ctrl->currentState == LOX_CHILL ){

	            	later_time  = HAL_GetTick();;
	                if(later_time - init_time == sometime ){
	            	  //is ok
	              }
	              else{
	            		nominal = FALSE;
	            		ctrl->currentState = AUTOMATIC_SHUTDOWN;

	              } // go to AUTOMATIC SHUTDOWN
	             }*/



	        }
	      }

		}// end (if checking the data packet is valid )

		//data packet is not valid because first value is not a valid key
		else{
               //x = x+1;//For testing Purposes

                sprintf(notice, "\nWARNING: Marionette data packet key is not correct. The key used: %s..", arr[0]);
                // Ok send the warning message
                UART_SendMessage(&huart2, notice);

			}

}//end of NominalValueCheck()

// Function to Double check that we are transitioning to the correct state
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

// Function to let the usr know which state transition is expected and which one is trying to transition being invalid.
void Get_Invalid_State_Error_Msg(char *errorMessage, enum StateName state, enum StateName lastState)
{
	char state_invalid[VALVE_STATE_BUFFER_SIZE] = "Invalid State: ";
	char temp[32];
	sprintf(temp, " %d ",state);
	strcat(state_invalid, temp);
	strcat(state_invalid, ", lastState: ");
	sprintf(temp, " %d \n\n", lastState);
	strcat(state_invalid, temp);
	memset(errorMessage, '\0', VALVE_STATE_BUFFER_SIZE);
	strcpy(errorMessage, state_invalid);
}

