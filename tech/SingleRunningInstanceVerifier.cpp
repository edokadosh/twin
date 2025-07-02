#include "SingleRunningInstanceVerifier.h"

#include <utility>

#include "utils.h"

using std::exchange;
using std::swap;
using std::cout;
using std::endl;

using utils::printError;
using utils::WinAPIErrorException;

namespace single_running_instance_verifier {

	SingleRunningInstanceVerifier::SingleRunningInstanceVerifier() : m_singleInstanceMutex(CreateMutexW(NULL, false, SINGLE_INSTANCE_MUTEX_NAME)) {
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

	SingleRunningInstanceVerifier::SingleRunningInstanceVerifier(
		const SingleRunningInstanceVerifier& other) {
		HANDLE thisProc = GetCurrentProcess();
		HANDLE newHandle;

		DuplicateHandle(
			thisProc,
			other.m_singleInstanceMutex,
			NULL,
			&newHandle,
			NULL,
			NULL,
			DUPLICATE_SAME_ACCESS
		);

		m_singleInstanceMutex = newHandle;
	}

	SingleRunningInstanceVerifier::SingleRunningInstanceVerifier(
		SingleRunningInstanceVerifier&& other) noexcept
		: m_singleInstanceMutex(
			exchange(other.m_singleInstanceMutex, INVALID_HANDLE_VALUE)) {
		// Left blank intentionally
	}

	SingleRunningInstanceVerifier& SingleRunningInstanceVerifier::operator=(
		const SingleRunningInstanceVerifier& other) {
		return *this = SingleRunningInstanceVerifier(other);
	}

	SingleRunningInstanceVerifier& SingleRunningInstanceVerifier::operator=(
		SingleRunningInstanceVerifier&& other) noexcept {
		std::swap(m_singleInstanceMutex, other.m_singleInstanceMutex);
		return *this;
	}

}  // namespace single_running_instance_verifier
