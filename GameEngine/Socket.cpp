#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Socket.h"
#include "Logger.h"

#define DATA_BUFSIZE 8192

Socket::Socket()
	: _isListening(false),
	_status(DISCONNECTED)
{
}

Socket::Socket(sockaddr_in addr)
	: _socketData(addr),
	_isListening(false),
	_status(DISCONNECTED)
{
}

Socket::Socket(SOCKET s) 
	: _socket(s),
	_isListening(false),
	_status(DISCONNECTED)
{}


Socket::~Socket()
{
}

//Set the sockaddr_in structure directly
void Socket::SetInfo(sockaddr_in s)
{
	_socketData = s;
}

//Set the ip and port
void Socket::SetDest(const char* ip, const int port)
{
	_socketData.sin_family = AF_INET;
	_socketData.sin_port = htons(port);
	_socketData.sin_addr.s_addr = inet_addr(ip);
}

void Socket::SetOptions(const int level, const int options)
{
	char enable = '1';

	if (setsockopt(_socket, level, options, &enable, sizeof(int)) < 0)
	{
		Logger::Log("Broadcast failed");
		Logger::Log(WSAGetLastError());
		Disconnect();
	}
}

void Socket::Bind()
{
	if (bind(_socket, (sockaddr *)&_socketData, sizeof(_socketData)) == SOCKET_ERROR) {
		int e = WSAGetLastError();
		Logger::Log("Bind failed with ");
		Logger::Log(e);
	}
}

bool Socket::Connect()
{
	if (connect(_socket, (sockaddr *)&_socketData, sizeof(_socketData)) == SOCKET_ERROR) {
		int e = WSAGetLastError();
		Logger::Log("Connect to peer failed with ");
		Logger::Log(e);
		return false;
	}

	_status = CONNECTED;
	return true;
}

bool Socket::Disconnect()
{
	if(shutdown(_socket, SD_SEND) != NO_ERROR)
		return false;

	char buffer;
	int result;
	int bufferlen = 2000;
	do {
		result = recv(_socket, &buffer, bufferlen, 0);
	} while (result > 0);
	
	closesocket(_socket);

	_status = DISCONNECTED;

	return true;
}

bool Socket::Equals(const Socket &b) const
{
	//Return true if IP is the same
	return (strcmp(inet_ntoa(_socketData.sin_addr), inet_ntoa(b.GetInfo().sin_addr)) == 0
		&& _socketData.sin_port == b.GetInfo().sin_port);
}

bool operator==(const Socket& a, const Socket& b) 
{ 
	return a.Equals(b); 
}