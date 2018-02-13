/**
********************************************************************************
*
*	@file		MovementDetect.cpp
*
*	@brief		Movement Detector Class
*
*	@version	1.1
*
*	@date		27/04/2017
*
*	@author		George Edward Hugh Othen
*
*
********************************************************************************
*/

// -- Includes --
#include "MovementDetect.h"

#include <ctime>
#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

//#define DEBUG

//******************************************************************************
//    Movement Detector
//******************************************************************************
MovementDetect::MovementDetect(int lectureWriteTime, int compareInterval, int zoomInterval, int writeSensitivity, float largeChange, float smallChange) :
	writeTimeInterval(lectureWriteTime),
	compareTimeInterval(compareInterval),
	zoomTimeInterval(zoomInterval),
	firstFrameSaved(false),
	zoomed(false),
	timersStarted(false),
	writeCounter(0),
	leavingFrameCounter(0),
	Count(0),
	saveCounter(0),
	writeSensitivity(writeSensitivity),
	writing(false),
	NCC(0.0f),
	currentNCC(0.0f),
	largeChange(largeChange),
	smallChange(smallChange)
{
}

void MovementDetect::DetectMovement(cv::UMat & cameraFrame, cv::Rect &imageCrop)
{
	// Start Timers
	if (!timersStarted) {
		writeDuration = clock(); // Start Save Timer
		zoomDuration = clock(); // Start Zoom Timer
		compareDuration = clock(); // Start Compare Timer
		timersStarted = true;
	}
	
	// Increment Timers, Store as seconds
	zoomTimer = (clock() / 1000.0f) - (zoomDuration / 1000.0f); 
	compareTimer = (clock() / 1000.0f) - (compareDuration / 1000.0f);
	writeTimer = (clock() / 1000.0f) - (writeDuration / 1000.0f);

	// Ensure that frames have been saved, calculate NCC
	if (!firstFrameSaved) {
		SaveFrame(cameraFrame, imageCrop);
		SaveCurrentFrame(cameraFrame, imageCrop);
		writeDuration = clock(); // Reset saveTimer to 0
		NCC = getNCC();
		firstFrameSaved = true;
	}

	// Graduate Comparison
	if (compareTimer > compareTimeInterval & !zoomed & firstFrameSaved) {
		// Store current frame
		SaveCurrentFrame(cameraFrame, imageCrop);

		// Construct Mean NCC
		currentNCC += getNCC();

		// Calculate Mean NCC
		if (Count >= 5) {
			NCC = currentNCC / 5.0f;
			Count = 0;
			currentNCC = 0.0f;
		}

		// Count number of frames added to mean construct
		Count++;

#ifdef DEBUG
		std::cout << "Comparing Frames | NCC: " << NCC << std::endl;
#endif 		
	}
	// If A large change has occurred
	if (NCC <= largeChange) {
#ifdef DEBUG
		std::cout << "Detected a change" << std::endl;
#endif 
		// If writing mode hasn't already been set
		if (!writing) {
			// Count frames
			writeCounter++;
#ifdef DEBUG
			std::cout << "WriteCounter: " << writeCounter << " | Sentivity: " << writeSensitivity << std::endl;
#endif 
		}
		// If large change has lasted longer than sensitivity
		if (writeCounter > writeSensitivity) {
#ifdef DEBUG
			std::cout << "Writing in progress" << std::endl;
#endif 
			// writing mode detected
			writing = true;
			writeCounter = 0;

			// Reset Writing Mode Timer
			writeDuration = clock();
		}
	}
	else {
#ifdef DEBUG
		std::cout << "No Changes Detected, Resetting WriteCounter" << std::endl;
		std::cout << "WTF" << std::endl;
#endif 
		// Reset Comparison Timer
		compareDuration = clock();

		// Writing hasn't taken place, reset counter
		if (writeCounter != 0) {
			writing = false;
			writeCounter = 0;
#ifdef DEBUG
			std::cout << "False Alarm, No Writing Taking Place" << std::endl;
#endif 
		}

	}
	// Check if timer has elapsed
	if (writing & writeTimer > writeTimeInterval) {
		// Disable Writing mode
		writing = false;
		// Recheck large change
		if (NCC <= largeChange) {
			// Writing is still taking place
			writing = true;

			// Reset Writing Timer
			writeDuration = clock();
#ifdef DEBUG
			std::cout << "Still Writing..." << std::endl;
			std::cout << "NCC: " << NCC << std::endl;
#endif 
		}
		// Check if there is writing on the board
		else if (NCC < smallChange & NCC > largeChange) {
			writing = true;

			// Allow time for lecturer to leave frame
			if (leavingFrameCounter >= 10) {
				writing = false;
#ifdef DEBUG
				std::cout << "Detected Writing, Zooming In" << std::endl;
				std::cout << "NCC: " << NCC << std::endl;
#endif 
				// Enable Zoom
				zoomed = true;

				// Reset Counter
				leavingFrameCounter = 0;
			}

			// Increment Counter
			leavingFrameCounter++;
#ifdef DEBUG
			std::cout << "Leaving Frame, counting: " << leavingFrameCounter << std::endl;
#endif 
		}
		else {
#ifdef DEBUG
			std::cout << "Nothing was Written" << std::endl;
			std::cout << "NCC: " << NCC << std::endl;
#endif 
			// Reset Counter
			leavingFrameCounter = 0;
		}
	}

	// If zoom timer hasn't elapsed
	if (zoomTimer < zoomTimeInterval & zoomed) {
#ifdef DEBUG
		std::cout << "Zooming in for " << zoomTimeInterval << " seconds" << std::endl;
#endif 
	}
	// If zoom timer has elapsed
	else if (zoomTimer > zoomTimeInterval & zoomed) {
#ifdef DEBUG
		std::cout << "Zoom Time Interval Elapsed, Disabling Zoom" << std::endl;
		std::cout << "NCC: " << NCC << std::endl;
#endif 
		// Disable Zoom
		zoomed = false;			
	}
	// Attempt to store updated board
	else if (zoomTimer > zoomTimeInterval & !zoomed) {
#ifdef DEBUG
		std::cout << "Attempting to Save Frame" << std::endl;
		std::cout << "NCC: " << NCC << std::endl;
#endif 
		
		// Allow Time to check if frame is obstructed
		if (NCC >= largeChange & saveCounter >= 10) {
			SaveFrame(cameraFrame, imageCrop);
			zoomDuration = clock();
			writing = false;
			saveCounter = 0;
		}
		else if (NCC <= largeChange){
			saveCounter = 0;
#ifdef DEBUG
			std::cout << "Board Obstructed, Waiting to Update Frame" << std::endl;
			std::cout << "NCC: " << NCC << std::endl;
#endif 
		}
		// Increment Counter
		saveCounter++;			
	}
	// Reset Zoom Timer
	else if (!zoomed) {
		zoomDuration = clock();
	}
}


