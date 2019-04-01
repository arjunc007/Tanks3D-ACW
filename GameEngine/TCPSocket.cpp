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

void TCPSocket::Send(const char* msg, const int len)
{
	if (send(_socket, msg, len, 0) == SOCKET_ERROR)
	{
		int e = WSAGetLastError();
		Logger::Log("Send failed with ");
		Logger::Log(e);
	}
}

TCPSocket TCPSocket::Accept()
{
	SOCKET s = accept(_socket, NULL, NULL);
	if (s == INVALID_SOCKET) {
		int e = WSAGetLastError();
		Logger::Log("Accept failed with ");
		Logger::Log(e);
		return TCPSocket(INVALID_SOCKET);
	}
#ifdef _DEBUG
	Logger::Log("Received packet");
	Logger::Log(s);
#endif

	return TCPSocket(s);
}

sockaddr_in TCPSocket::Read(char* buffer, const int len)
{
	TCPSocket s = Accept();
	int receivedBytes = 0;
	int bytes = 0;
	
	while (1)
	{
		do {
			bytes = recv(_socket, buffer + receivedBytes, len - receivedBytes, 0);

			if (bytes == SOCKET_ERROR) {
				int e = WSAGetLastError();
				Logger::Log("Receive failed with ");
				Logger::Log(e);
			}
			else
				receivedBytes += bytes;

		} while (receivedBytes < len);

	}
	IsListening(false);

	return s.GetInfo();
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

#ifdef _DEBUG
	Logger::Log("Started TCP Listening on port " + std::to_string(ntohs(_socketData.sin_port)));
#endif
}