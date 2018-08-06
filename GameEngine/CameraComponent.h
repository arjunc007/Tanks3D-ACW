#pragma once
#include "GameObjectComponent.h"
#include "DirectXMath.h"

class CameraComponent
	: public GameObjectComponent
{	
	//Data
protected:
	DirectX::XMFLOAT3 _lookAt;
	DirectX::XMFLOAT3 _up;
	float _fieldOfView;
	float _nearClipPlane;
	float _farClipPlane;
	float _aspectRatio;

	//Structors
public:
	CameraComponent(GameObject* parent);
	~CameraComponent();

	//Gets/Sets
public:
	DirectX::XMFLOAT3 GetLookAt()	const { return _lookAt; }
	void SetLookAt(DirectX::XMFLOAT3 lookAt) { _lookAt = lookAt; }

	DirectX::XMFLOAT3 GetUpVector() const { return _up; }
	void SetUpVector(DirectX::XMFLOAT3 up) { _up = up; }

	float GetFoV() const { return _fieldOfView; }
	void SetFoV(float fov) { _fieldOfView = fov; }

	float GetNearClippingPlane() const { return _nearClipPlane; }
	void SetNearClippingPlane(float n) { _nearClipPlane = n; }

	float GetFarClippingPlane() const { return _farClipPlane; }
	void SetFarClippingPlane(float f) { _farClipPlane = f; }

	float GetAspectRatio() const { return _aspectRatio; }
	void SetAspectRatio(float r) { _aspectRatio = r; }

	// Functions
public:

	// Setup function -- called when parent object is initialised (using its own Start method)
	virtual void Start();

	// Main update function (called every frame)
	virtual void Update(double deltaTime);

	// Message handler (called when message occurs)
	virtual void OnMessage(Message* msg);

	// Shutdown function -- called when parent object is destroyed
	virtual void End();

	//Extra Functions
public:
	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix() const;
};

