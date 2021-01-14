#pragma once
/*
File Command.hpp
Author: Chunhui Ma
Purpose: command class is user to run;
*/
#include "FileManager.hpp"
#include <iostream>
#include"CriticalSection.hpp"


class Command {
public:
	CriticalSection * cs;
	std::mutex* consoleMutex;
#ifdef _WIN32
#define LOCK CSLock lk(*cs);;
#else
#define LOCK lock_guard<mutex> lk(*consoleMutex);
#endif
	
public:
	inline virtual void execute() {};
};

class GrepCommand: public Command {

protected:
	FileMagnager* fileManager;
	
public:
	std::string p;
public:
	virtual void execute() override;
	inline void setFileMagnager(FileMagnager* f) {
		fileManager = f;
	}

};




