//Author: Nathan Perry Fant
//Use g++ -std=c++14 -o morseCode morseCode.cpp -lwiringPi

#include <chrono>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <stdlib.h>
using namespace std;

void stop(int);

string morseStr;
string str;
int counter = 0;
auto t_start = std::chrono::high_resolution_clock::now();

int button_state = LOW;
bool changed = false;
int LCDAddr = 0x27; //May have to change
int BLEN = 1; //1--open backlight 0--close backlight
int fd; //Linux file descriptor


// Send an 16 bits data to LCD buffer
void write_word(int data){
    int temp = data;
    if ( BLEN == 1 )
        temp |= 0x08;
    else
        temp &= 0xF7;

    /* Complete the code: write into the device using wiringPiI2CWrite */
    wiringPiI2CWrite(fd,temp);

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

    /* Complete the code: 4bit mode 2 Lines & 5*8 dots */
    send_command(0x28);
    delay(5);

    send_command(0x0C); // Enable display without cursor
    delay(5);

    /* Complete the code: Clear screen */
    send_command(0x01); 
    
}

// Clear screen
void clear(){
    /* Complete the code:clear Screen */
    send_command(0x01);

}

// Print the message on the lcd
void write(int x, int y, const char data[]){
    int addr, i;
    int tmp;
    if (x < 0) x = 0; if (x > 15) x = 15;
    if (y < 0) y = 0; if (y > 1)  y = 1;

    /* Complete the code: Target address, Move cursor */
    addr = 0x80 + 0x40 * y + x;

    // Set the address
    send_command(addr);
    
    tmp = strlen(data);
    for (i = 0; i < tmp; i++){
        send_data(data[i]);
    }
}

char morseLib(string morse){
	char Char;

	if(morse == ".-") Char = 'A';
	if(morse == "-...") Char = 'B';
	if(morse == "-.-.") Char = 'C';
	if(morse == "-..") Char = 'D';
	if(morse == ".") Char = 'E';
	if(morse == "..-.") Char = 'F';
	if(morse == "--.") Char = 'G';
	if(morse == "....") Char = 'H';
	if(morse == "..") Char = 'I';
	if(morse == ".---") Char = 'J';
	if(morse == "-.-") Char = 'K';
	if(morse == ".-..") Char = 'L';
	if(morse == "--") Char = 'M';
	if(morse == "-.") Char = 'N';
	if(morse == "---") Char = 'O';
	if(morse == ".--.") Char = 'P';
	if(morse == "--.-") Char = 'Q';
	if(morse == ".-.") Char = 'R';
	if(morse == "...") Char = 'S';
	if(morse == "-") Char = 'T';
	if(morse == "..-") Char = 'U';
	if(morse == "...-") Char = 'V';
	if(morse == ".--") Char = 'W';
	if(morse == "-..-") Char = 'X';
	if(morse == "-.--") Char = 'Y';
	if(morse == "--..") Char = 'Z';
	if(morse == "-----") Char = ' ';
	
	return Char;
}

void press_input() {
	pinMode(18, INPUT);
    int state = digitalRead(18);
	if(state == 1){
		t_start = std::chrono::high_resolution_clock::now();
		counter++;
	}
	if(state == 0){
		if(counter == 1){
			auto t_end = std::chrono::high_resolution_clock::now();
			double totTime = std::chrono::duration<double, std::milli>(t_end-t_start).count();
			if(totTime > 750 )
				morseStr.push_back('-');
			else
				morseStr.push_back('.');
			cout << "Current Input: " << morseStr << endl;
			counter = 0;
		}
	}
}
void press_reset() {
	pinMode(23, INPUT);
	int state = digitalRead(23);
	if(state == 1){
		cout << "Input Reset" << endl;
		morseStr.clear();
	}
}
void press_confirm() {
	clear();
	pinMode(24, INPUT);
	int state = digitalRead(24);
	if(state == 1){
		str += morseLib(morseStr);
		write(0,0,str.c_str());
	}
}


int main(){
	//Initialize the wiringPi library.
	if(wiringPiSetupGpio() == -1)  exit(1);

	fd = wiringPiI2CSetup(LCDAddr);
	init();

	wiringPiISR(18, INT_EDGE_RISING, &press_input);
	wiringPiISR(18, INT_EDGE_FALLING, &press_input);
	wiringPiISR(23, INT_EDGE_BOTH, &press_confirm);
	wiringPiISR(24, INT_EDGE_BOTH, &press_reset);

}

void stop(int sig){exit(1);}
