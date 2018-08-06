#include "Tank.h"
#include "CollisionMatrix.h"
#include "CollisionMessage.h"
#include "Components.h"
#include "DeadObjectMessage.h"
#include "Game.h"
#include "Logger.h"

const float Tank::MAX_SPEED = 20.0f;

/******************************************************************************************************************/

Tank::Tank(Mesh * mesh, Material* material) 
	: GameObject("Tank")
{
	// Create components for Tank (they will add themselves)
	new TankControllerComponent(this);
	new TankAIComponent(this);
	
	PhysicsComponent* pc = new PhysicsComponent(this);
	pc->SetMaxSpeed(MAX_SPEED);

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetCollisionRadius(mesh->CalculateMaxSize() * 0.9f);
	cc->SetCollisionID(CollisionID::Tank_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Arena_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Obstacle_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Shell_ID);

	RenderComponent* rc = new RenderComponent(this);
	rc->SetMaterial(material);
	rc->SetMesh(mesh);
	rc->ShouldDraw(true);

	NetworkComponent* nc = new NetworkComponent(this);
}

/******************************************************************************************************************/

Tank::~Tank()
{
}

/******************************************************************************************************************/

void Tank::Update(double deltaTime)
{
	GameObject::Update(deltaTime);
	// Cap speed
	/*PhysicsComponent* physics = (PhysicsComponent*)GetComponent("physics");
	physics->LimitToMaximumSpeed(MAX_SPEED);*/

}

/******************************************************************************************************************/

void Tank::OnMessage(Message* msg)
{
	if(msg->GetMessageType() == "collision")
	{ 
		CollisionMessage* col = static_cast<CollisionMessage*>(msg);
		if (col->GetCollidee() == this ||
			col->GetCollider() == this)
		{
			RenderComponent* rc = static_cast<RenderComponent*>(GetComponent("render"));
			//rc->ShouldDraw(false);
			//_alive = false;

			//Send out death message
			//DeadObjectMessage dom(this);
			//OnMessage(&dom);

			//Change the game state
			//Message msg("state=dead");
			//Game::TheGame->ListenToMessage(&msg);
		}
	}

	GameObject::OnMessage(msg);
}

/******************************************************************************************************************/

void Tank::Reset()
{
	_alive = true;

	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(true);

	PhysicsComponent* pc = (PhysicsComponent*)GetComponent("physics");
	pc->SetVelocity(DirectX::XMFLOAT3());

	TankControllerComponent* tc = (TankControllerComponent*)GetComponent("input");
	tc->Reset();

	_position = DirectX::XMFLOAT3(0.0f, 0.6f, 0.0f);

	TankAIComponent* tai = (TankAIComponent*)GetComponent("AI");
	tai->Reset();
}

/******************************************************************************************************************/