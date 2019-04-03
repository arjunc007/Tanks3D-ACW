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
		sockaddr_in client;
		_udpListenerSocket->Read(buffer, sizeof(buffer), &client);

		//Check if message is connect message
		NetworkMessage* msg = reinterpret_cast<NetworkMessage*>(buffer);
		Message* msgData = reinterpret_cast<Message*>(msg->GetMessageData());

		u_short p = 0;
		if (msg->GetID() == _id)
		{
			goto clientexists;
		}

#ifdef _DEBUG
		Logger::Log("Received UDP : " + msgData->GetMessageType() + " From " + std::string(inet_ntoa(client.sin_addr)) + "," + std::to_string(ntohs(client.sin_port)));
#endif

		if (msgData->GetMessageType().substr(0,7).compare("connect") == 0)
		{
			client.sin_port = std::stoi(msgData->GetMessageType().substr(7));
			p = ntohs(client.sin_port);
			//Check if client is already connected
			for (unsigned int i = 0; i < _clientIDs.size(); i++)
			{
				if (_clientIDs[i] == msg->GetID())
				{
					Logger::Log("Client already exists");
					goto clientexists;
				}
			}

			_clientIDs.push_back(msg->GetID());
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

	char buffer[1024];

	//set of socket descriptors  
	fd_set readfds;
	int sd, max_sd;
	TCPSocket newClient;

	while (1)
	{
		//clear the socket set  
		FD_ZERO(&readfds);

		FD_SET(_tcpListenerSocket->GetSocket(), &readfds);
		max_sd = _tcpListenerSocket->GetSocket();

		//add child sockets to set  
		for (int i = 0; i < _clientSockets.size(); i++)
		{
			//socket descriptor  
			sd = _clientSockets[i].GetSocket();

			//if valid socket descriptor then add to read list  
			if (sd > 0)
				FD_SET(sd, &readfds);

			//highest file descriptor number, need it for the select function  
			if (sd > max_sd)
				max_sd = sd;
		}

		//wait for an activity on one of the sockets , timeout is NULL ,  
		//so wait indefinitely 

		if ((select(max_sd + 1, &readfds, NULL, NULL, NULL) < 0) && (errno != EINTR))
		{
			Logger::Log("select error");
		}

		//If something happened on the master socket ,  
		//then its an incoming connection  
		if (FD_ISSET(_tcpListenerSocket->GetSocket(), &readfds))
		{
			if ((newClient = _tcpListenerSocket->Accept()) == TCPSocket(INVALID_SOCKET))
			{
				Logger::Log("Accept failed");
			}

			//inform user of socket number - used in send and receive commands  
			Logger::Log("New connection , socket fd is " + std::to_string(newClient.GetSocket()) 
				+ ", ip is : " + inet_ntoa(newClient.GetInfo().sin_addr) + ", port : " 
				+ std::to_string(ntohs(newClient.GetInfo().sin_port)));

			//send new connection greeting message
			Message msg("ACK");
			NetworkMessage nmsg(_id, reinterpret_cast<char*>(&msg), sizeof(msg));
			if (newClient.Send(reinterpret_cast<char*>(&nmsg), sizeof(nmsg)) != sizeof(nmsg))
			{
				Logger::Log("ACK message not sent fully");
			}

			Logger::Log("ACK sent successfully");

			//add new socket to array of sockets  
			_clientSockets.push_back(newClient);
		}
		int bytesRead;
		//else its some IO operation on some other socket 
		for (int i = 0; i < _clientSockets.size(); i++)
		{
			sd = _clientSockets[i].GetSocket();

			if (FD_ISSET(sd, &readfds))
			{
				//Check if it was for closing , and also read the  
				//incoming message  
				if (bytesRead = _clientSockets[i].Read(buffer, sizeof(buffer)) == 0)
				{
					//Somebody disconnected , get his details and print  
					Logger::Log(std::string("Host disconnected , ip ") + inet_ntoa(_clientSockets[i].GetInfo().sin_addr) + ", port: "
						+ std::to_string(ntohs(_clientSockets[i].GetInfo().sin_port)));

					//Close the socket and mark as 0 in list for reuse  
					_clientSockets[i].Disconnect();
					_clientSockets.erase(_clientSockets.begin() + i);
					i--;
				}

				//Echo back the message that came in  
				else
				{
					//set the string terminating NULL byte on the end  
					//of the data read  
					buffer[bytesRead] = '\0';
					Logger::Log(buffer);
					_clientSockets[i].Send("Received ACK", 13);
				}
			}
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

	_udpBroadcastThread = std::thread(&NetworkSystem::Find, this, 9173);
}

/******************************************************************************************************************/

void NetworkSystem::Add(sockaddr_in clientInfo)
{
	
	//Add to list of client sockets and start listening
	TCPSocket newClient(clientInfo);

	if (newClient.Connect())
	{
		Message innerMsg("ACK" + std::to_string(_tcpListenerSocket->GetInfo().sin_port));
		NetworkMessage msg(_id, reinterpret_cast<char*>(&innerMsg), sizeof(innerMsg));
		newClient.Send(reinterpret_cast<char*>(&msg), sizeof(msg));
		Logger::Log("ACK sent to new client");
	}
	else
	{
		_clientIDs.pop_back();
	}
	
	//Acknowledge(newClient);
}

/******************************************************************************************************************/

void NetworkSystem::Acknowledge(TCPSocket s)
{
	//Then calculate the ID and send it back
	size_t id = _clientSockets.size();
	Message ack("ACK");
	NetworkMessage msg = NetworkMessage(id, reinterpret_cast<char*>(&ack), sizeof(ack));
	s.Connect();
	s.Send(reinterpret_cast<char*>(&msg), sizeof(msg));
#ifdef _DEBUG
	Logger::Log("New client connected on" + std::to_string(id));
#endif
}

/******************************************************************************************************************/

void NetworkSystem::Find(int port)
{
	while (_tcpListenerSocket == nullptr || !_tcpListenerSocket->IsListening());
	//Broadcast UDP packets and wait for response

	Message innerMsg("connect" + std::to_string(_tcpListenerSocket->GetInfo().sin_port));
	NetworkMessage msg = NetworkMessage(_id, reinterpret_cast<char*>(&innerMsg), sizeof(Message));
	
	UDPSocket broadcastSocket;
	//broadcastSocket.SetDest("150.237.93.255", port);
	broadcastSocket.SetDest("127.0.0.255", port);
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