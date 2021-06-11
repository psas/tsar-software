#include "ring_buffer.h" // include the header file
#include "messages.h"

void ringBuffer(char data []){
//First store the marionette data in a ring buffer FIFO style
//If is the first time using the buffer Initialize it
	 if (circular==0) {

		 for (int i=0;i<100;i++){ // i signifys the y position
			 for (int j = 0; j<120; j ++){// j signifies the x position

				 circularBuffer[j][i]= 0;
			 }
		 }
		 //The 2D array is initialized !
		 circular == 1;
	 }

	 //Circular Buffer FULL
	 if (bufferLength == SIZE_OF_BUFFER){ // Case where the circular buffer is full
		 bufferLength = 0;

		 for (int i=0;i<100;i++){ // i signifys the y position
			 for (int j = 0; j<120; j ++){// j signifies the x position
				 circularBuffer[j][i]= 0;
			 }
		 }
				 //The 2D array is initialized !
	 }

	 //WRITE
	 //Store data packets in the circular buffer
	 else if (bufferLength != SIZE_OF_BUFFER){

		 // loop to store the data packet in the circular buffer
		 for (int i = 0; i < 120; i++){
			 circularBuffer[i][writeIndex] = data[i];

		 //Set the rest of the row with zeroes
		 if (strlen(data) == i){
			 for(int j = i+1; j <= 120; j++ ){
				 circularBuffer[j][writeIndex] = 0;
			 }
		 }
		 // At this point the command should be stored in the x dimension of the 2d array

		 }// end of for loop
		 bufferLength ++;
		 writeIndex++;
	 }

}
// One alternative to check the marionette values from withing the each states ( Create a function read and call it )
// Otherwise leave read inside the ring buffer function and handling the marionettes values will be done first!
void ringBufferRead(){
	int counter = 0;
	    //check if the circular buffer is empty
	    // Initialize it to zero
	     if(bufferLength == 0 && initOnceFlag == TRUE){

	    	 for (int i=0;i<100;i++){ // i signifys the y position
	    	 			 for (int j = 0; j<120; j ++){// j signifies the x position

	    	 				 circularBuffer[j][i]= 0;
	    	 			 }

	    	 	 }
	    	 initOnceFlag = FALSE;
	     }
		 // READ
		 // Then there is commands needed to be serviced in the circular buffer only when there is no
	     // data processing.
	     else if(bufferLength <= SIZE_OF_BUFFER && bufferLength != 0){ // && circular_buffer_readFlag){
             // substitute data for the last values added to circularBuffer
			 for(int j = 0; circularBuffer[j][readIndex] != '\0'; j++){
				 counter = j;
			 }

			 //found
			 for (int i = 0; i < counter+1; i++){
				 marionette_data[i] = circularBuffer[i][readIndex];
			 }

			 bufferLength --;
			 readIndex ++;

       //Then service data packets
       NominalValueCheck(NULL, marionette_data);
    }
}

