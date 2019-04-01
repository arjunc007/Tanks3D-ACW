#include "NetworkMessage.h"
#include "Logger.h"


NetworkMessage::NetworkMessage(int id, const char* msg, int size)
	: Message("network"),
	_id(id)
{

	if (size < 50)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			_msg[i] = *(msg + i);
		}
	}
	else
		Logger::Log("Message exceeds capacity");
}


NetworkMessage::~NetworkMessage()
{
}
