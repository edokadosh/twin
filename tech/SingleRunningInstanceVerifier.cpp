#include "SingleRunningInstanceVerifier.h"

#include <iostream>
#include <utility>

#include "utils.h"

using std::exchange;
using std::swap;
using std::cout;
using std::endl;

using exceptions::printError;
using exceptions::WinAPIErrorException;

namespace single_running_instance_verifier {

	SingleRunningInstanceVerifier::SingleRunningInstanceVerifier() : m_singleInstanceMutex(CreateMutexW(NULL, false, SINGLE_INSTANCE_MUTEX_NAME.c_str())) {
		int errorCode = ::GetLastError();
		if (errorCode == ERROR_ALREADY_EXISTS) {
			cout << "Instance of program is already running!" << endl;
			throw InstanceExistsException("Instance of program is already running!");
		}
		if (errorCode != ERROR_SUCCESS) {
			printError(errorCode, "CreateMutexW");
			throw WinAPIErrorException("CreateMutexW failed");
		}
	}

	SingleRunningInstanceVerifier::~SingleRunningInstanceVerifier() {
		CloseHandle(m_singleInstanceMutex);
	}

}  // namespace single_running_instance_verifier
