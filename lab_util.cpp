// Lab 5, lab_util.h
//Authors: Nathan fant, Nate Craig

#include <stdio.h>
#include <string.h>
#include <chrono>
#include <ratio>
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unistd.h>

using namespace std::chrono;
using namespace std;


std::string formatTime(high_resolution_clock::time_point t_begin, high_resolution_clock::time_point t_end);

/* Complete the code: return the difference of t_begin and t_end in the "MM:SS:XXX" format */

std::string formatTime(high_resolution_clock::time_point t_begin, high_resolution_clock::time_point t_end) {

	duration<double> timeDiff = duration_cast<duration<double>>(t_end - t_begin);
	double totTime = timeDiff.count();
	int min = int(totTime) / 60;
	int sec = int(totTime) % 60;
	int milli = int((totTime - min * 60 - sec) * 1000);
	char time[20];
	sprintf(time, "%02d:%02d:%03d", min, sec, milli);
	return time;

	/* Tips:
	   1) Get the time difference of t_begin and t_end
	   2) Convert the time difference to 3 parts: MM, SS, XXX
	   3) Use sprintf() to generate the format time and return it
	*/

}
