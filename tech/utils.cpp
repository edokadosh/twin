#include "utils.h"

using std::cout;
using std::endl;
using std::wcout;

HANDLE singleInstanceMutex;

namespace utils {

	void printError(int errorCode, string what_failed) {
		cout << what_failed << " - failed with error: " << errorCode << endl;
		cout << "Error string:" << getErrString(errorCode) << endl;
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

	string getErrString(int errorCode) {
		if (errorCode == 0) {
			return string();
		}

    LPSTR messageBuffer = nullptr;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		if (size == 0) {
			throw WinAPIErrorException("FormatMessageA failed");
		}

		string message(messageBuffer, size);

    LocalFree(messageBuffer);

    return message;
}

	void addToAutoruns(void) {
		wchar_t exe_path[MAX_PATH] = { 0 };

		checkError(GetModuleFileNameW(NULL, exe_path, MAX_PATH), "GetModuleFileNameA");
		wcout << "Current exe path: " << exe_path << endl;

		HKEY hkey = NULL;
		checkStatus(RegCreateKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey), "RegCreateKey");
		checkStatus(RegSetValueExW(hkey, L"management program autorun", 0, REG_SZ, (BYTE*)exe_path, lstrlenW(exe_path) * 2 + 1), "RegSetValueEx");

    checkStatus(RegCloseKey(hkey), "RegCloseKey");
}

} // namespace utils