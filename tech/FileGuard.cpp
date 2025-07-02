#include "FileGuard.h"
#include <iostream>
#include "exceptions.h"

using std::cerr;
using std::endl;

using exceptions::printError;
using exceptions::checkError;
using exceptions::FileErrorException;
using exceptions::checkFileError;

namespace file_guard {

	FileGuard::FileGuard(string name, int access) {
		switch (access) {
		case GENERIC_READ:
			m_file = CreateFileA(
				name.c_str(),
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
			break;
		case GENERIC_WRITE:
			m_file = CreateFileA(
				name.c_str(),
				GENERIC_WRITE,
				0,
				NULL,
				CREATE_NEW,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
			break;
		default:
			throw FileErrorException("Unsupported Access");
		}
		
		if (m_file == INVALID_HANDLE_VALUE) {
			printError(GetLastError(), "CreateFileA");
			throw FileErrorException("CreateFileA");
		}
	}

	FileGuard::~FileGuard() {
		if (m_file != INVALID_HANDLE_VALUE) {
			try {
				checkError(CloseHandle(m_file), "CloseHandle");
			}
			catch (const std::runtime_error& e) {
				cerr << "FileGuard destructor failed: " << e.what() << endl;
			}

		}
	}

	vector<char> FileGuard::read() {
		vector<char> result;
		vector<char> buffer(BUFFER_SIZE);
		DWORD bytesRead = 0;

		do {
			checkFileError(ReadFile(m_file, buffer.data(), BUFFER_SIZE, &bytesRead, NULL), "ReadFile");
			if (bytesRead > 0) {
				result.insert(result.end(), buffer.begin(), buffer.begin() + bytesRead);
			}
		} while (bytesRead > 0);

		result.shrink_to_fit();
		return result;
	}


	void FileGuard::write(const vector<char>& data) {
		DWORD bytesWritten = 0;
		size_t totalBytesWritten = 0;

		while (totalBytesWritten < data.size()) {
			size_t bytesToWrite = min(data.size() - totalBytesWritten, BUFFER_SIZE);
			checkFileError(WriteFile(m_file, data.data() + totalBytesWritten, bytesToWrite, &bytesWritten, NULL), "WriteFile");
			totalBytesWritten += bytesWritten;
		}
	}

} //namespace file_guard