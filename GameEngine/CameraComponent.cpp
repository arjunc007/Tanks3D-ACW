#include "CameraComponent.h"
#include "GameObject.h"

/******************************************************************************************************************/

CameraComponent::CameraComponent(GameObject* gob)
	: GameObjectComponent("camera", gob),
	_lookAt(DirectX::XMFLOAT3()),
	_up(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
	_fieldOfView(DirectX::XM_PIDIV4),
	_nearClipPlane(0.1f),
	_farClipPlane(100.0f),
	_aspectRatio(1.7777777f)
{
}

/******************************************************************************************************************/

CameraComponent::~CameraComponent()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void CameraComponent::Start()
{
}

/******************************************************************************************************************/

// Main update function (called every frame)
void CameraComponent::Update(double deltaTime)
{


}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void CameraComponent::OnMessage(Message* msg)
{
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void CameraComponent::End()
{
}

/******************************************************************************************************************/

DirectX::XMMATRIX CameraComponent::GetViewMatrix() const
{
	return DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&_parent->GetPosition()),
		DirectX::XMLoadFloat3(&_lookAt),
		DirectX::XMLoadFloat3(&_up));
}

/******************************************************************************************************************/

DirectX::XMMATRIX CameraComponent::GetProjectionMatrix() const
{
	return DirectX::XMMatrixPerspectiveFovLH(_fieldOfView, _aspectRatio, _nearClipPlane, _farClipPlane);
}