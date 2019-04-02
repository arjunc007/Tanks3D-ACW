#pragma once
#include "Socket.h"

class UDPSocket
	: public Socket
{
	//Structors
public:
	UDPSocket();
	UDPSocket(SOCKET s);
	~UDPSocket();

	//Functions
public:
	int Send(const char* msg, const int len);
	int Read(char* buffer, const int len, sockaddr_in* clientInfo);
};

