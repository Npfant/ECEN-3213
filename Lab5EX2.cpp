// Lab 5, Exercise 2
//Authors:Nathan Fant, Nate Craig
// g++ -std=c++14 Lab5EX2.cpp -o Lab5EX2 -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>
#include <chrono>
#include <ratio>
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unistd.h>
/* Complete the code: include the file lab_util.cpp  */
#include "lab_util.cpp"

using namespace std::chrono;
using namespace std;


/* Complete the code: Set button pin */
int BUTTON = 12;

// State enum definition
enum State {
	STOPPED,
	RUNNING,
	RESET
};

// Global variables
State state = STOPPED;
high_resolution_clock::time_point begin_time;
unsigned int counter = 0;

/* Complete the code, implement the timer */
void press_button() {
    pinMode(BUTTON,INPUT);
    delay(100);
    int val = digitalRead(BUTTON);
    if(val == 1){
        //cout << state<< endl;
    if (state == STOPPED) {
        /* Complete the code */
        //cout << "Test" << endl;
        begin_time = high_resolution_clock::now();
        state = RUNNING;
    } else if (state == RUNNING) {
        /* Complete the code  */
        high_resolution_clock::time_point end_time = high_resolution_clock::now();
        counter++;
        string timestamp = formatTime(begin_time, end_time);
        //cout << "Test2" << endl;
        if(counter == 1 || counter == 21)
            cout << counter << "st place: " << timestamp << endl;
        else if(counter == 2 || counter == 22)
            cout << counter << "nd place: " << timestamp << endl;
        else if (counter == 3 || counter == 23)
            cout << counter << "rd place: " << timestamp << endl;
        else
            cout << counter << "th place: " << timestamp << endl;
    }
    else{}
    double sleep_duration = 0.1;
    int reset_counter = 0;
    int counter_max = 4 / sleep_duration;
    while(digitalRead(BUTTON)){
        //cout << "test" << endl;
        usleep(sleep_duration * 1000 * 1000);
        reset_counter++;
        if(reset_counter >= counter_max){
            state = STOPPED;
            cout << "Reset" << endl;
            counter = 0;
            break;
    }
    }
    
    }
}

int main(){

    // Set up WiringPi
    if (wiringPiSetupGpio () == -1) exit (1);

    //state = STOPPED;
    //counter = 0;
    unsigned int button_time = 0;

    cout << "Hold button to reset" << endl;


    /* Complete the code to implement the timer  */

    wiringPiISR(BUTTON, INT_EDGE_RISING, &press_button);


    while(true){
    }
    /*
    Tips:
    1) Set digital pins to input, Use pinMode(PIN, MODE)
    2) Use wiringPiISR() to get the interupt
    3) Read the state of pin: digitalRead(PIN)
    4) Use delay() to set a delay
    5) Define the state for the timer
    6) Use the formatTime() in lab_util.cpp
    */


    return 0;
}