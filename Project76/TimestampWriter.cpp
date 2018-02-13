/**
********************************************************************************
*
*	@file		TimestampWriter.cpp
*
*	@brief		Time Stamp Writer Class
*
*	@version	1.0
*
*	@date		29/04/2017
*
*	@author		George Edward Hugh Othen
*
*
********************************************************************************
*/

// -- Includes --
#include "TimeStampWriter.h"
#include "Error.h"

#include <fstream>
#include <iostream>
#include <string>

//******************************************************************************
//    Time Stamp Writer
//******************************************************************************
TimeStampWriter::TimeStampWriter(std::string filename) :
	fileName(filename),
	fileMade(false),
	counter(0)
{
}

void TimeStampWriter::WriteTimeStamp(float time)
{
	// Create Filename
	std::string temp = fileName; int i = 1;

	// Check if filename has been made
	if (!fileMade) {
		// If file exists, increment
		while (fileExists()) {
			fileName = temp;
			fileName += std::to_string(i);
			i++;
		}
		fileMade = true;
	}
	// Create Output Stream
	std::ofstream timestamp_file(fileName, std::ios::app | std::ios::out);
	
	// Check if file is created
	if (!timestamp_file.is_open()) {
		std::string errorMessage = "Cannot create timestamp file";
		error(errorMessage);
	}
	else {		
		counter++;
		timestamp_file << "Zoom " << counter << " ; " << timeFrame(time) << std::endl;
	}
}

std::string TimeStampWriter::timeFrame(float time)
{
	// Declare Variables
	int days = 0, hours = 0, minutes = 0, seconds = 0;

	// Create string timestamp
	std::string timestamp;

	// Check length of time
	if (time > 86400) {
		days = (int) time / 86400;
		hours = (int) (((int)time % 86400) / 3600);
		minutes = (int) (((int)time % 3600) / 60);
		seconds = (int)((int)time % 60);
	}
	else if (time > 3600) {
		hours = (int)(time / 3600);
		minutes = (int)(((int)time % 3600) / 60);
		seconds = (int)((int)time % 60);
	}
	else if (time > 60) {
		minutes = (int)time / 60;
		seconds = (int)((int)time % 60);
	}
	else {
		seconds = (int)time;
	}

	// Build string
	timestamp.append(std::to_string(days));
	timestamp.append(":");
	timestamp.append(std::to_string(hours));
	timestamp.append(":");
	timestamp.append(std::to_string(minutes));
	timestamp.append(":");
	timestamp.append(std::to_string(seconds));
	
	return timestamp;
}

bool TimeStampWriter::fileExists()
{
	// Check filename
	std::ifstream checkFile(fileName.append(".ts"));
	return checkFile.good();
}

TimeStampWriter::~TimeStampWriter()
{
}
