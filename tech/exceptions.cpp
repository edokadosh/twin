#include "exceptions.h"

#include <iostream>
#include "pch.h"

using std::cout;
using std::endl;


namespace exceptions {
	void printError(int errorCode, string what_failed) {
		cout << what_failed << " - failed with error: " << errorCode << endl;
		cout << "Error string:" << getErrString(errorCode) << endl;
	}

	string getErrString(int errorCode) {
		if (errorCode == 0) {
			return string();
		}

		LPSTR messageBuffer = nullptr;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&messageBuffer), 0, NULL);

		if (size == 0) {
			throw WinAPIErrorException("FormatMessageA failed");
		}

		string message(messageBuffer, size);

		LocalFree(messageBuffer);

		return message;
	}

	int checkError(int success, string what_failed) {
		int error_code = GetLastError();
		if (error_code != ERROR_SUCCESS) {
			cout << "Returned value: " << success << endl;
			printError(error_code, what_failed);
			throw WinAPIErrorException(what_failed);
		}
		return success;
	}

	LSTATUS checkStatus(LSTATUS status, string what_failed) {
		int error_code = GetLastError();
		if (status != ERROR_SUCCESS) {
			printError(error_code, what_failed);
			throw RegistryErrorException(what_failed);
		}
		return status;
	}

	int checkWinSockError(int errorCode, const string& what_failed) {
		if (errorCode == SOCKET_ERROR) {
			printError(errorCode, what_failed);
			throw WinSockErrorException(what_failed);
		}
		return errorCode;
	}

	int checkFileError(int errorCode, const string& what_failed) {
		if (errorCode == FALSE) {
			printError(GetLastError(), what_failed);
			throw FileErrorException(what_failed);
		}
		return errorCode;
	}

}