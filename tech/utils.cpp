#include "utils.h"

using std::cout;
using std::endl;
using std::wcout;

HANDLE singleInstanceMutex;

namespace utils {
int checkError(int success, std::string what_failed)
{
    int error_code = ::GetLastError();
    if (error_code != ERROR_SUCCESS) {
        cout << what_failed << " - failed with error: " << error_code << endl;
        cout << "Returned value: " << success << endl;
        cout << "Error string:" << getErrString() << endl;
        throw std::runtime_error(what_failed);
    }
    return success;
}

std::string getErrString()
{
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

void addToAutoruns(void)
{
    wchar_t exe_path[MAX_PATH] = { 0 };

    checkError(GetModuleFileNameW(NULL, exe_path, MAX_PATH), "GetModuleFileNameA");
    wcout << "Current exe path: " << exe_path << endl;

    HKEY hkey = NULL;
    checkError(RegCreateKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey), "RegCreateKey");
    checkError(RegSetValueExW(hkey, L"management program autorun", 0, REG_SZ, (BYTE*)exe_path, lstrlenW(exe_path) * 2 + 1), "RegSetValueEx");

    checkError(RegCloseKey(hkey), "RegCloseKey");
}

void clean()
{
    CloseHandle(singleInstanceMutex);
}

}