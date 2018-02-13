/**
********************************************************************************
*
*	@file		TextEnhance.cpp
*
*	@brief		Text Enhancement Class
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

// -- Includes --
#include "TextEnhance.h"

#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"



TextEnhance::TextEnhance(int kernelSize, std::pair<int, int> imageSize) :
	adaptiveKernelSize(kernelSize),
	sizeOfImage(imageSize),
	bold(false),
	mode(0)
{
}

void TextEnhance::EnhanceText(cv::UMat & cameraFrame)
{
	// Convert to GreyScale
	cv::cvtColor(cameraFrame, cameraFrame, CV_RGB2GRAY);

	// Adaptively Threshold Image
	cv::adaptiveThreshold(cameraFrame, cameraFrame, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, adaptiveKernelSize, 0);

	// Closing to fill in gaps
	cv::erode(cameraFrame, cameraFrame, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1)));
	cv::dilate(cameraFrame, cameraFrame, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1)));

	// Boldens text for stronger text
	if (bold) {
		cv::erode(cameraFrame, cameraFrame, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1)));
	}

	int resize = resizeMode();
	// Resize Image
	cv::resize(cameraFrame, cameraFrame, cv::Size(sizeOfImage.first, sizeOfImage.second), 0, 0, resize);
}

void TextEnhance::increaseThreshold()
{
	if (adaptiveKernelSize < 301)
		adaptiveKernelSize += 2;
	else {
#ifdef DEBUG
		std::cout << "Adaptive Threshold Kernel Size at Max (299)" << std::endl;
#endif
	}
}


void TextEnhance::decreaseThreshold()
{
	if (adaptiveKernelSize > 3)
		adaptiveKernelSize -= 2;
	else {
#ifdef DEBUG
		std::cout << "Adaptive Threshold Kernel Size at Min (3)" << std::endl;
#endif
	}
}

void TextEnhance::toggleClosing()
{
	bold = !bold;
}

// Change upscaling method used on resize
void TextEnhance::changeResizeMode()
{
	if (mode >= 1) {
		mode = 0;
	}
	else {
		mode++;
	}
}

// Choose whether to use interpolation methods on resize
int TextEnhance::resizeMode()
{
	switch (mode) {
	case 0:
		resizeModeType = cv::INTER_NEAREST;
		break;
	case 1:
		resizeModeType = cv::INTER_LINEAR;
		break;
	default:
		resizeModeType = cv::INTER_NEAREST;
	}
	return resizeModeType;
}

TextEnhance::~TextEnhance()
{
}
