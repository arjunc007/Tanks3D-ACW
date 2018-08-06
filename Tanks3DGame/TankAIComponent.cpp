#include "TankAIComponent.h"
#include "GameObject.h"


TankAIComponent::TankAIComponent(GameObject* gob)
	: AIComponent(gob),
	_state(Hunting)
{
}


TankAIComponent::~TankAIComponent()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void TankAIComponent::Start()
{
}

/******************************************************************************************************************/

// Main update function (called every frame)
void TankAIComponent::Update(double deltaTime)
{
}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void TankAIComponent::OnMessage(Message* msg)
{
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void TankAIComponent::End()
{
}

/******************************************************************************************************************/

void TankAIComponent::Reset()
{
	_enabled = false;
}