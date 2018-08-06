#include "ConnectMessage.h"



ConnectMessage::ConnectMessage(int id)
	: Message("connect"),
	_id(id)
{
}


ConnectMessage::~ConnectMessage()
{
}
