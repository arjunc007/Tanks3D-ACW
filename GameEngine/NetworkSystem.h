#pragma once
#include "System.h"
#include "TCPSocket.h"
#include <vector>

class NetworkComponent;

class NetworkSystem :
	public System
{

	//Data
private:
	int _id;
	//Socket _listeningSocket;
	std::vector<TCPSocket*> _clientSockets;
	TCPSocket* _hostSocket;

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
};

