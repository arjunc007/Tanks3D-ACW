#pragma once
#include "GameObjectComponent.h"
#include "DirectXMath.h"

class PhysicsComponent
	: public GameObjectComponent
{
	// Data
protected:
	DirectX::XMFLOAT3		_velocity;	// Velocity of object
	float					_maxSpeed;  // Maximum speed


								// Structors
public:
	PhysicsComponent(GameObject* gob);
	virtual ~PhysicsComponent();

	// Gets/Sets
public:
	DirectX::XMFLOAT3 GetVelocity()	const { return _velocity; }
	void SetVelocity(DirectX::XMFLOAT3 v) { _velocity = v; }

	float GetMaxSpeed()		const { return _maxSpeed; }
	void SetMaxSpeed(float f) { _maxSpeed = f; }

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

	// Extra Functions
public:
	void LimitToMaximumSpeed(float max);
	DirectX::XMFLOAT3 GetObjectPosition();
	void SetObjectPosition(DirectX::XMFLOAT3 v);
};