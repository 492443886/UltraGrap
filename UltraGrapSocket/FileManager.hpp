#pragma once

/*
File FileMagnager.hpp
Author: Chunhui Ma
Purpose: FileMagnager is have file function;
*/

#include "File.hpp"
#include <filesystem>
#include <regex>
#include <map>
#include <mutex>
#include <fstream>  
#include <iostream>
#include <sstream>
#include "File.hpp"
//#include "CriticalSection.hpp"

class FileMagnager {

private:
public:
	std::map<std::string, std::vector<File>>files;
	bool verdose;
	std::regex exprRegex;
	std::regex extentionsRegex;
public:
	bool IsMatch(std::filesystem::path p);
	std::vector<Line> analyse(std::filesystem::path p);
	std::vector<File> scan(std::string fd);
	std::vector<File> scanV(std::string fd);
	std::stringstream report();
};


