/**
********************************************************************************
*
*	@file		Camera.cpp
*
*	@brief		Camera Class
*
*	@version	1.0
*
*	@date		24/1/2017
*
*	@author		George Edward Hugh Othen
*
*
********************************************************************************
*/

// -- Includes --
#include "Camera.h"
#include "Error.h"

#include <vector>

#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

// Set Camera
VideoCapture cap(0);

Camera::Camera(std::pair<int, int> frameDimensions)
{
	// Set Camera Frame Size
	cap.set(CV_CAP_PROP_FRAME_WIDTH, frameDimensions.first);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, frameDimensions.second);

	if (!cap.isOpened())  // if not successful, exit program
	{
		error("Unable to open camera");
	}
}

Camera::~Camera()
{
	cap.release();
	frame.release();
}

const UMat& Camera::getFrame() {
	bool bSuccess = cap.read(frame); // read new frame
	
	if (!bSuccess) {
		error("Cannot read from video stream");
	}

	return frame;
}

double Camera::getFrameWidth() {
	return cap.get(CV_CAP_PROP_FRAME_WIDTH);
}

double Camera::getFrameHeight() {
	return cap.get(CV_CAP_PROP_FRAME_HEIGHT);
}

double Camera::getFPS() {
	return cap.get(CV_CAP_PROP_FPS);
}
