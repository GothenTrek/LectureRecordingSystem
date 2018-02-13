/**
********************************************************************************
*
*	@file		Zoom.cpp
*
*	@brief		Zoom class
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

// -- Includes --
#include "Zoom.h"

#include <iostream>
#include <vector>

#include "opencv2/imgproc/imgproc.hpp"

//******************************************************************************
//    Zoom
//******************************************************************************
Zoom::Zoom(float zoomStepSize, std::pair<int, int> imageDimensions) :
	stepSize(zoomStepSize),
	sizeOfImage(imageDimensions),
	coordinates({ 0.0f,0.0f }),
	currentSizeOfImage({(float)sizeOfImage.first, (float)sizeOfImage.second})
{
}

void Zoom::ZoomIn()
{
	// If Cropped by coordinates, reset first
	if (coordinateCrop) {
		ResetCoords();
	}

	cropped = true;
	// If crop size greater than quarter original
	if (coordinates.first < ((currentSizeOfImage.first / 0.25f) - 1) & coordinates.second < ((currentSizeOfImage.second / 0.25f) - 1)) {

		// keep centralising crop rectangle
		coordinates.first += sizeOfImage.first* (stepSize / 2.0f);
		coordinates.second += sizeOfImage.second * (stepSize / 2.0f);

		// resize crop rectangle parameters
		currentSizeOfImage.first -= sizeOfImage.first * stepSize;
		currentSizeOfImage.second -= sizeOfImage.second * stepSize;
	}
	else {
#ifdef DEBUG
		std::cout << ("\nAlready Zoomed in Max") << std::endl;
#endif
	}

	coordinateCrop = false;
}

void Zoom::ZoomOut()
{
	// if Cropped by coordinates, reset first
	if (coordinateCrop) {
		ResetCoords();
	}

	// image has been cropped
	cropped = true;

	// keep centralising crop rectangle
	coordinates.first -= sizeOfImage.first * (stepSize / 2.0f);
	coordinates.second -= sizeOfImage.second * (stepSize / 2.0f);

	// resize crop rectangle parameters
	currentSizeOfImage.first += sizeOfImage.first * stepSize;
	currentSizeOfImage.second += sizeOfImage.second * stepSize;

	// If crop size is less than original
	if (coordinates.first < 0 | coordinates.second < 0 || currentSizeOfImage.first >= sizeOfImage.first | currentSizeOfImage.second >= sizeOfImage.second) {
		ResetCoords();		
	}

#ifdef DEBUG
	std::cout << "current X: " << currentSizeOfImage.first << " | current Y: " << currentSizeOfImage.second << std::endl;
#endif
	
	coordinateCrop = false;
}

void Zoom::AddCoords(std::pair<int, int> zoomCoordinates1, std::pair<int, int> zoomCoordinates2)
{
	// Requested Crop Offsets
	int MoveXCoordinate = 0, MoveYCoordinate = 0;

	// Calculate Requested Crop WIDTH & HEIGHT
	int requestedSizeX = abs(zoomCoordinates1.first - zoomCoordinates2.first);
	int requestedSizeY = abs(zoomCoordinates1.second - zoomCoordinates2.second);

#ifdef DEBUG
	std::cout << requestedSizeX << "|" << requestedSizeY << std::endl;
#endif

	// If requested crop size is greater than 0
	if (!(requestedSizeX == 0 | requestedSizeY == 0)) {

		// Make Crop Size = Aspect Ratio
		// This ensures that main object of crop is kept in main view
		// Compare ratio sizes, if absolute requested size X is greater than absolute requested size Y
		if (((float)sizeOfImage.second / (float) sizeOfImage.first) * requestedSizeX > ((float)sizeOfImage.first / (float)sizeOfImage.second) *  requestedSizeY) {
#ifdef DEBUG
			std::cout << "X is bigger than Y" << std::endl;
#endif

			// Height of crop is frame size's aspect ratio of Width
			currentSizeOfImage.second = (float)sizeOfImage.second / (float)sizeOfImage.first * requestedSizeX;
			currentSizeOfImage.first = requestedSizeX; // Keep Requested Width

			// Offset Crop so requested crop is centered.
			MoveYCoordinate = (currentSizeOfImage.second / 2.0f) - (requestedSizeY / 2.0f);
		}
		// Compare ratio sizes, if absolute requested size Y is greater than absolute requested size X
		else if (((float)sizeOfImage.first / (float)sizeOfImage.second) * requestedSizeY > ((float)sizeOfImage.second / (float)sizeOfImage.first) * requestedSizeX) {
#ifdef DEBUG
			std::cout << "Y is bigger than X" << std::endl;
#endif

			// Width of crop is frame size's aspect ratio of Height
			currentSizeOfImage.first = (float)sizeOfImage.first / (float)sizeOfImage.second * requestedSizeY;
			currentSizeOfImage.second = requestedSizeY; // Keep Requested Height

			// Offset Crop so requested crop is centered.
			MoveXCoordinate = (currentSizeOfImage.first / 2.0f) - (requestedSizeX / 2.0f);
		}

		// Calculate Crop X coordinate
		if (zoomCoordinates1.first < zoomCoordinates2.first) {
			coordinates.first = zoomCoordinates1.first;
			originalCoordinates.first = zoomCoordinates1.first;
		}
		else {
			coordinates.first = zoomCoordinates2.first;
			originalCoordinates.first = zoomCoordinates2.first;
		}

		// Calculate Crop Y coordinate
		if (zoomCoordinates1.second < zoomCoordinates2.second) {
			coordinates.second = zoomCoordinates1.second;
			originalCoordinates.second = zoomCoordinates1.second;
		}
		else {
			coordinates.second = zoomCoordinates2.second;
			originalCoordinates.second = zoomCoordinates2.second;
		}

		// Offset Coordinates of requested crop to be main subject of actual crop
		coordinates.first -= MoveXCoordinate;
		coordinates.second -= MoveYCoordinate;

		// Ensure Image is still within bounds of frame
		if (coordinates.first < 0) {
			coordinates.first = 0;
		}
		if ((coordinates.first + currentSizeOfImage.first) > sizeOfImage.first) {
			coordinates.first = (sizeOfImage.first - currentSizeOfImage.first);
		}
		if (coordinates.second < 0) {
			coordinates.second = 0;
		}
		if ((coordinates.second + currentSizeOfImage.second) > sizeOfImage.second) {
			coordinates.second = (sizeOfImage.second - currentSizeOfImage.second);
		}

		// Define Comparison Image Crop
		comparisonCrop = cv::Rect((int)originalCoordinates.first, (int)originalCoordinates.second, (int)requestedSizeX, (int)requestedSizeY);

		// Coordinate crop has been used
		coordinateCrop = true;

		// Image has been cropped
		cropped = true;
	}
}

// Reset to stock
void Zoom::ResetCoords()
{
	coordinates.first = 0.0f;
	coordinates.second = 0.0f;
	currentSizeOfImage.first = 1280;
	currentSizeOfImage.second = 720;
	cropped = false;
	coordinateCrop = false;
}

void Zoom::ZoomOnCoords(cv::UMat & cameraFrame) {
	if (cropped) {
		// Create Crop Rectangle
		imageCrop = cv::Rect((int)coordinates.first, (int)coordinates.second, (int)currentSizeOfImage.first, (int)currentSizeOfImage.second);

		// Crop Image
		cameraFrame = cameraFrame(imageCrop);
	}
}

Zoom::~Zoom()
{
}
