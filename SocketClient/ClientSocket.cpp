#include "ClientSocket.hpp"
using namespace std;
unsigned short constexpr PORT = 27015;

SocketClient::SocketClient()
{
	hSocket = SOCKET_ERROR;
	WSAData wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		shutDown();
	}
}

bool SocketClient::connects(string ip)
{
	// Create the TCP socket
	hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Create the server address
	sockaddr_in serverAddress = { 0 };
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());

	// make a connection
	if (connect(hSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		hSocket = SOCKET_ERROR;
		return false;
	}

	return true;
}

string SocketClient::receive()
{
	char recvbuf[256] = "";
	int bytesRecv = recv(hSocket, recvbuf, 256, 0);
	recvbuf[0];
	return string(recvbuf);
}

void SocketClient::sends(string s)
{

	auto c = s.c_str();
	auto len = strlen(s.c_str()) + 1;
	send(hSocket, c, static_cast<int>(len), 0);

}

void SocketClient::shutDown()
{
	closesocket(hSocket);
	WSACleanup();
}

