// http://docs.opencv.org/trunk/d5/d07/tutorial_multitracker.html#gsc.tab=0
#include "stdafx.h"


/*----------------------------------------------
* Usage:
* example_tracking_multitracker <video_name> [algorithm]
*
* example:
* example_tracking_multitracker Bolt/img/%04d.jpg
* example_tracking_multitracker faceocc2.webm KCF
*--------------------------------------------------*/

#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;
using namespace cv;

string getVideoPath_(int i = 0) {
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

int mainT() 
{
	// set the default tracking algorithm
	std::string trackingAlg = "MEDIANFLOW"; // MEDIANFLOW=fast+size, KCF=med, TLD, MIL=slow, BOOSTING = VERY slow
	std::string video = getVideoPath_(9); // 9, 1, 0,
	std::string windowName = "Algorithm: " + trackingAlg + " | " + video;
	
	cout << windowName << endl << endl;

	// create the tracker
	MultiTracker trackers(trackingAlg);

	// container of the tracked objects
	vector<Rect2d> objects;

	// set input video
	VideoCapture cap(video);

	Mat frame;

	// get bounding box
	cap >> frame;
	selectROI(windowName, frame, objects);

	//quit when the tracked object(s) is not provided
	if (objects.size()<1)
		return 0;

	// initialize the tracker
	trackers.add(frame, objects);

	// do the tracking
	printf("Start the tracking process, press ESC to quit.\n");
	for (;;) 
	{
		// get frame from the video
		cap >> frame;

		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;

		//update the tracking result
		trackers.update(frame);

		// draw the tracked object
		for (unsigned i = 0; i<trackers.objects.size(); i++)
			rectangle(frame, trackers.objects[i], Scalar(255, 0, 0), 2, 1);

		// show image with the tracked object
		imshow(windowName, frame);

		//quit on ESC button
		if (waitKey(1) == 27)break;
	}
	return 0;
}

