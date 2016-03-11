// http://dogfeatherdesign.com/opencv-3-0-microsoft-visual-studio-2015-cmake-and-c/
#include "stdafx.h"

//#include <opencv\cv.h>   // This is the original code, but I couldn't get VideoCapture work correctly.
#include <opencv2/opencv.hpp>
#include <opencv\highgui.h>

using namespace cv;

void CamTestMain() {
	Mat image;						//Create Matrix to store image
	VideoCapture cap;				//initialize capture
	cap.open(0);
	namedWindow("window", 1);		//create window to show image
	while (1) {
		cap >> image;				//copy webcam stream to image
		imshow("window", image);	//print image to screen
		waitKey(33);				//delay 33ms
	}
}
