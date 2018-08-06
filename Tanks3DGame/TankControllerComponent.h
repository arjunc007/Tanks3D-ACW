#pragma once
#include "UserInputComponent.h"
#include "Shell.h"

#include <vector>

class TankControllerComponent
	: public UserInputComponent
{
	//Constants
public:
	static const float ACCELERATION;
	static const unsigned int NUM_SHELLS;
	static const float FIRE_DELAY;

	//Data
protected:
	bool _turnLeftPressed;
	bool _turnRightPressed;
	bool _acceleratePressed;
	bool _reversePressed;
	double _fireDelay;
	std::vector<Shell*> _shells;

	//Structors
public:
	TankControllerComponent(GameObject* gob);
	virtual ~TankControllerComponent();

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


	// New Functions
public:
	void TurnLeft(float angle);
	void TurnRight(float angle);
	void Accelerate(float amt);
	void Reverse(float amt);
	void Fire();
	void Reset();
};

