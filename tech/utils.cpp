#include "utils.h"
#include <iostream>

#include "exceptions.h"
#include "RegKeyGuard.h"

using std::cout;
using std::endl;
using std::wcout;

using exceptions::checkError;
using regkey_Guard::RegKeyGuard;

namespace utils {

	void addToAutoruns(void) {
		wchar_t exe_path[MAX_PATH] = { 0 };

		checkError(GetModuleFileNameW(NULL, exe_path, MAX_PATH), "GetModuleFileNameA");
		wcout << "Current exe path: " << exe_path << endl;

		RegKeyGuard regKey(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
		HKEY hkey = NULL;

		regKey.setValue(L"management program autorun", REG_SZ, (BYTE*)exe_path, lstrlenW(exe_path) * 2 + 1);

	}

} // namespace utils