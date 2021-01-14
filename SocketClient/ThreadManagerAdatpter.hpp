#pragma once

/*
File SocketClient.hpp
Author: Chunhui Ma
Purpose: command class is user to run;
*/
#ifdef _WIN32

	#include "Win32ThreadManager.hpp"
	typedef Win32ThreadManager Parent;
#else
	#include "C11TheadMangaer.hpp"
	typedef C11ThreadManager Parent;
#endif
class ThreadManagerAdapter: public Parent
{
};