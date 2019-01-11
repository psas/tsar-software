#include <wiringPi.h>
#include <stdio.h>

#define  LedPin    0 //gpio 17 or pin 11 on pi

int main(void)
{
    if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
        printf("setup wiringPi failed !");
        return 1; 
    }
    printf("linker LedPin : GPIO %d(wiringPi pin)\n",LedPin); //when initialize wiring successfully,print message to screen
    

    while(1){
            pinMode(LedPin, OUTPUT);
            digitalWrite(LedPin, LOW);
            delay(500);
            pinMode(LedPin, OUTPUT);
            digitalWrite(LedPin, HIGH);
            delay(500);
    }

    return 0;
}

