#pragma once
#include "GameObject.h"
#include "DirectXMath.h"

class Camera
	: public GameObject
{
	//Structors
public:
	Camera();
	~Camera();

	//Functions
public:
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();
	void SetLookAt(DirectX::XMFLOAT3 lookAt);
};
