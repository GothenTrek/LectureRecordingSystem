/**
********************************************************************************
*
*	@file		LectureRecord.cpp
*
*	@brief		Lecture Recording System
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
#include "BoardDetect.h"
#include "Zoom.h"
#include "MovementDetect.h"
#include "TextEnhance.h"
#include "TimestampWriter.h"

#include <exception>
#include <iostream>

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <vector>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/ocl.hpp"

using namespace cv;
using namespace std;

// Frame Dimensions (WEBCAM RESOLUTION)
std::pair<int,int> FRAMEDIMENSIONS = { 1280, 720 };
cv::Size DIMENSIONS = cv::Size(FRAMEDIMENSIONS.first, FRAMEDIMENSIONS.second);

// Create Camera Object
Camera camera(FRAMEDIMENSIONS);

/// Global variables
UMat currentCameraFrame;

// Control Functionality
bool boardDetected, zoomingManual, textEnhanced, timeStampHasBeenWritten;

// Window Name
char* window_name = "A Lecture Recording System";

// Click Counter for Board Detection
int countVar(0);

// OpenCV WaitKey
int c;

// Keep track of time passed
float whenTimerStarted;

// Manual Zoom StepSize (between 0.02 - 0.2)
float zoomStepSize(0.04f);

// Create Zoom Object
Zoom zoomFunctionality(zoomStepSize, FRAMEDIMENSIONS);

// Check lecturer is writing after x seconds
int lecturerWritingTime(3);

// Compare Frames after x seconds
int timeBetweenCompares(0.25);

// Zoom into board for x seconds
int zoomCount(10);

// Wait x frames before considering lecturer as writing
int writeSensitivity(20);

// NCC considered a large change
float largeNCCChange = 0.980;

// NCC considered a small change
float smallNCCChange = 0.9987;

// Create Movement Detector Object
MovementDetect movementDetector(lecturerWritingTime, timeBetweenCompares, zoomCount, writeSensitivity, largeNCCChange, smallNCCChange);

// Intial Adaptive Threshold Kernel Size
int initialKernelSize(299);

// Create Text Enhancer
TextEnhance textEnhancer(initialKernelSize, FRAMEDIMENSIONS);

// Requested Zoom Crop Coordinates
std::pair<int, int> coordinates1, coordinates2;

// Video Location
std::string vidLocal = "../Video/LectureRecording.avi";

// Timestamp Location
std::string tsLocal = "../Video/Timestamp";

void mouseSelect(int event, int x, int y, int flags, void* userdata) {
	// Store Coordinates of Crop
	if (event == EVENT_LBUTTONDOWN & flags == (EVENT_FLAG_SHIFTKEY + EVENT_LBUTTONDOWN) & !zoomingManual) {
		countVar++;		
		if (countVar == 1) {
			coordinates1.first = x;
			coordinates1.second = y;
#ifdef DEBUG
			std::cout << "Coordinates 1 being saved" << std::endl;
#endif
		}
		if (countVar == 2) {
			coordinates2.first = x;
			coordinates2.second = y;
#ifdef DEBUG
			std::cout << "Coordinates 2 being saved" << std::endl;
#endif
			// Add coordinates of board
			zoomFunctionality.AddCoords(coordinates1, coordinates2);

			// Ensure initial frame is stored for comparison
			movementDetector.SaveFrame(currentCameraFrame, zoomFunctionality.comparisonCrop);

			// Allow board to be reselected
			countVar = 0;

			// Board is now detected
			boardDetected = true;
		}
	}
}

void setDevice(int device) {
	// Choose the OpenCL device
	cv::ocl::Context context;
	cv::ocl::Device(context.device(device)); // Change which GPU to use e.g. (0 or 1)
}

/**
* @function main
*/
int main(int argc, char** argv)
{
	try
	{		
		// Check for correct number of Arguments
		if (argc != 3) {
			std::string errorMessage = "Wrong number of Arguments";
			errorMessage += " Usage: Project outputVideoName.avi outputTimeStamps";
			throw errorMessage;
		}

		// Create Timestamp Writer
		TimeStampWriter timeStampWriter(argv[2]);

		// Enable OpenCL
		ocl::setUseOpenCL(true);
		setDevice(1); // Use OpenCL Device

		/// General instructions
#ifdef DEBUG
		std::cout << ("\n Computer Vision for Text Enhancement After Digital Zoom") << std::endl;
		std::cout << ("-------------------------------------------------------") << std::endl;
		std::cout << (" * SHIFT + CLICK x2 -> Select Area to Zoom\n") << std::endl;
		std::cout << (" * [u] -> Zoom in\n") << std::endl;
		std::cout << (" * [d] -> Zoom out\n") << std::endl;
		std::cout << (" * [r] -> Reset Zoom\n") << std::endl;	
		std::cout << (" * ['['] -> Reduce Threshold Kernel Size\n") << std::endl;
		std::cout << (" * [']'] -> Increase Threshold Kernel Size\n") << std::endl;
		std::cout << (" * [b] -> Toggle Bolden\n") << std::endl;
		std::cout << (" * [m] -> Change Resize Mode\n") << std::endl;
		std::cout << (" * [ESC] -> Close program\n") << std::endl;		
#endif DEBUG

		/// Create window, show OG image
		namedWindow(window_name, CV_WINDOW_AUTOSIZE);

		// Set Window Function
		setMouseCallback(window_name, mouseSelect, NULL);

		// Create Video Writer
		cv::VideoWriter video_writer;

		// Create Video File
		video_writer.open(argv[1], CV_FOURCC('M', 'J', 'P', 'G'), 15, DIMENSIONS, true);

		if (!video_writer.isOpened()) {
			std::string errorMessage = "Cannot Create Video";
			throw errorMessage;
		}

		while ((c = waitKey(1)) != 27)
		{
			// Get Frame from Camera
			if (!movementDetector.zoomed) {
				currentCameraFrame = camera.getFrame();
				cv::normalize(currentCameraFrame, currentCameraFrame, 0, 255, CV_MINMAX);
			}
			// Zoom in
			if ((char)c == 'u')
			{
				zoomingManual = true;
				zoomFunctionality.ZoomIn();
			}

			// Zoom Out
			if ((char)c == 'd')
			{
				zoomingManual = true;
				zoomFunctionality.ZoomOut();
			}

			// Reset Zoom
			if ((char)c == 'r') {
				zoomFunctionality.ResetCoords();
				boardDetected = false;
				zoomingManual = false;
			}

			// Increase Threshold Kernel Radius
			if ((char)c == ']') {
				textEnhancer.increaseThreshold();
			}

			// Decrease Threshold Kernel Radius
			if ((char)c == '[') {
				textEnhancer.decreaseThreshold();
			}

			// Toggle Closing - Boldens letters, reduces gaps
			if ((char)c == 'b') {
				textEnhancer.toggleClosing();
			}

			// Change Upscaling type
			if ((char)c == 'm') {
				textEnhancer.changeResizeMode();
			}

			// Check if Board is detected
			if (boardDetected) {
				// Disable Manual Zooming
				zoomingManual = false;

				// Detect movement of lecturer
				movementDetector.DetectMovement(currentCameraFrame, zoomFunctionality.comparisonCrop);
				// Increment Timer
				float currentTimer = (clock() / 1000.0f) - (whenTimerStarted / 1000.0f);

				// Check if zoomed
				if (movementDetector.zoomed) {
					if (!textEnhanced) {
						// Zoom onto board
						zoomFunctionality.ZoomOnCoords(currentCameraFrame);

						// Enhance board text
						textEnhancer.EnhanceText(currentCameraFrame);

						// Convert to unsigned char
						currentCameraFrame.convertTo(currentCameraFrame, CV_8UC3);

						// Convert Frame back to RGB
						cvtColor(currentCameraFrame, currentCameraFrame, CV_GRAY2BGR);
						textEnhanced = true;
					}
					// TimeStamp zoom event
					if (!timeStampHasBeenWritten) {
						timeStampWriter.WriteTimeStamp(currentTimer);
						timeStampHasBeenWritten = true;
					}
					
				}
				else {
					// Zoomed out, allow timestamp to be written again
					timeStampHasBeenWritten = false;
					textEnhanced = false;
				}
				// Stream current frame to videowriter
				video_writer << currentCameraFrame.getMat(0);				
			}
			else {
				// Replace start timer until board is detected (lecture has begun)
				whenTimerStarted = clock() / 1000.0f;
			}
			
			if (zoomingManual & !boardDetected) {
				// Zoom onto board using manual zoom
				zoomFunctionality.ZoomOnCoords(currentCameraFrame);
				textEnhancer.EnhanceText(currentCameraFrame);
			}

			// Display current frame (edited or not) in window
			imshow(window_name, currentCameraFrame);
		}
	}
	catch (std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}
	return 0;
}