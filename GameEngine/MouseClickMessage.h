#pragma once
#include "Message.h"

class MouseClickMessage
	: public Message
{
	// Data
protected:
	int		_position[2];
	bool	_down;

	// Structors
public:
	MouseClickMessage(int x, int y, bool down);
	~MouseClickMessage();


	// Gets/Sets
public:
	const int* GetPosition()				const { return _position; }
	bool GetDown()				const { return _down; }


	// Functions
public:

};

