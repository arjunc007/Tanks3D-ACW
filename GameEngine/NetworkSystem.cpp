#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "NetworkSystem.h"
#include "GameObject.h"
#include "NetworkComponent.h"
#include "Logger.h"
#include "NetworkMessage.h"
#include "Message.h"
#include "UDPSocket.h"
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
		Log("Socket initialisation failed");
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
	for (unsigned int i = 0; i < _clientSockets.size(); i++)
	{
		if (!_clientSockets[i]->IsListening())
		{
			char buffer[20];
			_clientSockets[i]->Bind();
			_clientSockets[i]->Listen(9171);
			std::thread(&TCPSocket::Read, _clientSockets[i], buffer, sizeof(buffer));

			_clientSockets[i]->IsListening(true);
		}
	}

	for (auto o : list)
	{
		// Does this object even have a Network component?
		if (NetworkComponent* pc = (NetworkComponent*)(o->GetComponent("network")))
		{
			//Send data in message over internet
		}
	}
}

/******************************************************************************************************************/

void NetworkSystem::Initialise(int port)
{
	//Listen for broadcast packets
	UDPSocket listeningSocket;
	listeningSocket.SetDest("0.0.0.0", 9172);
	listeningSocket.SetOptions(SOL_SOCKET, SO_BROADCAST);
	listeningSocket.Bind();

	char buffer[100];

#ifdef _DEBUG
	Log("Started Listening");
#endif

	while (1)
	{
		if (_clientSockets.size() < _maxClients)
		{
			sockaddr_in client = listeningSocket.Read(buffer, sizeof(buffer));

			//Check if message is connect message
			NetworkMessage* msg = reinterpret_cast<NetworkMessage*>(buffer);
			Message* msgData = reinterpret_cast<Message*>(msg->GetMessageData());

//#ifdef _DEBUG
//			Log(msg->GetMessageData());
//#endif
			if (msg->GetID() == _id)
			{
				Log("Received from self");
				goto clientexists;
			}

			if (msgData->GetMessageType() == "connect")
			{
				//Check if client is already connected
				for (unsigned int i = 0; i < _clientSockets.size(); i++)
				{
					if (*_clientSockets[i] == TCPSocket(client))
					{
						Log("Client already exists");
						goto clientexists;
					}
				}

				Add(client);
			clientexists:
				;
			}
		}
	}
}

/******************************************************************************************************************/

void NetworkSystem::Add(sockaddr_in client)
{
	
	//Add to list of client sockets and start listening
	TCPSocket newClient;
	newClient.SetInfo(client);
	_clientSockets.push_back(&newClient);
	Log("New client added");
	
	//Start TCP connection
	//newClient.ListenAsync(ntohs(client.sin_port));
	
	Acknowledge(newClient);
}

/******************************************************************************************************************/

void NetworkSystem::Acknowledge(TCPSocket s)
{
	//Then calculate the ID and send it back
	int id = _clientSockets.size();
	NetworkMessage msg = NetworkMessage(id, "ack");
	s.Connect();
	s.Send(reinterpret_cast<char*>(&msg), sizeof(msg));
#ifdef _DEBUG
	Log("New client connected on");
	Log(id);
#endif
}

/******************************************************************************************************************/

void NetworkSystem::Find(int port)
{
	//Broadcast UDP packets and wait for response

	NetworkMessage msg = NetworkMessage(_id, reinterpret_cast<char*>(&Message("connect")));
	
	UDPSocket broadcastSocket;
	broadcastSocket.SetDest("150.237.93.255", port);
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
	Log("ID updated to ");
	Log(_id);
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