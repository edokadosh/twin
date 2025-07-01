#pragma once
#include "pch.h"
#include <stdexcept>
#include <string>

using std::string;

namespace single_running_instance_verifier {

	// Exception class for when an instacne already exists
	class InstanceExistsException : public std::exception {
	private:
		string message;
	public:
		InstanceExistsException(const string msg);
		const char* what() const noexcept;
	};


	const LPCWSTR SINGLE_INSTANCE_MUTEX_NAME = L"app_single_instance_mutex";

	// class for verifiying that only one process is running this program.
	// only one process can create an instance of this class at a time.
	class SingleRunningInstanceVerifier {
	private:
		HANDLE m_singleInstanceMutex;

	public:
		// if there is a process with a member of this class this will throw 
		SingleRunningInstanceVerifier();

		virtual ~SingleRunningInstanceVerifier();
		SingleRunningInstanceVerifier(const SingleRunningInstanceVerifier& other);
		SingleRunningInstanceVerifier(SingleRunningInstanceVerifier&& other) noexcept;
		SingleRunningInstanceVerifier& operator=(
			const SingleRunningInstanceVerifier& other);
		SingleRunningInstanceVerifier& operator=(
			SingleRunningInstanceVerifier&& other) noexcept;
	};

}  // namespace single_running_instance_verifier
