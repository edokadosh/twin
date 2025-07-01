#include <Windows.h>
#include "utils.h"

using utils::verifySingleProgramInstance;
using utils::addToAutoruns;
using utils::checkError;
using utils::NONZERO;

const DWORD HOUR_MILLISEC = 3600000;


int main(void) {
	verifySingleProgramInstance();

	addToAutoruns();

	checkError(MessageBox(NULL, TEXT("MANAGEMENT PROGRAM IS UP"), TEXT("MANAGEMENT PROGRAM"), MB_OK), "MessageBox");

	Sleep(HOUR_MILLISEC);

	return 0;
}