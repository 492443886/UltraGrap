#pragma once
/*
File Win32ThreadManager.hpp
Author: Chunhui Ma
Purpose: A class that will be used When USE WIN32 PLATFORM
*/
#pragma once
#include <windows.h>
#include "CriticalSection.hpp"
#include "Command.hpp"
#include <string>
#include <iostream>
#include <queue>

class Win32ThreadManager {

	typedef std::shared_ptr<Command> commandPtr;
public:

	void setup();
	void finish();
	void addTaskHelper(commandPtr command);

};