#include "Scene.h"
#include "GameObject.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

Scene::Scene()
{
}

/******************************************************************************************************************/

Scene::~Scene()
{
	for (size_t i = 0; i < _gameObjects.size(); i++)
	{
		delete _gameObjects[i];
	}
	_gameObjects.clear();
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void Scene::OnMessage(Message* msg)
{
	for (int i = 0; i < (int)_gameObjects.size(); i++)
	{
		if (_gameObjects[i]->IsAlive())
		{
			_gameObjects[i]->OnMessage(msg);
		}
	}
}

/******************************************************************************************************************/

void Scene::Update(double deltaTime)
{
	// Delete game objects
	for (int i = 0; i < (int)_gameObjects.size(); i++)
	{
		if (_gameObjects[i]->IsAlive())
		{
			_gameObjects[i]->Update(deltaTime);
		}

		if (_gameObjects[i]->ShouldBeDeleted())
		{
			delete _gameObjects[i];
			_gameObjects.erase(_gameObjects.begin() + i);
			i--;
		}
	}
}

/******************************************************************************************************************/
