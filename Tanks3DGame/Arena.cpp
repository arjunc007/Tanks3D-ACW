#include "Arena.h"
#include "CollisionMatrix.h"
#include"CollisionMessage.h"
#include "Components.h"


Arena::Arena(Mesh* mesh, Material* material)
	: GameObject("arena")
{
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetCollisionRadius(mesh->CalculateMaxSize() * 4.5f);
	cc->SetCollisionID(CollisionID::Arena_ID);
	//cc->SetCollisionMatrixFlag(CollisionID::Tank_ID);
	//cc->SetCollisionMatrixFlag(CollisionID::Shell_ID);

	RenderComponent* rc = new RenderComponent(this);
	rc->SetMaterial(material);
	rc->SetMesh(mesh);
	rc->ShouldDraw(true);
}


Arena::~Arena()
{
}

void Arena::Update(double deltaTime)
{
	GameObject::Update(deltaTime);
}

/******************************************************************************************************************/

void Arena::OnMessage(Message* msg)
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

void Arena::Reset()
{
	_alive = true;
	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(true);

	_position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
}