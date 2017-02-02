//**************************************************
//ECE 578
//Project Gesture Recognition using Kinect and Open CV

//Softwares used: Visual Studios 2015
//language: C++
//Tools and HW: Kinect and Arduino

//Author: Sukrut Kelkar

//Description: Using Kinect Sensor the skeleton and the join location
//of the joints (20 joints) is read. Using this location in terms of x and y
//we are setting a character variable with a specific string indicating a 
//particular motion. This charater is then sent to a serial port.
//The arduino reads this character and depending on the value of the character
//it glows a particular led.
//****************************************************

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <conio.h>
#include <string>
#include "tserial.h"
#include "bot_control.h"

using namespace cv;
using namespace std;

//Array in which joint positions are stored
int x[20] = { 0 };
int y[20] = { 0 };

//Flags
int sht = 0;
int ctr = 0;
int se = 0;

float mx[20] = { 0 }, my[20] = { 0 };

//Flag to indicating detection should begin
int detectMotion = 0;

//Character variable for serial communication
char Char2arduino [1];

//Draws the skeleton with all the joints using OpenCV Mat
Mat draw(Mat image)
{
	line(image, Point(x[3], y[3]), Point(x[2], y[2]), CV_RGB(0, 255, 0)); //Head - shoulder
	line(image, Point(x[2], y[2]), Point(x[1], y[1]), CV_RGB(0, 255, 0)); //shoulder - spine
	line(image, Point(x[1], y[1]), Point(x[0], y[0]), CV_RGB(0, 255, 0)); // spine - hip
	line(image, Point(x[11], y[11]), Point(x[10], y[10]), CV_RGB(0, 255, 0)); //right hand - wrist
	line(image, Point(x[10], y[10]), Point(x[9], y[9]), CV_RGB(0, 255, 0)); // right wrist to elbow
	line(image, Point(x[9], y[9]), Point(x[8], y[8]), CV_RGB(0, 255, 0)); //right elbow to shoulder
	line(image, Point(x[8], y[8]), Point(x[2], y[2]), CV_RGB(0, 255, 0)); // shoulder right to shoulder center
	line(image, Point(x[2], y[2]), Point(x[4], y[4]), CV_RGB(0, 255, 0)); //shoulder center to shoulder left
	line(image, Point(x[4], y[4]), Point(x[5], y[5]), CV_RGB(0, 255, 0)); // left shoulder to elbow
	line(image, Point(x[5], y[5]), Point(x[6], y[6]), CV_RGB(0, 255, 0)); // left elbow to wrist
	line(image, Point(x[6], y[6]), Point(x[7], y[7]), CV_RGB(0, 255, 0)); // left wrist to hand
	line(image, Point(x[0], y[0]), Point(x[16], y[16]), CV_RGB(0, 255, 0)); // hip center to hip right
	line(image, Point(x[16], y[16]), Point(x[17], y[17]), CV_RGB(0, 255, 0)); // right hip to knee
	line(image, Point(x[17], y[17]), Point(x[18], y[18]), CV_RGB(0, 255, 0)); // right knee to ankle
	line(image, Point(x[18], y[18]), Point(x[19], y[19]), CV_RGB(0, 255, 0)); // right ankle to foot
	line(image, Point(x[0], y[0]), Point(x[12], y[12]), CV_RGB(0, 255, 0)); // hip center to hip left
	line(image, Point(x[12], y[12]), Point(x[13], y[13]), CV_RGB(0, 255, 0)); // left hip to knee
	line(image, Point(x[13], y[13]), Point(x[14], y[14]), CV_RGB(0, 255, 0)); // left knee to ankle
	line(image, Point(x[14], y[14]), Point(x[15], y[15]), CV_RGB(0, 255, 0)); // left ankle to foot
	return image;
}

