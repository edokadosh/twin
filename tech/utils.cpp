#include "utils.h"

using std::cout;
using std::endl;

HANDLE mutex;


namespace utils {
	int checkError(int success, std::string what_failed) {
		int error_code = ::GetLastError();
		if (error_code != ERROR_SUCCESS) {
			cout << what_failed << " - failed with error: " << error_code << endl;
			cout << "Returned value: " << success << endl;
			cout << "Error string:" << getErrString() << endl;
			throw std::runtime_error("GetModuleFileNameA");
		}
		return success;
	}

	void verifySingleProgramInstance() {
		mutex = CreateMutexA(NULL, false, SINGLE_INSTANCE_MUTEX_NAME);
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			printf("Instance of program is already running!");
			exit(1);
		}
	}

	std::string getErrString() {
		DWORD errorMessageID = ::GetLastError();
		if (errorMessageID == 0) {
			return std::string();
		}

		LPSTR messageBuffer = nullptr;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		std::string message(messageBuffer, size);

		LocalFree(messageBuffer);

		return message;
	}

	void addToAutoruns(void) {
		char exe_path[MAX_PATH] = { 0 };

		checkError(GetModuleFileNameA(NULL, exe_path, MAX_PATH), "GetModuleFileNameA");

		HKEY hkey = NULL;
		checkError(RegCreateKey(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey), "RegCreateKey");
		checkError(RegSetValueEx(hkey, L"myapp", 0, REG_SZ, (BYTE*)exe_path, ((DWORD)strlen(exe_path) + 1) * 2), "RegSetValueEx");
	}

}