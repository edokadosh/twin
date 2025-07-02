#include "Server.h"
#include "SingleRunningInstanceVerifier.h"
#include "pch.h"
#include "utils.h"

using std::cerr;
using std::endl;

using server::Server;
using single_running_instance_verifier::SingleRunningInstanceVerifier;
using utils::addToAutoruns;
using utils::checkError;
using single_running_instance_verifier::SingleRunningInstanceVerifier;

const DWORD HOUR_MILLISEC = 60 * 60 * 1000;

int main(void) {
	try {
		SingleRunningInstanceVerifier verify;

        //addToAutoruns();

        checkError(MessageBox(NULL, TEXT("MANAGEMENT PROGRAM IS UP"), TEXT("MANAGEMENT PROGRAM"), MB_OK), "MessageBox");

		Server s;
		s.start();
	}
	catch (const std::exception& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}

	return 0;
}
