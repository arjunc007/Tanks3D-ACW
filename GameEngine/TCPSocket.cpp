#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "TCPSocket.h"
#include "Logger.h"
#include "Window.h"


TCPSocket::TCPSocket()
	: Socket()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (_socket == INVALID_SOCKET) {
		Log("Create socket failed");
	}
}

TCPSocket::TCPSocket(sockaddr_in addr)
	: Socket(addr)
{

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
		Log("Send failed with ");
		Log(e);
	}
}

TCPSocket TCPSocket::Accept()
{
	SOCKET s = accept(_socket, NULL, NULL);
	if (s == INVALID_SOCKET) {
		int e = WSAGetLastError();
		Log("Accept failed with ");
		Log(e);
		return TCPSocket(INVALID_SOCKET);
	}
#ifdef _DEBUG
	Log("Received packet");
	Log(s);
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
				Log("Receive failed with ");
				Log(e);
			}
			else
				receivedBytes += bytes;

		} while (receivedBytes < len);

	}
	IsListening(false);

	return s.GetInfo();
}

void TCPSocket::Listen(const int port)
{
#ifdef _DEBUG
	Log("Start TCP Listening...");
#endif
	
	//Make nonblocking
	u_long mode = 1;
	if (ioctlsocket(_socket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		int e = WSAGetLastError();
		Log("Setting nonblocking falied");
		Log(e);
	}
	else if (listen(_socket, 5) == SOCKET_ERROR)
	{
		int e = WSAGetLastError();
		Log("Setting nonblocking falied");
		Log(e);
	}
}