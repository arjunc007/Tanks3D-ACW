#include "TankControllerComponent.h"
#include "GameObject.h"
#include "KeyPressMessage.h"
#include "MouseClickMessage.h"
#include "ThrustMessage.h"
#include "DirectXMath.h"
#include "Game.h"
#include "Tank.h"
#include "Components.h"

#ifdef _DEBUG
#include "Logger.h"
#endif

const float TankControllerComponent::ACCELERATION = 20.0f;
const unsigned int TankControllerComponent::NUM_SHELLS = 2;
const float TankControllerComponent::FIRE_DELAY = 1.0f;

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

TankControllerComponent::TankControllerComponent(GameObject* gob)
	: UserInputComponent(gob),
	_turnLeftPressed(false),
	_turnRightPressed(false),
	_acceleratePressed(false),
	_reversePressed(false),
	_fireDelay(FIRE_DELAY)
{
	// Create shells
	for (int i = 0; i < NUM_SHELLS; i++)
	{
		_shells.push_back(new Shell(Game::TheGame->GetMesh("shell"), Game::TheGame->GetMaterial("shell")));
		_shells.back()->SetAlive(false);
		_shells.back()->SetScale(0.15f);
		Game::TheGame->AddGameObject(_shells.back());
	}
}


TankControllerComponent::~TankControllerComponent()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void TankControllerComponent::Start()
{
	_parent->RegisterListener("keypress", this);
	_parent->RegisterListener("mouseclick", this);
//#ifdef _DEBUG
//	Log("Register keypress listener for tankcontroller");
//#endif
}

/******************************************************************************************************************/

// Main update function (called every frame)
void TankControllerComponent::Update(double deltaTime)
{
	if (_turnLeftPressed)	//Left
	{
		if (!(_acceleratePressed || _reversePressed))
			TurnLeft(static_cast<float>(90.0f * deltaTime));
	}
	if (_turnRightPressed)	// Right
	{
		if (!(_acceleratePressed || _reversePressed))
			TurnRight(static_cast<float>(90.0f * deltaTime));
	}
	if (_acceleratePressed)	// Up
	{
		if (!(_turnRightPressed || _turnLeftPressed))
			Accelerate(static_cast<float>(ACCELERATION * deltaTime));
	}
	if (_reversePressed)		//Down
	{
		if(!(_turnRightPressed ||_turnLeftPressed))
			Reverse(static_cast<float>(ACCELERATION * deltaTime));
	}

	if (_fireDelay > 0.0f)
		_fireDelay -= deltaTime;
}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void TankControllerComponent::OnMessage(Message* msg)
{
	if (msg->GetMessageType() == "mouseclick")
	{
		MouseClickMessage* mcm = (MouseClickMessage*)msg;
		//if(!mcm->GetDown())
			Fire();
	}

	if (msg->GetMessageType() == "keypress")
	{
		KeyPressMessage* kpm = (KeyPressMessage*)msg;
		switch (kpm->GetKey())
		{
		case 32: //Spacebar
			Fire();
			break;
		case 65:	//A
		case 37: // Left
			_turnLeftPressed = kpm->GetDown();
			break;
		case 68:	//D
		case 39: // Right
			_turnRightPressed = kpm->GetDown();
			break;
		case 87:	//W
		case 38: // Up
			_acceleratePressed = kpm->GetDown();
			if (!_acceleratePressed)
			{
				Message msg("stop");
				_parent->OnMessage(&msg);
			}
			break;
		case 83:	//S
		case 40: //Down
			_reversePressed = kpm->GetDown();
			if (!_reversePressed)
			{
				Message msg("stop");
				_parent->OnMessage(&msg);
			}
			break;
		case 17: // Ctrl
		case 71: // G
			//if (kpm->GetDown() == false) // Only on key release
			//{
			//	Message msg("godmode");
			//	_parent->OnMessage(&msg);
			//}
			break;
		}
	}
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void TankControllerComponent::End()
{
	_parent->UnregisterListener("keypress", this);
	_parent->UnregisterListener("mouseclick", this);
}

/******************************************************************************************************************/

void TankControllerComponent::TurnLeft(float angle)
{
	DirectX::XMVECTOR currentRotation = DirectX::XMLoadFloat4(&_parent->GetRotation());
	DirectX::XMVECTOR diff = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 1), -DirectX::XMConvertToRadians(angle));
	currentRotation = DirectX::XMQuaternionMultiply(currentRotation, diff);

	_parent->SetRotation(DirectX::XMFLOAT4(DirectX::XMVectorGetX(currentRotation), DirectX::XMVectorGetY(currentRotation), DirectX::XMVectorGetZ(currentRotation), DirectX::XMVectorGetW(currentRotation)));
}

/******************************************************************************************************************/

void TankControllerComponent::TurnRight(float angle)
{
	DirectX::XMVECTOR currentRotation = DirectX::XMLoadFloat4(&_parent->GetRotation());
	DirectX::XMVECTOR diff = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 1), DirectX::XMConvertToRadians(angle));
	currentRotation = DirectX::XMQuaternionMultiply(currentRotation, diff);

	_parent->SetRotation(DirectX::XMFLOAT4(DirectX::XMVectorGetX(currentRotation), DirectX::XMVectorGetY(currentRotation), DirectX::XMVectorGetZ(currentRotation), DirectX::XMVectorGetW(currentRotation)));
}

/******************************************************************************************************************/

void TankControllerComponent::Accelerate(float amt)
{

	//f = forward * m_rotation
	DirectX::XMVECTOR currentRotation = DirectX::XMLoadFloat4(&_parent->GetRotation());
	DirectX::XMVECTOR f = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, -1, 1), currentRotation);
	f = DirectX::XMVectorScale(f, amt);

	DirectX::XMFLOAT3 thrust(DirectX::XMVectorGetX(f), DirectX::XMVectorGetY(f), DirectX::XMVectorGetZ(f));

	// Create thrust message and send to parent object
	ThrustMessage msg(thrust);
	_parent->OnMessage(&msg);
}

/******************************************************************************************************************/

void TankControllerComponent::Reverse(float amt)
{
	//f = forward * m_rotation
	DirectX::XMVECTOR currentRotation = DirectX::XMLoadFloat4(&_parent->GetRotation());
	DirectX::XMVECTOR f = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, -1, 1), currentRotation);
	f = DirectX::XMVectorScale(f, -amt);

	DirectX::XMFLOAT3 thrust(DirectX::XMVectorGetX(f), DirectX::XMVectorGetY(f), DirectX::XMVectorGetZ(f));

	// Create thrust message and send to parent object
	ThrustMessage msg(thrust);
	_parent->OnMessage(&msg);
}

/******************************************************************************************************************/

void TankControllerComponent::Fire()
{
	for (int i = 0; i < NUM_SHELLS; i++)
	{
		if (!_shells[i]->IsAlive() && _fireDelay <= 0.0f)
		{
			// Use this bullet
			_shells[i]->Spawn(_parent->GetPosition(), _parent->GetRotation());
			_fireDelay = FIRE_DELAY;
			return;
		}
	}
}

/******************************************************************************************************************/

void TankControllerComponent::Reset()
{
	_turnLeftPressed = false;
	_turnRightPressed = false;
	_acceleratePressed = false;
	_reversePressed = false;
}