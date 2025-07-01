#pragma once
#include "pch.h"
#include <iostream>
#include <stdexcept>
#include <string>

using std::string;

namespace utils {
	// Exception class for winapi errors
	class WinAPIErrorException : public std::exception {
	private:
		string message;
	public:
		WinAPIErrorException(const string msg);
		const char* what() const noexcept;
	};

	// Exception class for registry errors
	class RegistryErrorException : public std::exception {
	private:
		string message;
	public:
		RegistryErrorException(const string msg);
		const char* what() const noexcept;
	};

	// get the error string of last error
	std::string getErrString();

	// check GetLastError and if error accured throw runtime_error
	int checkError(int success, std::string what_failed);

	// check status and if error accured throw runtime_error
	LSTATUS checkStatus(LSTATUS status, std::string what_failed);

	// add current exe to autoruns using registry
	void addToAutoruns(void);
}