#include "BoardDetect.h"

#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

BoardDetect::BoardDetect(int tolerance):
	tolerance(tolerance)
{
}

void BoardDetect::DetectBoard(cv::UMat & cameraFrame, std::pair<int,int> coordinates)
{
	TemporarilyStoredFrame = cameraFrame;
	//// Convert frame to grey scale
	//cv::cvtColor(frame, TemporarilyStoredFrame, CV_RGB2GRAY);

	//// Blur image to reduce noise
	//cv::GaussianBlur(TemporarilyStoredFrame, TemporarilyStoredFrame, cv::Size(3, 3), 0);

	cv::namedWindow("Board Detection Frame", CV_WINDOW_AUTOSIZE);
	cv::imshow("Board Detection Frame", cameraFrame);

	//// Store value of seed point
	//int seedPixelValue = (int) TemporarilyStoredFrame.getMat(cv::ACCESS_READ).at<uchar>(coordinates.first, coordinates.second);

	//// Set current pixel iteration to value of seed point
	//int currentPixelValue = seedPixelValue;

	//std::cout << seedPixelValue << std::endl;

	//// Create currentPoint array to iterate through
	//int currentPoint[2];

	//// set currentPoint to point
	//currentPoint[0] = coordinates.first;
	//currentPoint[1] = coordinates.second;

	//while (currentPixelValue < (seedPixelValue + tolerance) & currentPixelValue >(seedPixelValue - tolerance)) {
	//	currentPixelValue = (int) TemporarilyStoredFrame.getMat(cv::ACCESS_READ).at<uchar>(currentPoint[0], currentPoint[1]);
	//	currentPoint[0]++;
	//	std::cout << currentPoint[0] << std::endl;
	//}

	//// Save horizontal extreme
	//coordinates1.first = currentPoint[0];

	//// reset currentPoint to point
	//currentPoint[0] = coordinates.first;
	//currentPoint[1] = coordinates.second;

	//do {
	//	currentPixelValue = TemporarilyStoredFrame.getMat(cv::ACCESS_READ).at<uchar>(currentPoint[0], currentPoint[1]);
	//	currentPoint[0]--;
	//} while (currentPixelValue < seedPixelValue + (tolerance & currentPixelValue) >(seedPixelValue - tolerance));

	//// Save horizontal extreme
	//coordinates2.first = currentPoint[0];

	//// reset currentPoint to point
	//currentPoint[0] = coordinates.first;
	//currentPoint[1] = coordinates.second;

	//do {
	//	currentPixelValue = TemporarilyStoredFrame.getMat(cv::ACCESS_READ).at<uchar>(currentPoint[0], currentPoint[1]);
	//	currentPoint[1]++;
	//} while (currentPixelValue < seedPixelValue + (tolerance & currentPixelValue) >(seedPixelValue - tolerance));

	//// Save horizontal extreme
	//coordinates1.second = currentPoint[1];

	//// reset currentPoint to point
	//currentPoint[0] = coordinates.first;
	//currentPoint[1] = coordinates.second;

	//do {
	//	currentPixelValue = TemporarilyStoredFrame.getMat(cv::ACCESS_READ).at<uchar>(currentPoint[0], currentPoint[1]);
	//	currentPoint[1]--;
	//} while (currentPixelValue < seedPixelValue + (tolerance & currentPixelValue) >(seedPixelValue - tolerance));

	//// Save horizontal extreme
	//coordinates2.second = currentPoint[1];
}

std::pair<int,int> BoardDetect::getCoordinates1()
{
	return coordinates1;
}

std::pair<int, int> BoardDetect::getCoordinates2()
{
	return coordinates2;
}


BoardDetect::~BoardDetect()
{
	TemporarilyStoredFrame.release();
}
