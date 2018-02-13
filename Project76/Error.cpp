/**
********************************************************************************
*
*	@file		Error.cpp
*
*	@brief		Error class
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
#include "Error.h"

#include <iostream>

void error(std::string error) {
	std::cerr << error << std::endl;
	std::cout << "Press return to exit" << std::endl;
	exit(0);
}