//For recognizing the gesture
Mat gesture(Mat image)
{
	Mat sto = image;
	if (detectMotion == 0)
	{
		putText(image, "ENTER DETECTION MODE", Point(200, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 200, 200), 4);
		//if ((y[11] - 100)<10 && (x[11] - 200)<10)//if(ctr>5)  //position of hand or gesture....
		if ((y[11] < 100) && (200 < x[11] < 600))
		{
			image = sto;
			detectMotion = 1;//Setting the detection falg depending on the position of the right hand
			ctr = 0;
		}
	}
    
    //Detection mode enetered
	if (detectMotion == 1)
	{
		if (sht == 1)
		{
			putText(image, "CONFIRM", Point(320, 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(200, 0, 200), 4);
			putText(image, "YES", Point(220, 140), FONT_HERSHEY_SIMPLEX, 1, Scalar(200, 0, 200), 4);
			putText(image, "NO", Point(420, 140), FONT_HERSHEY_SIMPLEX, 1, Scalar(200, 0, 200), 4);
			if ((y[11] - 140)<10 && (x[11] - 220)<10)
				sht = 2;
			else if ((y[11] - 140)<10 && (420 - x[11])<10)
				sht = 0;
		}

		if (sht == 0)
		{
			putText(image, "EXIT", Point(500, 380), FONT_HERSHEY_SIMPLEX, 1, Scalar(200, 0, 200), 4);
			putText(image, "DETECTING....", Point(200, 80), FONT_HERSHEY_SIMPLEX, 1, Scalar(200, 0, 200), 4);
			if ((380 - y[11])<10 && (500 - x[11])<10)
				sht = 1;
			if (my[10]>my[9])// hello start //wrist goes above elbow
			{
				if (se == 0)
				{
					if (100 * (mx[11] - mx[8])<5)
						se = 1;
				}
				else if (se == 1)
				{
					if (mx[11]>mx[9]) // wrist x axis is more than elbow
					{
						image = sto;
						putText(image, "Hello", Point(20, 300), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 4);
						cout << "hello\n";
						waitKey(200);
						se = 0;
					}
				}
			} // hello end
//Depending on the specific conditions it determines the gesture and sets the serial communication variable
			if ((x[11] - x[3])<10 && (y[11] - y[3])<10 && (y[7] > y[4]))
			{
				putText(image, "I need a Doctor", Point(20, 400), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 4);
				strcpy(Char2arduino , "N");
			}
			else if ((x[11] - x[7])<5 && (x[11] - x[1])<10 && (y[11] - y[7])<10 && (y[11] - y[1])<7)
			{
				putText(image, "Ahh!! I am Hungry", Point(20, 400), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 4);
				strcpy(Char2arduino, "H");

			}
			else if (((x[11]>x[16]) && (x[7] > x[16])) || ((x[11] < x[12]) && (x[7] < x[12])))
			{
				putText(image, "Dancing!!", Point(20, 400), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 4);
				strcpy(Char2arduino, "N");
			}
			else if ((y[8] > y[11]) && (y[4] > y[7]) && (x[11]>x[9]) && (x[7] < x[5]))
			{
				putText(image, "I Don't Know", Point(20, 400), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 4);//Both hands up
				strcpy(Char2arduino, "B");
			}
			//Right arm up    
			else if ((y[8] > y[11]) && (x[11] > x[8]))
			{
				putText(image, "Right Arm Up ", Point(20, 400), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 4);
				strcpy(Char2arduino, "R");
			}
			//Left arm
			else if ((y[4] > y[7]) && (x[7] < x[4]))
			{
				putText(image, "Left Arm Up", Point(20, 400), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 4);
				strcpy(Char2arduino, "L");
			}
			else
			{
				putText(image, "Lazy guy do something", Point(20, 450), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 4);
				strcpy(Char2arduino, "N");
			}
		}

	}

	return image;
}

//Serial Communication Code
serial comm;

int SerialComm(char data[])
{
	//printf("Communicating to Arduino.....\n\n");
	comm.startDevice("COM8", 9600);
	//printf("...Communication initiated...\n\n");
	comm.send_data(data);
	//printf("Data: %s\n\n", data);
	//printf("...Communication complete...\n\n");

	comm.stopDevice();
	//getchar();
	return (0);
}

//Main function
int main()
{
	float fx = 0, fy = 0, fx1 = 0, fy1 = 0;
	int px = 0, py = 0, px1 = 0, py1 = 0;
	char inp[20];
	Mat frame;

	cout << "Hello world" << endl;

	NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);

	NUI_SKELETON_FRAME ourframe;

	while (1)
	{
		Mat sam = Mat::zeros(480, 640, CV_8UC3);//CV_8UC3 is an 8-bit unsigned integer matrix/image with 3 channels

		NuiSkeletonGetNextFrame(0, &ourframe);
		for (int i = 0; i < 6; i++)
		{
			if (ourframe.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED)
			{
				cout << "Right Hand: ";
				cout << ourframe.SkeletonData[i].SkeletonPositions[11].y << endl;
				for (int k = 0; k<20; k++)
				{
					fy1 = ourframe.SkeletonData[i].SkeletonPositions[k].y;
					fx1 = ourframe.SkeletonData[i].SkeletonPositions[k].x;
					mx[k] = fx1;
					my[k] = fy1;
					//Resizing the display frame
                    px1 = (int)320 * (fx1 + 1);
					py1 = (int)240 * (fy1 + 1);
					py1 = 480 - py1;
					x[k] = px1;
					y[k] = py1;
					circle(sam, Point(px1, py1), 10, Scalar(0, 0, 255));
				}
				sam = draw(sam);
				sam = gesture(sam);
			}
			//Calling the Serial Communication code
            strcpy(inp, Char2arduino);//-----Left Arm Up---/----Both Arms Up---/--Both Arms on Hip-
			SerialComm(inp);
			
		}
		
		imshow("win", sam);
		waitKey(2);
		if (sht == 2)
		{
			break;
		}
		//video test
		system("cls");
	}
	NuiShutdown();
	return 0;
}