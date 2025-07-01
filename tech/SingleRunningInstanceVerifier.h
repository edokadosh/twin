#pragma once
#include "pch.h"

namespace single_running_instance_verifier {

	const LPCWSTR SINGLE_INSTANCE_MUTEX_NAME = L"app_single_instance_mutex";

	// class for verifiying that only one process is running this program.
	// only one process can create an instance of this class at a time.
	class SingleRunningInstanceVerifier {
	private:
		HANDLE m_singleInstanceMutex;

	public:
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
