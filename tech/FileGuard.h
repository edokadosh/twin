#pragma once
#include "pch.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace file_guard {

	const size_t BUFFER_SIZE = 1024;

	class FileGuard {
	public:
		/**
		 * @brief Constructor for FileGuard
		 * @param name The name of the file to open
		 * @param access The access mode (GENERIC_READ or GENERIC_WRITE)
		 * @throws FileErrorException if the file cannot be opened
		 */
		FileGuard(string name, int access);
		~FileGuard();
		FileGuard(const FileGuard& other) = delete;
		FileGuard(FileGuard&& other) noexcept = delete;
		FileGuard& operator=(const FileGuard& other) = delete;
		FileGuard& operator=(FileGuard&& other) noexcept = delete;

		vector<char> read();
		void write(const vector<char>& data);
	private:
		HANDLE m_file;
	};

} //namespace file_raii

