#include "PauseScene.h"
#include "SceneManager.h"
#include "Game.h"
#include "RenderSystem.h"
#include "GamePlayScene.h"
#include <cmath>

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

PauseScene::PauseScene(int score)
	:// _title(NULL),
	_score(score)
{

}

/******************************************************************************************************************/

PauseScene::~PauseScene()
{
//	delete _title;

}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void PauseScene::Initialise()
{
	//_title = new WordDisplay();
	//_title->SetPosition(Vector4(-0.45f, 0.2f, 0));
	//_title->SetColour(Colour::Blue());
	//_title->SetWord(" PAUSED ");

	//// Setup Score Display
	//_scoreDisplay = new ScoreDisplay();
	//_scoreDisplay->SetPosition(Vector4(-0.15f, -0.2f, 0));
	//_scoreDisplay->SetScore(_score);
	//_gameObjects.push_back(_scoreDisplay);

	// Start all objects to set them up
	for (int i = 0; i < (int)_gameObjects.size(); i++)
	{
		_gameObjects[i]->Start();
	}
}

/******************************************************************************************************************/

void PauseScene::OnKeyboard(int key, bool down)
{
	if (down) return; // Ignore key down events

					  // Switch key presses
	switch (key)
	{
	case 27: // Escape
		_sceneManager->PopScene(); // Leave menu
		break;
	case 13: // Enter
		_sceneManager->PopScene(); // Leave menu
		break;
	case 80: // P = pause
		_sceneManager->PopScene();
		break;
	}
}

void PauseScene::OnClick(int x, int y, bool down)
{
	if (down) return;	//Ignore mouse down events
}

/******************************************************************************************************************/

/// Update current scene
void PauseScene::Update(double deltaTime)
{
	Scene::Update(deltaTime);
	_physicsSystem.Process(_gameObjects, deltaTime);

	for (int i = 0; i < (int)_gameObjects.size(); i++)
	{
		if (_gameObjects[i]->IsAlive())
		{
			_gameObjects[i]->Update(deltaTime);
		}
	}
}

/******************************************************************************************************************/

/// Render current scene
void PauseScene::Render(RenderSystem* renderer)
{
	renderer->Process(_gameObjects, 0);

	//_title->Render(renderer->GetRenderer(), renderer->GetCamera());
}

/******************************************************************************************************************/
