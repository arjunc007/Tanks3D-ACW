#include "Camera.h"
#include "Components.h"

Camera::Camera()
	: GameObject("camera")
{
	CameraComponent * cc = new CameraComponent(this);
}

Camera::~Camera()
{
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	CameraComponent* cc = (CameraComponent*)GetComponent("camera");
	return cc->GetViewMatrix();
}

DirectX::XMMATRIX Camera::GetProjectionMatrix()
{
	CameraComponent* cc = (CameraComponent*)GetComponent("camera");
	return cc->GetProjectionMatrix();
}

void Camera::SetLookAt(DirectX::XMFLOAT3 lookAt)
{
	CameraComponent* cc = (CameraComponent*)GetComponent("camera");
	cc->SetLookAt(lookAt);
}