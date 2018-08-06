#include "NetworkMessage.h"
#include "Logger.h"


NetworkMessage::NetworkMessage(int id, const char* msg)
	: Message("network"),
	_id(id)
{
	if (strlen(msg) < 50)
	{
		for (unsigned int i = 0; i < strlen(msg); i++)
		{
			_msg[i] = *(msg + i);
		}
	}
	else
		Log("Message exceeds capacity");
}


NetworkMessage::~NetworkMessage()
{
}
