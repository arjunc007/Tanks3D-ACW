#include "PhysicsSystem.h"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "Logger.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

PhysicsSystem::PhysicsSystem()
{
}

/******************************************************************************************************************/

PhysicsSystem::~PhysicsSystem()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void PhysicsSystem::Process(std::vector<GameObject*>& list, double deltaTime)
{
	for (auto o : list)
	{
		// Does this object even have a Physics component?
		if (PhysicsComponent* pc = (PhysicsComponent*)(o->GetComponent("physics")))
		{
			DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&pc->GetObjectPosition());
			DirectX::XMVECTOR vel = DirectX::XMLoadFloat3(&pc->GetVelocity());

			//newPos = pos + vel * deltaTime
			pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(vel, static_cast<float>(deltaTime)), pos);

			DirectX::XMFLOAT3 newPos;
			DirectX::XMStoreFloat3(&newPos, pos);

			pc->SetObjectPosition(newPos);

			// Limit speed
			float maxSpeed = pc->GetMaxSpeed();
			if (maxSpeed > 0)
			{
				pc->LimitToMaximumSpeed(maxSpeed);
			}
		}
	}
}

/******************************************************************************************************************/
