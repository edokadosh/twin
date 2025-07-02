#pragma once

#include "pch.h"
#include <string>
#include <stdexcept>

using std::string;
using std::runtime_error;

namespace exceptions {
	/**
	 * @brief Exception class for WinAPI errors
	 */
	class WinAPIErrorException : public runtime_error {
	public:
		WinAPIErrorException(const string& msg) : runtime_error(msg) {}
	};

	/**
	 * @brief Exception class for registry errors
	 */
	class RegistryErrorException : public runtime_error {
	public:
		RegistryErrorException(const string& msg) : runtime_error(msg) {}
	};

	/**
	 * @brief Exception class for when an instance of the program already exists
	 */
	class InstanceExistsException : public runtime_error {
	public:
		InstanceExistsException(const string& msg) : runtime_error(msg) {}
	};

	/**
	 * @brief get error string from error code
	 * @param errorCode The error code to retrieve the string for
	 * @return A string containing the error message corresponding to the error code
	 */
	string getErrString(int errorCode);

	/**
	 * @brief Print error message to console
	 * @param errorCode The error code to print
	 * @param what_failed A string description of the operation that failed
	 */
	void printError(int errorCode, string what_failed);

	/**
	 * @brief Check if a WinAPI function call was successful
	 * @param success The return value of the WinAPI function
	 * @param what_failed A string description of the operation that failed
	 * @return The success value if the call was successful
	 * @throws WinAPIErrorException if the call failed
	 */
	int checkError(int success, string what_failed);

	/**
	 * @brief Check if a registry operation was successful
	 * @param status The return value of the registry operation
	 * @param what_failed A string description of the operation that failed
	 * @return The status value if the operation was successful
	 * @throws RegistryErrorException if the operation failed
	 */
	LSTATUS checkStatus(LSTATUS status, string what_failed);

} // namespace exceptions