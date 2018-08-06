#pragma once
#include "Tanks3DGame.h"
#include "Components.h"
#include "Message.h"
#include "MainMenuScene.h"
#include "GamePlayScene.h"
#include "Font.h"
#include <sstream>
#include "Logger.h"

/******************************************************************************************************************/

Tanks3DGame::Tanks3DGame()
{
}

Tanks3DGame::~Tanks3DGame()
{
}

void Tanks3DGame::Initialise(Window* w)
{
	//Initialise parent
	Game::Initialise(w);

	LoadResources();

	// Create VBOs
	for (MeshMapIterator i = _meshes.begin();
		i != _meshes.end();
		++i)
	{
		i->second->CreateVBO(_renderer);
	}

	_sceneManager.PushScene(new MainMenuScene());
	//_sceneManager.PushScene(new GamePlayScene());

	//_gameState = Playing;
}

void Tanks3DGame::OnKeyboard(int key, bool down)
{
	Game::OnKeyboard(key, down);

	_sceneManager.OnKeyboard(key, down);
}

void Tanks3DGame::OnClick(int x, int y, bool down)
{
	Game::OnClick(x, y, down);
	_sceneManager.OnClick(x, y, down);
}

void Tanks3DGame::Render()
{
	//Clear screen
	_renderer->ClearScreen();

	_sceneManager.Render(&_renderSystem);

	_renderer->SwapBuffers();
}

void Tanks3DGame::Run()
{
	//Run parent method to get delta time etc
	Game::Run();

	//Update scenes
	_sceneManager.Update(_timer->DeltaTime());

	//Check for exit
	if (_sceneManager.GetCurrentScene() == nullptr)
	{
		SetQuitFlag(true);
	}

	//Draw
	Render();
}