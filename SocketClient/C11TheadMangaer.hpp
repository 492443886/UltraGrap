#pragma once
/*
File C11UltraGrep.hpp
Author: Chunhui Ma
Purpose: A class that will be used When USE NO-WIN32 PLATFORM
*/

#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <filesystem>
#include <regex>
#include <map>
#include <fstream>  
#include "Command.hpp"


class C11ThreadManager {

	typedef std::shared_ptr<Command> commandPtr;

	std::vector<std::thread> threads;

	unsigned nThreads = 4;
	bool morePossibleWork = true;
	std::mutex consoleMtx;
	std::mutex resultPushMtx;

	// wake
	std::mutex wakeMtx;
	std::condition_variable wakeCond;

	// queue of tasks to perform
	std::queue<commandPtr> tasks;
	std::mutex taskMtx;

	// barrier
	unsigned barrierThreshold = nThreads + 1;
	unsigned barrierCount = barrierThreshold;
	unsigned barrierGeneration = 0;
	std::mutex barrierMtx;
	std::condition_variable barrierCond;

public:

	void setup();
	void finish();
	void addTaskHelper(commandPtr command);

private:
	void perform_task();
	void barrier();
};