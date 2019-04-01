#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "NetworkSystem.h"
#include "GameObject.h"
#include "NetworkComponent.h"
#include "Logger.h"
#include "NetworkMessage.h"
#include "Message.h"
#include "Game.h"

#include <thread>

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

NetworkSystem::NetworkSystem() 
	: _isListeningUDP(false)
	, _maxClients(4)
{
	// Create version identifier
	WORD wVersionRequested = MAKEWORD(2, 0);

	// Startup windows sockets
	WSADATA _wsaData;
	if (WSAStartup(wVersionRequested, &_wsaData)) {
		Logger::Log("Socket initialisation failed");
	}

	srand((unsigned)time(0));
	_id = rand();
}

/******************************************************************************************************************/

NetworkSystem::~NetworkSystem()
{
	// Cleanup windows sockets
	WSACleanup();
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void NetworkSystem::Process(std::vector<GameObject*>& list, double deltaTime)
{
	//Listen to any client messages
	char buffer[2000];
	//sockaddr_in client = _tcpListenerSocket->Read(buffer, sizeof(buffer));

	for (auto o : list)
	{
		// Does this object even have a Network component?
		if (NetworkComponent* pc = (NetworkComponent*)(o->GetComponent("network")))
		{
			//Send data in message over internet to all clients
		}
	}
}

/******************************************************************************************************************/

void NetworkSystem::ListenUDP(int port)
{
	_udpListenerSocket = new UDPSocket();
	_udpListenerSocket->SetDest("0.0.0.0", port);
	_udpListenerSocket->SetOptions(SOL_SOCKET, SO_BROADCAST);
	_udpListenerSocket->Bind();

	char buffer[150];

#ifdef _DEBUG
	Logger::Log("Started UDP Listening on " + std::to_string(port));
#endif

	while (_clientSockets.size() < _maxClients)
	{
		sockaddr_in client = _udpListenerSocket->Read(buffer, sizeof(buffer));

		//Check if message is connect message
		NetworkMessage* msg = reinterpret_cast<NetworkMessage*>(buffer);
		Message* msgData = reinterpret_cast<Message*>(msg->GetMessageData());

#ifdef _DEBUG
		Logger::Log("Received UDP broadcast: " + msgData->GetMessageType() + " From " + std::string(inet_ntoa(client.sin_addr)) + "," + std::to_string(ntohs(client.sin_port)));
#endif

		u_short p = 0;
		if (msg->GetID() == _id)
		{
			Logger::Log("Received from self");
			goto clientexists;
		}

		if (msgData->GetMessageType().substr(0,7).compare("connect") == 0)
		{
			client.sin_port = std::stoi(msgData->GetMessageType().substr(7));
			p = ntohs(client.sin_port);
			//Check if client is already connected
			for (unsigned int i = 0; i < _clientSockets.size(); i++)
			{
				if (_clientSockets[i] == TCPSocket(client))
				{
					Logger::Log("Client already exists");
					goto clientexists;
				}
			}

			Add(client);
		clientexists:
			;
		}
	}
}

/******************************************************************************************************************/

void NetworkSystem::ListenTCP(int port)
{
	_tcpListenerSocket = new TCPSocket();
	_tcpListenerSocket->SetOptions(SOL_SOCKET, SO_REUSEADDR);
	_tcpListenerSocket->SetDest("0.0.0.0", port);
	_tcpListenerSocket->Bind();
	_tcpListenerSocket->Listen();

	char buffer[2000];

	//set of socket descriptors  
	fd_set readfds;

	while (1)
	{
		//clear the socket set  
		FD_ZERO(&readfds);

		if(_clientSockets.size() > 0)
		{ 
			ZeroMemory(buffer, sizeof(buffer));
			TCPSocket client = _tcpListenerSocket->Read(buffer, sizeof(buffer));
		}


	}
}

/******************************************************************************************************************/

void NetworkSystem::Initialise(int port)
{
	//Listen for UDP broadcast packets
	_udpListenerThread = std::thread(&NetworkSystem::ListenUDP, this, port);

	//Start TCP Listener on another port
	_tcpListenerThread = std::thread(&NetworkSystem::ListenTCP, this, port + 5);
}

/******************************************************************************************************************/

void NetworkSystem::Add(sockaddr_in clientInfo)
{
	
	//Add to list of client sockets and start listening
	TCPSocket newClient(clientInfo);
	_clientSockets.push_back(newClient);
	Logger::Log("New client added");
	
	//Start TCP connection
	//newClient.ListenAsync(ntohs(client.sin_port));
	
	Acknowledge(newClient);
}

/******************************************************************************************************************/

void NetworkSystem::Acknowledge(TCPSocket s)
{
	//Then calculate the ID and send it back
	size_t id = _clientSockets.size();
	NetworkMessage msg = NetworkMessage(id, reinterpret_cast<char*>(&Message("ack")), sizeof(Message));
	s.Connect();
	s.Send(reinterpret_cast<char*>(&msg), sizeof(msg));
#ifdef _DEBUG
	Logger::Log("New client connected on");
	Logger::Log(id);
#endif
}

/******************************************************************************************************************/

void NetworkSystem::Find(int port)
{
	//Broadcast UDP packets and wait for response

	Message innerMsg("connect" + std::to_string(_tcpListenerSocket->GetInfo().sin_port));
	NetworkMessage msg = NetworkMessage(_id, reinterpret_cast<char*>(&innerMsg), sizeof(Message));

	Message* originalmessage = reinterpret_cast<Message*>(msg.GetMessageData());
	std::string msgstring = originalmessage->GetMessageType();
	
	UDPSocket broadcastSocket;
	//broadcastSocket.SetDest("150.237.93.255", port);
	broadcastSocket.SetDest("192.168.0.255", port);
	broadcastSocket.SetOptions(SOL_SOCKET, SO_BROADCAST);

	while (_clientSockets.size() < _maxClients)
	{
		broadcastSocket.Send(reinterpret_cast<char*>(&msg), sizeof(msg));
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

/******************************************************************************************************************/

void NetworkSystem::UpdateID(const char* data)
{
	const NetworkMessage* msg = reinterpret_cast<const NetworkMessage*>(data);
	_id = msg->GetID();
#ifdef _DEBUG
	Logger::Log("ID updated to ");
	Logger::Log(_id);
#endif
}

/******************************************************************************************************************/

//void NetworkSystem::Decode(WPARAM wParam)
//{
//	// Receive data from the socket in wParam
//	char data[2000];
//	Socket(wParam).Read(data, 2000);
//
//	Message* message = reinterpret_cast<Message*>(data);
//	//Check the type of message
//	if (message->GetMessageType() == "connect")
//	{
//		Add(Socket(wParam));
//		Acknowledge(Socket(wParam));
//	}
//	if (message->GetMessageType() == "ack")
//	{
//		Add(Socket(wParam));
//		UpdateID(data);
//	}
//}