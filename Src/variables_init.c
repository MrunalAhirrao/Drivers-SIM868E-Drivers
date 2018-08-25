/*
This file contains initialization of Global variables
*/

#include "Variables.h"

FLAGS flags={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


char buffer[2][500]; //two buffers
int bufferCounter[2]; ///two counters
unsigned int uartISR=0, uartUser=1, commandFound = 0; 
unsigned char GPCntr=0;	
