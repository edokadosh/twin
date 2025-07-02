#pragma once
#include "pch.h"
#include "exceptions.h"


using exceptions::WinAPIErrorException;
using exceptions::RegistryErrorException;


namespace utils {

	/**
	 * @brief Add the current executable to the Windows autorun registry key
	 * This function retrieves the path of the current executable and adds it to autorun registry key
	 */
	void addToAutoruns(void);
}