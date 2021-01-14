

#include "SocketWraper.hpp"
unsigned short constexpr PORT = 27015;
using namespace std;


SocketWraper::SocketWraper()
{
	hAccepted = SOCKET_ERROR;

	WSAData wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cerr << "WSAStartup failed: " << iResult << endl;
	}

}

void SocketWraper::setUpServer(string ip)
{
	// Create the TCP socket
	hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Create the server address
	sockaddr_in serverAddress = { 0 };
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	this->ip = ip;
	serverAddress.sin_addr.s_addr = inet_addr(this->ip.c_str());

	if (bind(hSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		cerr << "bind() failed\n.";
		shutDown();
	}

	if (listen(hSocket, 10) == SOCKET_ERROR) {
		cerr << "Error listening on socket.\n";
		shutDown();
	}
}

std::string SocketWraper::receive()
{
	char recvbuf[128] = "";
	int bytesRecv = recv(hAccepted, recvbuf, 128, 0);
	recvbuf[0];
	return string(recvbuf);
}

void SocketWraper::sends(string s)
{

	send(hAccepted, s.c_str(), static_cast<int>(strlen(s.c_str())) + 1, 0);

}

void SocketWraper::connect(string ip)
{
	cout << "Waiting for connection\n";
	while (hAccepted == SOCKET_ERROR)
		hAccepted = accept(hSocket, NULL, NULL);
	cout << "A client connected\n";
}

void SocketWraper::drop()
{

	closesocket(hAccepted);
	hAccepted = SOCKET_ERROR;
	connect(ip);
}

void SocketWraper::shutDown()
{
	closesocket(hAccepted);
	closesocket(hSocket);
	WSACleanup();

}