/**
********************************************************************************
*
*	@file		MovementDetect.h
*
*	@brief		Header File for Movement Detector Class
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

#pragma once

// -- Includes --
#include <vector>

#include "opencv2/imgproc/imgproc.hpp"

class MovementDetect
{
public:
	//--------------------------------------------------------------------------
	/// Default constructor.
	/**
	*	@lecturerWriteTime: Number of seconds to wait before checking if lecturer is still writing
	*	@compareInterval: Number of seconds to wait before adding current frame to mean NCC
	*	@zoomInterval: Number of seconds to zoom onto board
	*	@writeSensitivity: Number of frames to wait before engaging writing mode
	*	@largeChange: NCC Value considered a large change (typical value 0.980 | 0.970) Higher for better lecturer detection, lower for more absolute lecturer detection
	*	@smallChange: NCC Value considered a small change (typical value 0.9987 | 0.9975) Lower for resistances to noise, Higher for increased sensitivity
	*/
	//--------------------------------------------------------------------------
	MovementDetect(int lecturerWriteTime, int compareInterval, int zoomInterval, int writeSensitivity, float largeChange, float smallChange);

	/**
	*	Detects movement on the board
	*	@cameraFrame: the current camera frame to compare
	*	@imageCrop: the selected region the board is located within
	*/
	void DetectMovement(cv::UMat & cameraFrame, cv::Rect & imageCrop);

	/**
	*	Stores the latest board state revision
	*	@cameraFrame: the current camera frame to compare
	*	@imageCrop: the selected region the board is located within
	*/
	void SaveFrame(cv::UMat & cameraFrame, cv::Rect & imageCrop);

	/**
	*	Stores the current frame temporarily
	*	@cameraFrame: the current camera frame to compare
	*	@imageCrop: the selected region the board is located within
	*/
	void SaveCurrentFrame(cv::UMat & cameraFrame, cv::Rect & imageCrop);

	//------------------------------------------------------------------------
	//// Destructor
	//------------------------------------------------------------------------
	~MovementDetect();

	// Establish if zoom is enabled
	bool zoomed;
private:
	/**
	*	Sum all pixel intensities
	*	@cameraFrame: the current camera frame to compare
	*	@return: The sum of all pixel intensities
	*/
	double getSum(cv::UMat & cameraFrame);

	/**
	*	Average all pixel intensities
	*	@cameraFrame: the current camera frame to compare
	*	@return: The average pixel intensity
	*/
	double getAverage(cv::UMat & cameraFrame);

	/**
	*	Calculate the Normalised Cross-Correlation
	*	@return: The Normalised Cross-Correlation
	*/
	double getNCC();

	// The temporary frames
	cv::UMat savedFrame, savedCurrentFrame, result;

	// Timers and Durations (for timer resets)
	float writeTimer, compareTimer, zoomTimer, writeDuration, compareDuration, zoomDuration;

	// Time Intervals to wait for, NCC, current state of mean NCC calc, sensitivities
	float writeTimeInterval, compareTimeInterval, zoomTimeInterval, NCC, currentNCC, largeChange, smallChange;

	// Frame counters
	int writeCounter, writeSensitivity, Count, saveCounter, leavingFrameCounter;

	// Establish set tasks have begun
	bool firstFrameSaved, timersStarted, writing;
};

