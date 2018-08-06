#pragma once
#include "Message.h"

class ConnectMessage
	: public Message
{
	int _id;

public:
	ConnectMessage(int id);
	~ConnectMessage();
};

