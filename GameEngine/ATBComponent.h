#pragma once
#include "GameObjectComponent.h"
#include <AntTweakBar.h>

class ATBComponent
	: public GameObjectComponent
{
	//Data
protected:
	TwBar * myBar;

	//Structors
public:
	ATBComponent(GameObject* parent);
	~ATBComponent();

	//Gets/Sets
public:
	TwBar * GetBar() const { return myBar; }

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

	//Extra Functions
public:
	void Draw();
};

