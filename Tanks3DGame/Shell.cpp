#include "Shell.h"
#include "Components.h"
#include "CollisionMatrix.h"
#include "CollisionMessage.h"
#include "DeadObjectMessage.h"
#include "Logger.h"

const float Shell::SHELL_SPEED = 50.0f;
const float Shell::MAX_LIFE = 2.0f;

Shell::Shell(Mesh* mesh, Material* material) :
	GameObject("shell")
{
	// Create components for object (they will add themselves)
	new PhysicsComponent(this);
	new LifeTimerComponent(this, MAX_LIFE);

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetCollisionRadius(mesh->CalculateMaxSize() * 0.9f);
	cc->SetCollisionID(CollisionID::Shell_ID);
	//cc->SetCollisionMatrixFlag(CollisionID::Arena_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Obstacle_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Tank_ID);

	RenderComponent* rc = new RenderComponent(this);
	rc->SetMaterial(material);
	rc->SetMesh(mesh);
	rc->ShouldDraw(true);

	_alive = false;
}


Shell::~Shell()
{
}

void Shell::Update(double deltaTime)
{
	GameObject::Update(deltaTime);
}

void Shell::OnMessage(Message* msg)
{
	// If the bullet's life is expired, stop drawing it
	if (msg->GetMessageType() == "dead")
	{
		DeadObjectMessage* dead = (DeadObjectMessage*)msg;
		if (dead->GetDeadObject() == this)
		{
			RenderComponent* rc = (RenderComponent*)GetComponent("render");
			rc->ShouldDraw(false);
			_alive = false;
		}
	}
	else if (msg->GetMessageType() == "collision")
	{
		CollisionMessage* col = static_cast<CollisionMessage*>(msg);
		if (col->GetCollidee() == this ||
			col->GetCollider() == this)
		{
			Logger::Log(col->GetCollidee()->GetType());

			// Send out death message
			DeadObjectMessage dom(this);
			OnMessage(&dom);
		}
	}

	GameObject::OnMessage(msg);
}

void Shell::Reset()
{
	_alive = false;

	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(false);

	_position = DirectX::XMFLOAT3();
	_rotation = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Shell::Spawn(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 rotation)
{
	DirectX::XMVECTOR f = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, -1, 1), DirectX::XMLoadFloat4(&rotation));
	//m_pos = pos + f * dist 
	XMStoreFloat3(&_position, DirectX::XMVectorAdd(DirectX::XMVectorScale(f, 3.7f), DirectX::XMLoadFloat3(&position)));
	_position.y = position.y + 0.35f;
	_rotation = rotation;
	_alive = true;

	// Update components

	DirectX::XMFLOAT3 vel;
	DirectX::XMStoreFloat3(&vel, DirectX::XMVectorScale(f, SHELL_SPEED));
	PhysicsComponent* p = (PhysicsComponent*)GetComponent("physics");
	p->SetVelocity(vel);

	LifeTimerComponent* c = (LifeTimerComponent*)GetComponent("lifetimer");
	c->ResetLifeTime();

	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(true);
}