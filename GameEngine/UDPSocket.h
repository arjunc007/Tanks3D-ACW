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
	void Send(const char* msg, const int len);
	sockaddr_in Read(char* buffer, const int len);
};

