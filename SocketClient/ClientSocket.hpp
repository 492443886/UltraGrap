#pragma once
/*
File SocketClient.hpp
Author: Chunhui Ma
Purpose: A class that will be used When USE NO-WIN32 PLATFORM
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib")
#include <iostream>

class SocketClient {
	SOCKET hSocket;
public:
	SocketClient();
	bool connects(std::string ip);
	std::string receive();
	void sends(std::string s);
	void shutDown();
	inline bool isConnected() {
		return hSocket != SOCKET_ERROR;
	}
	inline void drop() {
		closesocket(hSocket);
		hSocket = SOCKET_ERROR;
	}
};