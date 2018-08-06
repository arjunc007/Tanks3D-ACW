#pragma once
#include "GameObjectComponent.h"

class AIComponent
	: public GameObjectComponent
{
	//Data
protected:
	bool _enabled;

	//Structors
public:
	AIComponent(GameObject* parent);
	~AIComponent();

	//Gets/Sets
public:
	bool IsEnabled() const { return _enabled; }
	void IsEnabled(const bool b) { _enabled = b; }

	// Functions
public:
	// Setup function -- called when parent object is initialised (using its own Start method)
	virtual void Start();

	// Main update function (called every frame)
	virtual void Update(double deltaTime);

	// Message handler (called when message occurs)
	virtual void OnMessage(Message* msg);

	// Shutdown function -- called when parent object is destroyed
	virtual void End();
};

