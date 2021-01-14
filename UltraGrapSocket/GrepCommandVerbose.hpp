#pragma once
/*
File GrepCommandVerbose.hpp
Author: Chunhui Ma
Purpose: command class is user to run;
*/
#include "SocketWraper.hpp"
#include "Command.hpp"
#include <mutex>
class GrepCommandVerbose : public GrepCommand {

public:
	SocketWraper* socket;
	virtual void execute() override;
};

