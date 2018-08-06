#pragma once
#include "AIComponent.h"

class TankAIComponent
	: public AIComponent
{
	//Enums
	enum BotState
	{
		Hunting,
		Engaging
	};

	//Data
private:
	BotState _state;

	//Structors
public:
	TankAIComponent(GameObject* parentgob);
	virtual ~TankAIComponent();

	//Functions
public:

	// Setup function -- called when parent object is initialised (using its own Start method)
	virtual void Start();

	// Main update function (called every frame)
	virtual void Update(double deltaTime);

	// Message handler (called when message occurs)
	virtual void OnMessage(Message* msg);

	// Shutdown function -- called when parent object is destroyed
	virtual void End();

	//Extra functions
	void Reset();
};

