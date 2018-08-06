#include "MouseClickMessage.h"



MouseClickMessage::MouseClickMessage(int x, int y, bool down)
	: Message("mouseclick"),
	_position{x, y},
	_down(down)
{
}


MouseClickMessage::~MouseClickMessage()
{
}
