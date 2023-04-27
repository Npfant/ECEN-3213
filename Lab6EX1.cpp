//Authors: nathan fant, Nate Craig
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;



// g++ -std=c++14 Lab6EX1.cpp -o Lab6EX1 -lwiringPi

int main(void){
    
    wiringPiSetupGpio();

    /* 1. set mode for the three pins, the pin mode of the PWM pin should be PWM_OUTPUT */
    pinMode(27,OUTPUT);
    pinMode(22,OUTPUT);
    pinMode(12,PWM_OUTPUT);
	/* 2. write HIGH or LOW to the two I/O pins to control the rotation direction */
    //while (true) {
    digitalWrite(27,LOW);
    digitalWrite(22,HIGH);


    
    /* 3. use function pwmWrite to control the motor speed. Refer to document pwm.pdf for more information. */
    while(true) {
        
    pwmWrite(12,500);
  //  delay(3000);
   // digitalWrite(27,LOW);
    //digitalWrite(22,LOW);
    }
    //tips:	run the code using sudo ./Lab6EX1

    return 0;
}