void MovementDetect::SaveFrame(cv::UMat &cameraFrame, cv::Rect & imageCrop)
{
	// Store a copy of current board state iteration
	savedFrame = cameraFrame(imageCrop).clone();
#ifdef DEBUG
	cv::imshow("SavedImage", savedFrame);
#endif 
	// Convert to greyscale
	cv::cvtColor(savedFrame, savedFrame, CV_BGR2GRAY);

	// Filter Noise
	cv::GaussianBlur(savedFrame, savedFrame, cv::Size(3, 3), 0);
}

void MovementDetect::SaveCurrentFrame(cv::UMat &cameraFrame, cv::Rect & imageCrop)
{
	// Store a copy of current frame, board crop
	savedCurrentFrame = cameraFrame(imageCrop).clone();
#ifdef DEBUG
	cv::imshow("SavedCurrentImage", savedCurrentFrame);
#endif 
	// Convert to greyscale
	cv::cvtColor(savedCurrentFrame, savedCurrentFrame, CV_BGR2GRAY);

	// Filter Noise
	cv::GaussianBlur(savedCurrentFrame, savedCurrentFrame, cv::Size(3, 3), 0);
}

//----------------------------------------------------------------
double MovementDetect::getSum(cv::UMat & Frame)
//----------------------------------------------------------------
{
	cv::Scalar sum = cv::sum(Frame);
	return sum[0];
}

//----------------------------------------------------------------
double MovementDetect::getAverage(cv::UMat & Frame)
//----------------------------------------------------------------
{
	cv::Scalar avg = cv::mean(Frame);
	return avg[0];
}

double MovementDetect::getNCC()
{
	double min, max;
	cv::matchTemplate(savedFrame, savedCurrentFrame, result, CV_TM_CCORR_NORMED);
	cv::minMaxLoc(result, &min, &max);
	return min;
}

MovementDetect::~MovementDetect()
{
}
