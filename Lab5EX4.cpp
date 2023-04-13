// Lab 5, Exercise 3
// g++ -std=c++14 Lab5EX3.cpp -o Lab5EX3 -lwiringPi

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

/* Complete the code: I2c address of LCD, some LCD i2c address might be 0x27 */
int LCDAddr = 0x27;  

int BLEN = 1; //1--open backlight.0--close backlight

int fd;//linux file descriptor

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
int count = 0;
string place = " ";
char times[30][30] = {0};


// Send an 16 bits data to LCD buffer
void write_word(int data){
    int temp = data;
    if ( BLEN == 1 )
        temp |= 0x08;
    else
        temp &= 0xF7;

    wiringPiI2CWrite(fd, temp);
}

// Send control command to lcd
void send_command(int comm){
    int buf;
    // Send bit7-4 firstly
    buf = comm & 0xF0;
    buf |= 0x04;            // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
    // Send bit3-0 secondly
    buf = (comm & 0x0F) << 4;
    buf |= 0x04;            // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
}

// Send character to lcd
void send_data(int data){
    int buf;
    // Send bit7-4 firstly
    buf = data & 0xF0;
    buf |= 0x05;            // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
    // Send bit3-0 secondly
    buf = (data & 0x0F) << 4;
    buf |= 0x05;            // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
}

// Initialize the lcd
void init(){
    send_command(0x33); // Initialize 
    delay(5);
    send_command(0x32); // Initialize 
    delay(5);

    send_command(0x28);
    delay(5);

    send_command(0x0C); // Enable display without cursor
    delay(5);

    send_command(0x01);    
}

// Clear screen
void clear(){
    send_command(0x01);
}

// Print the message on the LCD
void write(int x, int y, const char data[]){
    int addr, i;
    int tmp;
    if (x < 0) x = 0; if (x > 15) x = 15;
    if (y < 0) y = 0; if (y > 1)  y = 1;

    addr = 0x80 + 0x40 * y + x;

    // Set the address
    send_command(addr);
    
    tmp = strlen(data);
    for (i = 0; i < tmp; i++){
        send_data(data[i]);
    }
}

void writeString(string data) {
    int addr, i;
    // Move cursor
    addr = 0x80 ;

    // set the address
    send_command(addr);
    
    for (i = 0; i < data.length(); i++){
        send_data(data[i]);
    }
}

void print_info()
{
    printf("\n");
    printf("|***************************|\n");
    printf("|    IIC 1602 LCD test      |\n");
    printf("| --------------------------|\n");
    printf("| | LCD |            | Pi   |\n");
    printf("| --------------------------|\n");
    printf("| | GND | connect to | GND  |\n");
    printf("| | VCC | connect to | 5V   |\n");
    printf("| | SDA | connect to | SDA.1|\n");
    printf("| | SCL | connect to | SCL.1|\n");
    printf("| --------------------------|\n");
    printf("|                   ECEN3213|\n");
    printf("|***************************|\n");
    printf("Program is running...\n");
    printf("Press Ctrl+C to end the program\n");
}
string timestamp = "00:00:000";
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
        timestamp = formatTime(begin_time, end_time);
        //cout << "Test2" << endl;
        cout << counter << endl;
        if(counter == 1 || counter == 21){
            place = to_string(counter) + "st: " + timestamp; 
            strcpy(times[counter], place.c_str());
            cout << times[counter] << endl;
        }
        else if(counter == 2 || counter == 22){
            place = to_string(counter) + "nd: " + timestamp; 
            strcpy(times[counter], place.c_str());
            cout << times[counter] << endl;
        }
        else if (counter == 3 || counter == 23){
            place = to_string(counter) + "rd: " + timestamp; 
            strcpy(times[counter], place.c_str());
            cout << times[counter] << endl;
        }
        else{
            place = to_string(counter) + "th: " + timestamp; 
            strcpy(times[counter], place.c_str());
            cout << times[counter] << endl;
            cout << times[1] << endl;
        }       
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
            count = 0;
            for(int i = 0; i < 30; i++){
                for(int j = 0; j < 30; j++)
                    times[i][j] = ' ';
            }
            place = " ";
            break;
    }
    }
    }
}
void press_two() {
    pinMode(18,INPUT);
    delay(100);
    int val = digitalRead(18);
    if(val == 1){
    if(state == RUNNING)
        state = RESET;
    else if (state == RESET){
        count++;
    }
    else{}
    }
}

int main(){

    high_resolution_clock::time_point timeBegin; 
    // Set up WiringPi
    if (wiringPiSetupGpio () == -1) exit (1);

    // Init I2C, assign a buffer handler to variable fd
    fd = wiringPiI2CSetup(LCDAddr);

    init();
    print_info();

    
    /* Complete here to implement the stopwatch  */
    wiringPiISR(BUTTON, INT_EDGE_RISING, &press_button);
    wiringPiISR(18, INT_EDGE_RISING, &press_two);


    while(true){
        if(state == RUNNING){
        string currTime = formatTime(timeBegin, high_resolution_clock::now());
        write(0,0,currTime.c_str());
        write(0,1,place.c_str());
        delay(100);
        }
        else if(state == RESET){
            clear();
            string reset = "STOPPED";
            write(0,0, reset.c_str());
            //cout << count << endl;
            if(count > counter){
                count = 1;
            }
            cout << times[count] << endl;
            write(0,1,times[count]);
            delay(200);
        }
        else{
            timeBegin =  high_resolution_clock::now();
        }
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
