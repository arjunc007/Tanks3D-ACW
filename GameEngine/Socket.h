#pragma once
#include <winsock2.h>

#pragma comment (lib, "Ws2_32.lib")

class Socket
{
	//Data
protected:
	SOCKET _socket;
	sockaddr_in _socketData;
	bool _isListening;

	//Structors
public:
	Socket();
	Socket(SOCKET s);
	Socket(sockaddr_in addr);
	virtual ~Socket();

	//Gets/Sets
public:
	SOCKET GetSocket() const { return _socket; }
	sockaddr_in GetInfo() const { return _socketData; }
	void SetInfo(sockaddr_in);
	bool IsListening() const { return _isListening; }
	void IsListening(bool b) { _isListening = b; }

	//Functions
public:
	virtual void Send(const char* msg, const int len) = 0;
	void SetDest(const char* ip, const int port);
	void SetOptions(const int level, const int option);
	void Bind();
	virtual sockaddr_in Read(char* buffer, const int len) = 0;
	bool Connect();
	bool Disconnect();
	
	bool Equals(const Socket& s) const;
};

bool operator==(const Socket& a, const Socket& b);