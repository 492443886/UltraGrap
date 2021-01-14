#pragma once
/*
File SocketClient.hpp
Author: Chunhui Ma
Purpose: command class is user to run;
*/
#include <iostream>
#include <string>
#include <mutex>
#include "CriticalSection.hpp"

class Command {

public:
	CriticalSection * cs;
	std::mutex* consoleMutex;
public:
	inline virtual void execute() {};
};

class PrintCommand : public Command {
	std::string message;
public:
	inline PrintCommand(std::string x) {
		message = x;
	}
	inline virtual void execute() {
		{
#ifdef _WIN32
			CSLock CS(*cs);
#else
			lock_guard<mutex> lk((*consoleMutex));
#endif
			std::cout << message << std::endl;
		}
	};
};

