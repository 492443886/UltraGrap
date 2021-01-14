/*
File File.hpp
Author: Chunhui Ma
Purpose: Data stucture
*/

#pragma once

#include <string>
#include <vector>

struct Line {

	int number;
	size_t match;
	std::string content;
};
struct File
{
	std::string name;
	std::vector<Line> lines;
};