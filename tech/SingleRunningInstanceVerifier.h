#pragma once
#include "pch.h"
#include <stdexcept>
#include <string>

using std::string;
using std::runtime_error;

namespace single_running_instance_verifier {

	/**
	 * @brief Exception class for when an instance of the program already exists
	 */
	class InstanceExistsException : public runtime_error {
	public:
		InstanceExistsException(const string& msg) : runtime_error(msg) {}
	};


	const LPCWSTR SINGLE_INSTANCE_MUTEX_NAME = L"app_single_instance_mutex";


	/**
	 * @brief Class to verify that only a single instance of the application is running
	 */
	class SingleRunningInstanceVerifier {
	private:
		HANDLE m_singleInstanceMutex;

	public:
		/**
		 * @brief Constructor for SingleRunningInstanceVerifier
		 * @throws InstanceExistsException if another instance of the application is already running
		 * @throws WinAPIErrorException if there is an error creating the mutex
		 */
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
