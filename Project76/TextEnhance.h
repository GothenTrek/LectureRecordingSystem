/**
********************************************************************************
*
*	@file		TextEnhance.h
*
*	@brief		Header File for Text Enhancer Class
*
*	@version	1.1
*
*	@date		1/05/2017
*
*	@author		George Edward Hugh Othen
*
*
********************************************************************************
*/

#pragma once

// -- Includes --
#include "opencv2/imgproc/imgproc.hpp"

class TextEnhance
{
public:
	//--------------------------------------------------------------------------
	/// Default constructor.
	/**
	*	@kernelSize: Initial size of Adaptive Kernel
	*	@imageSize: Size of the output image
	*/	
	//--------------------------------------------------------------------------
	TextEnhance(int kernelSize, std::pair<int, int> imageSize);

	/**
	*   Enhance the text of in the camera frame
	*	@cameraFrame: The current camera frame
	*/
	void EnhanceText(cv::UMat & cameraFrame);

	/**
	*   Increase the adaptive kernel size
	*/
	void increaseThreshold();

	/**
	*   Decrease the adaptive kernel size
	*/
	void decreaseThreshold();

	/**
	*   Toggle Closing to bolden Text
	*/
	void toggleClosing();

	/**
	*   Change the upscaling method used
	*/
	void changeResizeMode();	

	//--------------------------------------------------------------------------
	//// Destructor
	//--------------------------------------------------------------------------
	~TextEnhance();

	// The adaptive kernel size
	int adaptiveKernelSize;
private:
	/**
	*   @return: the resize mode to use
	*/
	int resizeMode();

	// Enable/Disable Closing
	bool bold;

	// Store the resize mode type and mode selection
	int resizeModeType, mode;

	// The size of the output image
	std::pair<int, int> sizeOfImage;
};

