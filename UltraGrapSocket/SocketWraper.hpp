#pragma once
/*
File SocketWraper.hpp
Author: Chunhui Ma
Purpose: SocketWraper is have socket function;
*/

#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib")

class SocketWraper {

	SOCKET hSocket;
	SOCKET hAccepted;
	std::string ip;
public:
	SocketWraper();
	void setUpServer(std::string ip);
	std::string receive();
	void sends(std::string);
	void connect(std::string ip);
	void drop();
	void shutDown();
};

