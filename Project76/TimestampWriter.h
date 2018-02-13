/**
********************************************************************************
*
*	@file		TimestampWriter.h
*
*	@brief		Header File for Time Stamp Writer
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

#pragma once

// -- Includes --
#include <string>
class TimeStampWriter
{
public:
	//--------------------------------------------------------------------------
	/// Default constructor.
	/**
	*	@fileName: The Filename for the timestamp file
	*/
	//--------------------------------------------------------------------------
	TimeStampWriter(std::string filename);

	/**
	*	Writes the time stamp to file
	*	@time: The time in seconds to write to file
	*/
	void WriteTimeStamp(float time);

	/**
	*	Calculates the time as DD:HH:MM:SS
	*	@time: The time in seconds to write to file
	*/
	std::string timeFrame(float time);

	/**
	*	Check if the file already exists
	*/
	bool fileExists();

	//------------------------------------------------------------------------
	//// Destructor
	//------------------------------------------------------------------------
	~TimeStampWriter();
private:

	// Store the fileName
	std::string fileName;

	// Count the number of zooms
	int counter;

	// Check the filename once
	bool fileMade;
};

