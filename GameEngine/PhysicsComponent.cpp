#include "PhysicsComponent.h"
#include "GameObject.h"
#include "Message.h"
#include "ThrustMessage.h"
#include "Logger.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

PhysicsComponent::PhysicsComponent(GameObject* gob)
	: GameObjectComponent("physics", gob),
	_velocity(DirectX::XMFLOAT3())
{

}

/******************************************************************************************************************/

PhysicsComponent::~PhysicsComponent()
{

}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void PhysicsComponent::Start()
{
	_parent->RegisterListener("stop", this);
	_parent->RegisterListener("thrust", this);
}

/******************************************************************************************************************/

// Main update function (called every frame)
void PhysicsComponent::Update(double deltaTime)
{


}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void PhysicsComponent::OnMessage(Message* msg)
{
	if (msg->GetMessageType() == "thrust")
	{
		ThrustMessage* tm = (ThrustMessage*)msg;
		DirectX::XMVECTOR newVel = DirectX::XMVectorAdd(XMLoadFloat3(&tm->GetThrust()), DirectX::XMLoadFloat3(&_velocity));
		DirectX::XMStoreFloat3(&_velocity, newVel);
	}

	if (msg->GetMessageType() == "stop")
	{
		_velocity = DirectX::XMFLOAT3();
	}
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void PhysicsComponent::End()
{
	_parent->UnregisterListener("stop", this);
	_parent->UnregisterListener("thrust", this);
}

/******************************************************************************************************************/

void PhysicsComponent::LimitToMaximumSpeed(float max)
{
	DirectX::XMVECTOR clampedVel = DirectX::XMVector3ClampLength(DirectX::XMLoadFloat3(&_velocity), 0.0f, max);
	DirectX::XMStoreFloat3(&_velocity, clampedVel);
}

/******************************************************************************************************************/

const DirectX::XMFLOAT3& PhysicsComponent::GetObjectPosition()
{
	return _parent->GetPosition();
}

/******************************************************************************************************************/

void PhysicsComponent::SetObjectPosition(DirectX::XMFLOAT3 v)
{
	_parent->SetPosition(v);
}

/******************************************************************************************************************/
