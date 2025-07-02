#pragma once
#include "pch.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace file_raii {

	const size_t BUFFER_SIZE = 1048;

	class FileRAII {
	public:
		/**
		 * @brief Constructor for FileRAII
		 * @param name The name of the file to open
		 * @param access The access mode (GENERIC_READ or GENERIC_WRITE)
		 * @throws FileErrorException if the file cannot be opened
		 */
		FileRAII(string name, int access);
		~FileRAII();
		FileRAII(const FileRAII& other) = delete;
		FileRAII(FileRAII&& other) noexcept = delete;
		FileRAII& operator=(const FileRAII& other) = delete;
		FileRAII& operator=(FileRAII&& other) noexcept = delete;

		vector<char> read();
		void write(const vector<char>& data);
	private:
		HANDLE m_file;
	};

} //namespace file_raii

