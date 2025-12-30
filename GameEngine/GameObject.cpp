#include "GameObject.h"
#include "GameObjectComponent.h"
#include "Message.h"
#include "RenderComponent.h"
#include "DirectXMath.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

GameObject::GameObject(std::string type)
	: _scale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)),
	_position(DirectX::XMFLOAT3()),
	_rotation(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)),
	_alive(true),
	_type(type)
{
}

/******************************************************************************************************************/

GameObject::~GameObject()
{
	End();
}

/******************************************************************************************************************/
// Component Functions
/******************************************************************************************************************/

bool GameObject::AddComponent(GameObjectComponent* goc)
{
	if (_components.find(goc->GetComponentType()) != _components.end())
	{
		// Error - already have this component!
		return false;
	}
	_components[goc->GetComponentType()] = goc;

	return true;
}

/******************************************************************************************************************/

bool GameObject::RemoveComponent(GameObjectComponent* goc)
{
	return RemoveComponent(goc->GetComponentType());
}

/******************************************************************************************************************/

bool GameObject::RemoveComponent(std::string type)
{
	ComponentMapIterator i = _components.find(type);
	if (i != _components.end())
	{
		// Remove it
		_components.erase(i);

		// Delete it
		i->second->End();
		delete i->second;

		return true;
	}

	// Couldn't find it
	return false;
}

/******************************************************************************************************************/

GameObjectComponent* GameObject::GetComponent(std::string type)
{
	ComponentMapIterator i = _components.find(type);
	if (i != _components.end())
	{
		// Return it
		return i->second;
	}

	// Couldn't find it
	return nullptr;
}

/******************************************************************************************************************/
// General Functions
/******************************************************************************************************************/

// Setup function -- called to initialise object and its components
void GameObject::Start()
{
	// Initialise all objects
	for (ComponentMapIterator i = _components.begin();
		i != _components.end();
		++i)
	{
		i->second->Start();
	}
}

/******************************************************************************************************************/

// Main update function (called every frame)
void GameObject::Update(double deltaTime)
{
	// Update all objects
	for (ComponentMapIterator i = _components.begin();
		i != _components.end();
		++i)
	{
		i->second->Update(deltaTime);
	}
}

/******************************************************************************************************************/

// Shutdown function -- called when object is destroyed (i.e., from destructor)
void GameObject::End()
{
	// End all objects
	for (ComponentMapIterator i = _components.begin();
		i != _components.end();
		++i)
	{
		GameObjectComponent* component = i->second;
		component->End();
		delete component;
	}

	// Clear list
	_components.clear();
}

/******************************************************************************************************************/

// Resets the game object to the start state (similar to Start(), but may be called more than once)
void GameObject::Reset()
{

}

/******************************************************************************************************************/

DirectX::XMFLOAT3 GameObject::GetForward() const
{
	DirectX::XMVECTOR currentRotation = DirectX::XMLoadFloat4(&_rotation);
	DirectX::XMVECTOR f = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, -1, 1), currentRotation);
	f = DirectX::XMVector3Normalize(f);
	
	return DirectX::XMFLOAT3(DirectX::XMVectorGetX(f), DirectX::XMVectorGetY(f), DirectX::XMVectorGetZ(f));
}

/******************************************************************************************************************/

void GameObject::SetRotation(float pitch, float yaw, float roll)
{
	DirectX::XMVECTOR rotation = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(pitch), DirectX::XMConvertToRadians(yaw), DirectX::XMConvertToRadians(roll));

	DirectX::XMStoreFloat4(&_rotation, rotation);
}

void GameObject::SetTextureScale(float texScale)
{
	RenderComponent* rc = static_cast<RenderComponent*>(GetComponent("render"));

	if (rc)
	{
		rc->SetTextureScale(texScale);
	}
}