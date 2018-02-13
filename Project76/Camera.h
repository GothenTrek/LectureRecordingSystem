/**
********************************************************************************
*
*	@file		Camera.h
*
*	@brief		Header File for Camera Class
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

#define PROJ_MAT cv::UMat

//#define PROJ_MAT cv::Mat


class Camera
{
public:
	//--------------------------------------------------------------------------
	/// Default constructor.
	/**
	* @frameDimensions: The dimensions expected of the camera output
	*/
	//--------------------------------------------------------------------------
	Camera(std::pair<int, int> frameDimensions);

	//------------------------------------------------------------------------
	//// Destructor
	//------------------------------------------------------------------------
	~Camera();

	/**
	* Get the current camera frame
	* @return: The Current Camera Frame
	*/
	const PROJ_MAT& getFrame();

	/**
	* return: The width of the camera frame
	*/
	double getFrameWidth();

	/**
	* return: The height of the camera frame
	*/
	double getFrameHeight();

	/**
	* return: The frame rate of the camera stream
	*/
	double getFPS();
	
private:
	// Temporary Frame
	PROJ_MAT frame;	
};

