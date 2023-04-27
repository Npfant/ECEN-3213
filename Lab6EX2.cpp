//Authors: Nathan Fant, Nate Craig
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


// g++ -std=c++14 Lab6EX2.cpp -o Lab6EX2 -lwiringPi

int speed = 0;
enum State {
            STOPPED,
            RUNNING
            };

State state = STOPPED;

void press_start() {
cout << "test" << endl;
pinMode(23,INPUT);
delay(1000);
int val = digitalRead(23);

if (val == 1) {
   // cout << "test 1" << endl;
    if (state == STOPPED) 
        state = RUNNING;

    else
        state = STOPPED;
         
    }
cout << state << endl;


}


void press_increment() {
cout << "increment" << endl;
pinMode(24,INPUT);
delay(1000);
int val = digitalRead(24);


if (val == 1) {
    speed += 100;
    cout << speed << endl;
    if (speed > 1024)
        speed = 0;
}
}


int main(void){
    
    wiringPiSetupGpio();
    /* 1. set mode for the three I/O pins, the pin mode of the PWM pin should be PWM_OUTPUT */
    pinMode(22,OUTPUT);
    pinMode(27,OUTPUT);
    pinMode(12,PWM_OUTPUT);

    wiringPiISR(23,INT_EDGE_RISING, &press_start);
    wiringPiISR(24,INT_EDGE_RISING, &press_increment);
    digitalWrite(27,LOW);
    digitalWrite(22,LOW);
    while(true){
       
    // control the motor using two buttons 

        if (state == STOPPED) {
            //cout << "test 3" << endl;
            digitalWrite(22,LOW);
            digitalWrite(27,LOW);
        }
        if (state == RUNNING) {
           // cout << "test 4" << endl;
            digitalWrite(27,LOW);
            digitalWrite(22,HIGH);
            pwmWrite(12,speed);

        }
        }



    return 0;
}

