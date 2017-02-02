// SerialCommV2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include "tserial.h"
#include "bot_control.h"


//void SerialComm(char toSend[]);
serial comm;
int dataAvailable;


void SerialComm(char data[])
{
	printf("Communicating to Arduino.....\n\n");
	comm.startDevice("COM8", 9600);
	printf("...Communication initiated...\n\n");
	comm.send_data(data);
	printf("Data: %s\n\n", data);
	printf("...Communication complete...\n\n");

	comm.stopDevice();
	getchar();
}

void main()
{
	char inp[20] = "--Both Arms on Hip-";//-----Left Arm Up---/----Both Arms Up---/--Both Arms on Hip-
	SerialComm(inp);
	getchar();
}
