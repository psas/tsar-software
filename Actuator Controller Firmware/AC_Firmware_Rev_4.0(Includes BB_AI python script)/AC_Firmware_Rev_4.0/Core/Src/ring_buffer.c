#include "ring_buffer.h" // include the header file
#include "messages.h"

void ringBuffer(char data []){
//First store the marionette data in a ring buffer FIFO style
//If is the first time using the buffer Initialize it
	 if (circular==0) {

		 for (int i=0;i<16;i++){ // i for rows
			 for (int j = 0; j<250; j ++){// j for columns

				 circularBuffer[i][j]= 0;
			 }
		 }
		 //The 2D array is initialized !
		 circular = 1;
	 }

	 //Circular Buffer FULL
	 if (bufferLength == SIZE_OF_BUFFER){ // Case where the circular buffer is full
		 bufferLength = 0;
		 writeIndex = 0;
		 /* Maybe setting the array to 0 once is full is not the best idea
		  *
		 for (int i=0;i<100;i++){ // i signifys the y position
			 for (int j = 0; j<120; j ++){// j signifies the x position
				 circularBuffer[i][j]= 0;
			 }
		 }
				 //The 2D array is initialized !
				  *
				  */
	 }

	 //WRITE
	 //Store data packets in the circular buffer
	 else if (bufferLength != SIZE_OF_BUFFER){

		 // loop to store the data packet in the circular buffer
		 for (int i = 0; i < 250; i++){
			 circularBuffer[writeIndex][i] = data[i];

		 //Set the rest of the row with zeroes
		 if (strlen(data) == i){
			 for(int j = i+1; j <= 250; j++ ){
				 circularBuffer[writeIndex][j] = 0;
			 }
		 }
		 // At this point the command should be stored in the x dimension of the 2d array

		 }// end of for loop
		 bufferLength ++;
		 writeIndex++;

		 if (writeIndex == 15){writeIndex = 0;}
	 }

}
// One alternative to check the marionette values from withing the each states ( Create a function read and call it )
// Otherwise leave read inside the ring buffer function and handling the marionettes values will be done first!
void ringBufferRead(){
	int counter = 0;
	    //check if the circular buffer is empty
	    // Initialize it to zero
	     if(bufferLength == 0 && initOnceFlag == TRUE){

	    	 for (int i=0;i<16;i++){ // rows
	    	 			 for (int j = 0; j<250; j ++){// columns

	    	 				 circularBuffer[i][j]= 0;
	    	 			 }

	    	 	 }
	    	 initOnceFlag = FALSE;
	     }
		 // READ
		 // Then there is commands needed to be serviced in the circular buffer only when there is no
	     // data processing.
	     else if(bufferLength <= SIZE_OF_BUFFER && bufferLength != 0){ // && circular_buffer_readFlag){
             // substitute data for the last values added to circularBuffer
			 for(int j = 0; circularBuffer[readIndex][j] != '\0'; j++){
				 counter = j;
			 }

			 //found
			 for (int i = 0; i < counter+1; i++){
				 marionette_data[i] = circularBuffer[readIndex][i];
			 }

			 bufferLength --;
			 readIndex ++;
			 if (readIndex == 15){readIndex = 0;}

       //Then service data packets
       NominalValueCheck(NULL, marionette_data);
    }
}

