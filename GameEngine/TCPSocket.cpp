#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "TCPSocket.h"
#include "Logger.h"
#include "Window.h"

#include <string>

TCPSocket::TCPSocket() : Socket()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (_socket == INVALID_SOCKET) {
		Logger::Log("Create socket failed");
	}
}

TCPSocket::TCPSocket(sockaddr_in addr = { 0 })
	: Socket(addr)
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (_socket == INVALID_SOCKET) {
		Logger::Log("Create socket failed");
	}
}

TCPSocket::TCPSocket(SOCKET s) : Socket(s)
{

}


TCPSocket::~TCPSocket()
{
}

int TCPSocket::Send(const char* msg, const int len)
{
	int bytesSent = send(_socket, msg, len, 0);
	if (bytesSent == SOCKET_ERROR)
	{
		int e = WSAGetLastError();
		Logger::Log("Send failed with ");
		Logger::Log(e);
	}

	return bytesSent;
}

TCPSocket TCPSocket::Accept()
{
	sockaddr_in addr;
	int addrlen = sizeof(addr);
	SOCKET s = accept(_socket, (sockaddr*)&addr, &addrlen);
	if (s == INVALID_SOCKET) {
		int e = WSAGetLastError();
		Logger::Log("Accept failed with ");
		Logger::Log(e);
		return TCPSocket(INVALID_SOCKET);
	}

#ifdef _DEBUG
	Logger::Log("Accepted socket " + std::to_string(s));
#endif

	TCPSocket acceptedSocket(s);
	acceptedSocket.SetInfo(addr);
	return acceptedSocket;
}

int TCPSocket::Read(char* buffer, const int len, sockaddr_in* clientInfo)
{
	int bytes = 0;
	
	bytes = recv(_socket, buffer, len, 0);

	if (bytes == SOCKET_ERROR) {
		int e = WSAGetLastError();
		Logger::Log("TCP Receive failed with " + std::to_string(e));
	}

	return bytes;
}

void TCPSocket::Listen()
{
	//Make nonblocking
	u_long mode = 1;
	if (ioctlsocket(_socket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		int e = WSAGetLastError();
		Logger::Log("Setting nonblocking failed");
		Logger::Log(e);
		return;
	}
	else if (listen(_socket, 5) == SOCKET_ERROR)
	{
		int e = WSAGetLastError();
		Logger::Log("TCP Listen failed");
		Logger::Log(e);
		return;
	}

	_status = LISTENING;

#ifdef _DEBUG
	Logger::Log("Started TCP Listening on port " + std::to_string(ntohs(_socketData.sin_port)));
#endif
}