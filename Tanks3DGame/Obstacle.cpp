#include "Obstacle.h"
#include "Components.h"
#include "CollisionMatrix.h"
#include "CollisionMessage.h"

Obstacle::Obstacle(Mesh* mesh, Material* material)
	: GameObject("obstacle")
{
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetCollisionRadius(mesh->CalculateMaxSize() * 0.9f);
	cc->SetCollisionID(CollisionID::Obstacle_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Tank_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Shell_ID);

	RenderComponent* rc = new RenderComponent(this);
	rc->SetMaterial(material);
	rc->SetMesh(mesh);
	rc->ShouldDraw(true);
}


Obstacle::~Obstacle()
{
}

void Obstacle::Update(double deltaTime)
{
	GameObject::Update(deltaTime);
}

/******************************************************************************************************************/

void Obstacle::OnMessage(Message* msg)
{
	if (msg->GetMessageType() == "collision")
	{
		CollisionMessage* col = static_cast<CollisionMessage*>(msg);
		if (col->GetCollidee() == this ||
			col->GetCollider() == this)
		{

		}
	}

	GameObject::OnMessage(msg);
}

/******************************************************************************************************************/

void Obstacle::Reset()
{
	_alive = true;
	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(true);

	_position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
}