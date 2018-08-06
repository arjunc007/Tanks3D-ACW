#include "CollisionComponent.h"
#include "GameObject.h"
#include "CollisionMessage.h"
#include <algorithm>

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

CollisionComponent::CollisionComponent(GameObject* gob)
	: GameObjectComponent("collision", gob),
	_radius(0),
	_collisionMatrix(0),
	_collisionID(0)
{

}

/******************************************************************************************************************/

CollisionComponent::~CollisionComponent()
{

}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void CollisionComponent::Start()
{

}

/******************************************************************************************************************/

// Main update function (called every frame)
void CollisionComponent::Update(double deltaTime)
{

}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void CollisionComponent::OnMessage(Message* msg)
{

}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void CollisionComponent::End()
{

}

/******************************************************************************************************************/
float CollisionComponent::GetCollisionRadius() const
{
	float maxScale = std::max(_parent->GetScale().x, _parent->GetScale().z);
	return _radius * maxScale;
}