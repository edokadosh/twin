#pragma once
#include <iostream>
#include "pch.h"
#include <string>
#include <stdexcept>


namespace utils {
	const int NONZERO = 1;

	std::string getErrString();

	int checkError(int success, std::string what_failed);

	void addToAutoruns(void);

	void clean();

}