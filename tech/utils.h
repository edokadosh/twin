#pragma once
#include "pch.h"
#include <iostream>
#include <stdexcept>
#include <string>

namespace utils {
const int NONZERO = 1;

std::string getErrString();

int checkError(int success, std::string what_failed);
LSTATUS checkStatus(LSTATUS status, std::string what_failed);

void addToAutoruns(void);
}