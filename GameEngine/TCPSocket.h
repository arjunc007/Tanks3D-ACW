#pragma once
#include "Socket.h"

class TCPSocket
	: public Socket
{
	//Structors
public:
	TCPSocket();
	TCPSocket(sockaddr_in addr);
	TCPSocket(SOCKET s);
	~TCPSocket();

	//Functions
public:
	void Send(const char* msg, const int len);
	TCPSocket Accept();
	sockaddr_in Read(char* buffer, const int len);
	void Listen(const int port);
};

