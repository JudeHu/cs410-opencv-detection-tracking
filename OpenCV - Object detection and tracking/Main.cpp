/* 
OpenCV3.1 Moving Cars and Pedestrians.cpp : Defines the entry point for the console application.

Combination of Haar Detection + Multitracker approaches implemented in following papers
http://docs.opencv.org/trunk/d5/d07/tutorial_multitracker.html#gsc.tab=0
http://docs.opencv.org/3.0-beta/doc/tutorials/objdetect/cascade_classifier/cascade_classifier.html

Problem: Multitracker must be improved
*/

#include "stdafx.h"
#include <iostream>
#include <cstring>
#include <ctime>

#include <opencv2/opencv.hpp> 
#include <opencv2/tracking.hpp>


using namespace std;
using namespace cv;

CascadeClassifier cascade;

string getVideoPath(int i = 0)
{
	string path = "data/";

	string list[] = {
		"240 - IPU3030RV IP Camera Highway Surveillance.mp4",
		"240p - 1min - rush hour - British Highway traffic, congestion..mp4",
		"240p - Pedestrians Rescue Woman from underneath Car in SW China.mp4",
		"240p - Truck accident caught on police camera Motorway M621 (M62 Crash Leeds West Yorks UK).mp4",
		"480p - Truck accident caught on police camera Motorway M621 (M62 Crash Leeds West Yorks UK).mp4",
		"720p - 1min - rush hour - British Highway traffic, congestion..mp4",
		"720p - Pedestrians Rescue Woman from underneath Car in SW China.mp4",
		"720p - rush hour - British Highway traffic, congestion..mp4",
		"720p IPU3030RV IP Camera Highway Surveillance.mp4",
		"video1.avi",
		"video2.avi",
	};

	return path + list[i];
}

string getCascade(int i = 0)
{
	string path = "haarcascades/";

	string list[] = {
		"car_1.xml",
		"car_2.xml",
		"car_3.xml",
		"pedestrian_1.xml",
		"pedestrian_2.xml",
		"fullbody.xml"
	};

	return path + list[i];
}

vector<Rect2d> detectObject(Mat frame)
{
	vector<Rect2d> result;
	vector<Rect> objects;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	imshow("GRAY & EQUALIZE", frame_gray);

	//-- Detect objects
	cascade.detectMultiScale(frame_gray, objects, 1.01, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10), Size(60, 60));

	for (size_t i = 0; i < objects.size(); i++)
	{
		Point center(objects[i].x + objects[i].width*0.5, objects[i].y + objects[i].height*0.5);
		ellipse(frame, center, Size(objects[i].width*0.5, objects[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 2, 8, 0);
		
		result.push_back(Rect2d(objects[i].x, objects[i].y, objects[i].width, objects[i].height));
	}
	//-- Show what you got
	imshow("Detector", frame);

	return result;
}

int main()
{
	if (!cascade.load(getCascade(2)))
	{
		cout << "Error can't loading cascade" << endl;
		return -1;
	}

	string video = getVideoPath(2);
	string trackingAlg = "MEDIANFLOW"; // MEDIANFLOW=fast+size, KCF=med, TLD, MIL=slow, BOOSTING = VERY slow

	// create the tracker
	MultiTracker trackers(trackingAlg);

	// container of the tracked objects
	vector<Rect2d> objects;

	// Holds a frame at a time
	Mat frame;

	// set input video
	VideoCapture capture(video);

	// Open the output
	string outputFileName = "output/Result_" + to_string(std::time(nullptr)) + ".avi";
	VideoWriter outputVideo(outputFileName, CV_FOURCC('M', 'P', '4', '2'), capture.get(CV_CAP_PROP_FPS),
					cv::Size(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT)));

	if (!outputVideo.isOpened())
	{
		cout << "Could not open output video..." << endl;
		return -1;
	}

	if (!capture.isOpened())
	{
		cout << "Could not initialize capturing..." << endl;
		return -1;
	}


	capture.set(CAP_PROP_POS_FRAMES, 600);

	// do the tracking
	cout << "Start the tracking process, press ESC to quit." << endl;
	while(true)
	{
		// get frame from the video
		capture >> frame;


		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;

		objects = detectObject(frame);

		//quit when the tracked object(s) is not provided
		/*if (objects.size() < 1)
		{
			cout << "No tracking objects provided..." << endl;
			return 0;
		}*/

		// initialize the tracker
		trackers.add(frame, objects);

		//update the tracking result
		trackers.update(frame);

		// draw the tracked object
		for (unsigned i = 0; i < trackers.objects.size(); i++)
		{
			rectangle(frame, trackers.objects[i], Scalar(255, 0, 0), 2, 1);
			
			/*Point textPos(trackers.objects[i].x, trackers.objects[i].y);
			string text = "id: " + std::to_string(i);
			cv::putText(frame, text, textPos, CV_FONT_NORMAL, 0.5f, Scalar::all(255), 1, 8);*/
		}

		// show image with the tracked object
		imshow("Tracker", frame);

		outputVideo.write(frame);

		//quit on ESC button
		if (waitKey(1) == 27)break;
	}


	return 0;
}