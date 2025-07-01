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
using utils::clean;
using utils::NONZERO;

const DWORD HOUR_MILLISEC = 60 * 60 * 1000;

int main(void)
{
    try {
        SingleRunningInstanceVerifier verify;

        addToAutoruns();

        checkError(MessageBox(NULL, TEXT("MANAGEMENT PROGRAM IS UP"), TEXT("MANAGEMENT PROGRAM"), MB_OK), "MessageBox");

        // Sleep(HOUR_MILLISEC);
        Server s;
        s.start();
    } catch (const std::runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    } catch (...) {
        // Left blank intentionally
    }
    clean();

    return 0;
}