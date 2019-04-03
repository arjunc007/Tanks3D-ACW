#pragma once
#include "System.h"
#include "TCPSocket.h"
#include "UDPSocket.h"
#include <vector>
#include <thread>
#include <mutex>

class NetworkComponent;

class NetworkSystem :
	public System
{

	//Data
private:
	int _id;
	std::vector<int> _clientIDs;
	std::vector<TCPSocket> _clientSockets;
	std::thread _tcpListenerThread;
	std::thread _udpListenerThread;
	std::thread _udpBroadcastThread;
	TCPSocket* _tcpListenerSocket;
	UDPSocket* _udpListenerSocket;

	//Flags
	bool _isListeningUDP;
	unsigned int _maxClients;

	//Structors
public:
	NetworkSystem();
	virtual ~NetworkSystem();

	//Functions
public:
	virtual void Process(std::vector<GameObject*>& list, double deltaTime);
	void Initialise(int port);
	void Add(sockaddr_in client);
	void Acknowledge(TCPSocket s);
	void Find(int port);
	void UpdateID(const char* msg);
	//void Decode(WPARAM wParam);

private:
	void ListenUDP(int port);
	void ListenTCP(int port);
};

