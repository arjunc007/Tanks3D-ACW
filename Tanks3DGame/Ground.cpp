#include "Ground.h"
#include "Renderer.h"
#include "Components.h"

Ground::Ground(Mesh* mesh, Material* material) 
	: GameObject("Ground")
{
	RenderComponent* rc = new RenderComponent(this);
	rc->SetMaterial(material);
	rc->SetMesh(mesh);
	rc->ShouldDraw(true);
}


Ground::~Ground()
{
}

/******************************************************************************************************************/

void Ground::Update(double deltaTime)
{
	GameObject::Update(deltaTime);
}

/******************************************************************************************************************/

void Ground::OnMessage(Message* msg)
{
	GameObject::OnMessage(msg);
}

/******************************************************************************************************************/

void Ground::Reset()
{
	_alive = true;
	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(true);

	_position = DirectX::XMFLOAT3();
}