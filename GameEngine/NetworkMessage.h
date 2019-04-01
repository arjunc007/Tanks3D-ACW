#pragma once
#include "Message.h"

class NetworkMessage :
	public Message
{
	int _id;
	char _msg[50];

public:
	NetworkMessage(int id, const char* msg, int size);
	~NetworkMessage();

	//Gets/Sets
public:
	int GetID() const { return _id; }
	char* GetMessageData() { return _msg; }
};

