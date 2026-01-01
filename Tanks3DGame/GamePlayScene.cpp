#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Game.h"
#include "Material.h"
#include "RenderSystem.h"
#include "Message.h"
#include "GameOverScene.h"
#include "PauseScene.h"
#include "Logger.h"

//Gameobjects
#include "Ground.h"
#include "Arena.h"
#include "Obstacle.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

GamePlayScene::GamePlayScene()
	: _online(false),
	_restartCounter(0),
	_frequency(0)
{

}

/******************************************************************************************************************/

GamePlayScene::~GamePlayScene()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void GamePlayScene::Initialise()
{
	////////////////
	// Setup Objects
	_tank = new Tank(_sceneManager->GetGame()->GetMesh("tank"), _sceneManager->GetGame()->GetMaterial("tank"));
	_tank->SetPosition(DirectX::XMFLOAT3(0.0f, 0.6f, 0.0f));
	_tank->SetScale(0.5f);
	_gameObjects.push_back(_tank);

	Material* groundMat = _sceneManager->GetGame()->GetMaterial("ground");
	groundMat->SetData(5.f, 5.f);
	Ground* ground = new Ground(_sceneManager->GetGame()->GetMesh("ground"), groundMat);
	ground->SetScale(100.0f);
	_gameObjects.push_back(ground);

	Arena* arena = new Arena(_sceneManager->GetGame()->GetMesh("arena"), _sceneManager->GetGame()->GetMaterial("arena"));
	arena->SetPosition(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
	arena->SetScale(DirectX::XMFLOAT3(4.5f, 1.0f, 4.5f));
	_gameObjects.push_back(arena);

	for (int i = 0; i < 5; i++)
	{
		Obstacle* obstacle = new Obstacle(_sceneManager->GetGame()->GetMesh("obstacle"), _sceneManager->GetGame()->GetMaterial("obstacle"));
		obstacle->SetPosition(DirectX::XMFLOAT3(5.0f * i + 7.0f, 1.3f, 10.0f * i));
		obstacle->SetRotation(0.0f, 30.0f * i, 0.0f);
		_gameObjects.push_back(obstacle);
	}
	_camera = new Camera();
	_camera->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f));
	_gameObjects.push_back(_camera);

	_antTweakBar = new ATB();
	_gameObjects.push_back(_antTweakBar);

	

	// Start all objects to set them up
	for (int i = 0; i < (int)_gameObjects.size(); i++)
	{
		_gameObjects[i]->Start();
	}

	//Need to start bar before adding variables
	_antTweakBar->AddIntVar("Render Freq", &_frequency, "in Hz");
}

/******************************************************************************************************************/

void GamePlayScene::OnKeyboard(int key, bool down)
{
	if (down) return; // Ignore key down events

					  // Switch key presses
	switch (key)
	{
	//case 74: // J = Find and join existing game
	//	if (_online)
	//	{
	//		_threadList.push_back(new std::thread(&NetworkSystem::Find, &_networkSystem, 9172));
	//	}
	case 80: // P = pause
		//_sceneManager->PushScene(new PauseScene(_scoreDisplay->GetScore()));
		break;
	case 82: // R
		Reset();
		break;
	case 78: // N = open for networking
		if (!_online)
		{
			_networkSystem.Initialise(9172);//UDP listening port
			_online = true;
		}
		break;
	case 85: // U
		break;
	case 27: // Escape
		_sceneManager->PopScene();
		break;
	}
}

///Respond to clicks
void GamePlayScene::OnClick(int x, int y, bool down)
{
	if (down) return;	//Ignore mouse down events
}

/******************************************************************************************************************/

/// Update current scene
void GamePlayScene::Update(double deltaTime)
{
	Scene::Update(deltaTime);
	_frequency = static_cast<int>(1.0 / _sceneManager->GetGame()->GetTimer()->DeltaTime());

	_physicsSystem.Process(_gameObjects, deltaTime);
	_collisionSystem.Process(_gameObjects, deltaTime);

	if(_online)
		_networkSystem.Process(_gameObjects, deltaTime);

	//Follow camera
	DirectX::XMVECTOR cameraOffset = DirectX::XMLoadFloat3(&_tank->GetPosition());
	const auto forward = _tank->GetForward();
	DirectX::XMVECTOR tankPos = DirectX::XMLoadFloat3(&forward);
	tankPos = DirectX::XMVectorScale(tankPos, -24.0f);
	cameraOffset = DirectX::XMVectorAdd(cameraOffset, tankPos);
	cameraOffset = DirectX::XMVectorAdd(cameraOffset, DirectX::XMVectorSet(0.0f, 15.0f, 0.0f, 0.0f));
	_camera->SetPosition(DirectX::XMFLOAT3(DirectX::XMVectorGetX(cameraOffset), DirectX::XMVectorGetY(cameraOffset), DirectX::XMVectorGetZ(cameraOffset)));
	_camera->SetLookAt(_tank->GetPosition());
}

/******************************************************************************************************************/

/// Render current scene
void GamePlayScene::Render(RenderSystem* renderer)
{
	renderer->Process(_gameObjects, 0, _camera->GetViewMatrix(), _camera->GetProjectionMatrix());
	
	_antTweakBar->Draw();
}

/******************************************************************************************************************/

void GamePlayScene::Reset()
{
	for (int i = 0; i < (int)_gameObjects.size(); i++)
	{
		// Reset
		_gameObjects[i]->Reset();
	}
}

/******************************************************************************************************************/
