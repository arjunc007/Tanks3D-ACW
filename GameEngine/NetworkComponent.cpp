#include "NetworkComponent.h"
#include "GameObject.h"
#include "Message.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

NetworkComponent::NetworkComponent(GameObject* gob)
	: GameObjectComponent("network", gob)
{
}

/******************************************************************************************************************/

NetworkComponent::~NetworkComponent()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void NetworkComponent::Start()
{
	_parent->RegisterListener("stop", this);
	_parent->RegisterListener("thrust", this);
}

/******************************************************************************************************************/

// Main update function (called every frame)
void NetworkComponent::Update(double deltaTime)
{


}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void NetworkComponent::OnMessage(Message* msg)
{
	_data = reinterpret_cast<char*>(msg);

	//Convert data to bytes
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void NetworkComponent::End()
{
	_parent->UnregisterListener("stop", this);
	_parent->UnregisterListener("thrust", this);
}

/******************************************************************************************************************/