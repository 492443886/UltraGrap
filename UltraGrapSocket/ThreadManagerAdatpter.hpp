#pragma once
/*
File ThreadManagerAdapter.hpp
Author: Chunhui Ma
Purpose: ThreadManagerAdapter is adapter for 2 thread manager;
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