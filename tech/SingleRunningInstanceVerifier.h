#pragma once
#include "pch.h"
#include <stdexcept>
#include <string>
#include "exceptions.h"

using std::wstring;
using std::runtime_error;

using exceptions::InstanceExistsException;

namespace single_running_instance_verifier {

	const wstring SINGLE_INSTANCE_MUTEX_NAME = L"app_single_instance_mutex";

	/**
	 * @brief Class to verify that only a single instance of the application is running
	 */
	class SingleRunningInstanceVerifier {
	public:
		/**
		 * @brief Constructor for SingleRunningInstanceVerifier
		 * @throws InstanceExistsException if another instance of the application is already running
		 * @throws WinAPIErrorException if there is an error creating the mutex
		 */
		SingleRunningInstanceVerifier();

		virtual ~SingleRunningInstanceVerifier();
		SingleRunningInstanceVerifier(const SingleRunningInstanceVerifier& other) = delete;
		SingleRunningInstanceVerifier(SingleRunningInstanceVerifier&& other) noexcept = delete;
		SingleRunningInstanceVerifier& operator=(
			const SingleRunningInstanceVerifier& other) = delete;
		SingleRunningInstanceVerifier& operator=(
			SingleRunningInstanceVerifier&& other) noexcept = delete;

	private:
		HANDLE m_singleInstanceMutex;
	};

}  // namespace single_running_instance_verifier
