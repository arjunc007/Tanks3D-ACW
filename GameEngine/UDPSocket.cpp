#include "UDPSocket.h"
#include "Logger.h"


UDPSocket::UDPSocket()
	: Socket()
{
	_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if (_socket == INVALID_SOCKET) {
		int e = WSAGetLastError();
		Logger::Log("Create broadcast socket failed");
		Logger::Log(e);
	}
}

UDPSocket::UDPSocket(SOCKET s) : Socket(s)
{

}

UDPSocket::~UDPSocket()
{
}

void UDPSocket::Send(const char* msg, const int len)
{
	if (sendto(_socket, msg, len, 0, (sockaddr*)&_socketData, sizeof(_socketData)) == SOCKET_ERROR)
	{
		int e = WSAGetLastError();
		Logger::Log("Sending message failed");
		Logger::Log(e);
	}
}

sockaddr_in UDPSocket::Read(char* buffer, const int len)
{
	sockaddr_in sender;
	int addrlen = sizeof(sockaddr_in);
	int bytes = 0;
	
	bytes = recvfrom(_socket, buffer, len, 0, (sockaddr*)&sender, &addrlen);

	if (bytes == SOCKET_ERROR)
	{
		int e = WSAGetLastError();
		Logger::Log("Receive failed with ");
		Logger::Log(e);
	}

	return sender;
}