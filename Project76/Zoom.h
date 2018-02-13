/**
********************************************************************************
*
*	@file		Zoom.h
*
*	@brief		Header for Zoom Class
*
*	@version	1.1
*
*	@date		24/4/2017
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
class Zoom
{
public:
	//--------------------------------------------------------------------------
	/// Default constructor.
	//--------------------------------------------------------------------------
	Zoom(float zoomStepSize, std::pair<int, int> imageDimensions);

	//------------------------------------------------------------------------
	/**
	* Calculate the coordinates to zoom in a step
	*/
	//------------------------------------------------------------------------
	void ZoomIn();

	//------------------------------------------------------------------------
	/**
	* Calculate the coordinates to zoom out a step
	*/
	//------------------------------------------------------------------------
	void ZoomOut();

	//------------------------------------------------------------------------
	/**
	* Add board coordinates to zoom class
	* @param zoomCoordinates1: the first corner of the board
	* @param zoomCoordinates2: the second corner of the board
	*/
	//------------------------------------------------------------------------
	void AddCoords(std::pair<int, int> zoomCoordinates1, std::pair<int, int> zoomCoordinates2);

	//------------------------------------------------------------------------
	/**
	* Reset the coordinates to 0 for default zoom
	*/
	//------------------------------------------------------------------------
	void ResetCoords();

	//------------------------------------------------------------------------
	/**
	* @param cameraFrame: The current camera frame to crop
	*/
	//------------------------------------------------------------------------
	void ZoomOnCoords(cv::UMat & cameraFrame);

	//------------------------------------------------------------------------
	//// Destructor
	//------------------------------------------------------------------------
	~Zoom();

	// Crop for Movement Detector
	cv::Rect comparisonCrop;	

	// State of zoom
	bool cropped;
private:
	// Crop for Output image
	cv::Rect imageCrop;

	// New Coordinates Calculated from requested crop
	std::pair<int, int> coordinates;

	// Original Coordinates Requested
	std::pair<int, int> originalCoordinates;

	// Size of the output image
	std::pair<int, int> sizeOfImage;

	// Size of the current crop
	std::pair<float, float> currentSizeOfImage;	

	// Manual or Automatic Zoom?
	bool coordinateCrop;

	// Manual Zoom Step Size
	float stepSize;
};

