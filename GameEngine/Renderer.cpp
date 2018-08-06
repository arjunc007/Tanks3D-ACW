#include "Renderer.h"
#include "RenderComponent.h"

/******************************************************************************************************************/

Renderer::Renderer()
	: _clearColour(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f))
{
}

/******************************************************************************************************************/

Renderer::~Renderer()
{
}

/******************************************************************************************************************/

void Renderer::Draw(RenderComponent* rc, const DirectX::XMMATRIX &viewMatrix, const DirectX::XMMATRIX &projectionMatrix)
{
	//Position gameobject in world
	DirectX::XMMATRIX modelMatrix = DirectX::XMMatrixScaling(rc->GetScale().x, rc->GetScale().y, rc->GetScale().z);
	modelMatrix *= DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rc->GetRotation()));
	modelMatrix *= DirectX::XMMatrixTranslation(rc->GetPosition().x, rc->GetPosition().y, rc->GetPosition().z);

	if (rc->GetMesh())
	{
		Draw(rc->GetMesh(), rc->GetMaterial(), modelMatrix, viewMatrix, projectionMatrix);
	}
}

/******************************************************************************************************************/
