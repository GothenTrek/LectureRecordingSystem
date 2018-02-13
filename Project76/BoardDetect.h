#pragma once

#include "opencv2/imgproc/imgproc.hpp"

class BoardDetect
{
public:
	BoardDetect(int tolerance);
	void DetectBoard(cv::UMat & cameraFrame, std::pair<int, int> coordinates);
	std::pair<int, int> getCoordinates1();
	std::pair<int, int> getCoordinates2();
	~BoardDetect();
private:
	cv::UMat TemporarilyStoredFrame;
	std::pair<int, int> coordinates1, coordinates2;
	int tolerance;
};

