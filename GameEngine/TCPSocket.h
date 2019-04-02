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
	int Send(const char* msg, const int len);
	TCPSocket Accept();
	int Read(char* buffer, const int len, sockaddr_in* clientInfo = { 0 });
	void Listen();
};

