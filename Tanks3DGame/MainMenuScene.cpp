#include "MainMenuScene.h"
#include "SceneManager.h"
#include "Game.h"
#include "RenderSystem.h"
#include "GamePlayScene.h"
#include "GameOverScene.h"
#include "Text.h"
#include <cmath>
//#include "Font_DX.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

MainMenuScene::MainMenuScene()
	//: _title(NULL)
{

}

/******************************************************************************************************************/

MainMenuScene::~MainMenuScene()
{
	//delete _title;

}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void MainMenuScene::Initialise()
{


	//_text = new Font_DX();
	//_title = new WordDisplay();
	//_title->SetPosition(Vector4(-0.45f, 0.1f, 0));
	//_title->SetColour(Colour::Yellow());
	//_title->SetWord("ASTEROIDS");

	// Create asteroids
	//for (int i = 0; i < 50; i++)
	//{
	//	Asteroid* asteroid = new Asteroid(_sceneManager->GetGame()->GetMesh("asteroid"));
	//	asteroid->Reset();
	//	asteroid->SetScale(2.0f * (rand() * 1.0f / RAND_MAX) + 0.5f);
	//	_gameObjects.push_back(asteroid);
	//}

	//// Start all objects to set them up
	//for (int i = 0; i < (int)_gameObjects.size(); i++)
	//{
	//	_gameObjects[i]->Start();
	//}
}

/******************************************************************************************************************/

void MainMenuScene::OnKeyboard(int key, bool down)
{
	if (down) return; // Ignore key down events

					  // Switch key presses
	switch (key)
	{
	case 27: // Escape
		_sceneManager->PopScene(); // Leave menu
		break;
	default: // Any other key
		_sceneManager->PushScene(new GamePlayScene()); // Play game
	}
}

void MainMenuScene::OnClick(int x, int y, bool down)
{
	if (down) return;	//Ignore mouse down events
}

/******************************************************************************************************************/

/// Update current scene
void MainMenuScene::Update(double deltaTime)
{
	Scene::Update(deltaTime);
	//_physicsSystem.Process(_gameObjects, deltaTime);
}

/******************************************************************************************************************/

/// Render current scene
void MainMenuScene::Render(RenderSystem* renderer)
{
	DirectX::XMMATRIX MVM = DirectX::XMMatrixIdentity();

	//renderer->Process(_gameObjects, 0);
	//_text->Load(renderer->GetRenderer(), L"..\\Assets\\Textures\\myfile.spritefont");
	//_text->Draw(renderer->GetRenderer());
	//_title->Render(renderer->GetRenderer(), MVM);
	//Text txt(_sceneManager->GetGame()->GetFont("CaptureIt"));
	//txt.SetText("Hello World!");
	//txt.Draw(renderer->GetRenderer());
}

/******************************************************************************************************************/
