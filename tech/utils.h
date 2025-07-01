#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <stdexcept>


namespace utils {
	const LPCSTR SINGLE_INSTANCE_MUTEX_NAME = "app_single_instance_mutex";
	const int NONZERO = 1;

	void verifySingleProgramInstance();

	std::string getErrString();

	int checkError(int success, std::string what_failed);

	void addToAutoruns(void);

}