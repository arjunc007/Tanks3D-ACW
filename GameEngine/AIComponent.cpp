#include "AIComponent.h"



AIComponent::AIComponent(GameObject* parent)
	: GameObjectComponent("AI", parent),
	_enabled(false)
{
}


AIComponent::~AIComponent()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void AIComponent::Start()
{
}

/******************************************************************************************************************/

// Main update function (called every frame)
void AIComponent::Update(double deltaTime)
{


}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void AIComponent::OnMessage(Message* msg)
{
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void AIComponent::End()
{
}

/******************************************************************************************************************/