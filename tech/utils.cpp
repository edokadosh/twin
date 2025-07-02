#include "utils.h"
#include <iostream>

using std::cout;
using std::endl;
using std::wcout;


HANDLE singleInstanceMutex;
using exceptions::checkError;
using exceptions::checkStatus;

namespace utils {

	void addToAutoruns(void) {
		wchar_t exe_path[MAX_PATH] = { 0 };

		checkError(GetModuleFileNameW(NULL, exe_path, MAX_PATH), "GetModuleFileNameA");
		wcout << "Current exe path: " << exe_path << endl;

		HKEY hkey = NULL;
		checkStatus(RegCreateKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey), "RegCreateKey");
		checkStatus(RegSetValueExW(hkey, L"management program autorun", 0, REG_SZ, (BYTE*)exe_path, lstrlenW(exe_path) * 2 + 1), "RegSetValueEx");

		checkStatus(RegCloseKey(hkey), "RegCloseKey");
	}


}