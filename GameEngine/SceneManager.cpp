#include "SceneManager.h"
#include "Scene.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

SceneManager::SceneManager(Game* game)
	: _game(game)
{
}

/******************************************************************************************************************/

SceneManager::~SceneManager()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void SceneManager::AddGameObject(GameObject* obj)
{
	Scene* currentScene = GetCurrentScene();
	if (currentScene)
	{
		currentScene->AddGameObject(obj);
	}
}

/******************************************************************************************************************/

const std::vector<GameObject*>& SceneManager::GetGameObjects() const
{
	Scene* currentScene = GetCurrentScene();
	if (currentScene)
	{
		return currentScene->GetGameObjects();
	}
	return std::vector<GameObject*>();
}

/******************************************************************************************************************/

void SceneManager::OnKeyboard(int key, bool down)
{
	Scene* currentScene = GetCurrentScene();
	if (currentScene)
	{
		currentScene->OnKeyboard(key, down);
	}
}

/******************************************************************************************************************/

void SceneManager::OnClick(int x, int y, bool down)
{
	Scene* currentScene = GetCurrentScene();
	if (currentScene)
	{
		currentScene->OnClick(x, y, down);
	}
}

void SceneManager::OnMessage(Message* msg)
{
	Scene* currentScene = GetCurrentScene();
	if (currentScene)
	{
		currentScene->OnMessage(msg);
	}
}

/******************************************************************************************************************/

/// Update current scene
void SceneManager::Update(double deltaTime)
{
	Scene* currentScene = GetCurrentScene();
	if (currentScene)
	{
		currentScene->Update(deltaTime);
	}
}

/******************************************************************************************************************/

/// Render current scene
void SceneManager::Render(RenderSystem* renderer)
{
	Scene* currentScene = GetCurrentScene();
	if (currentScene)
	{
		currentScene->Render(renderer);
	}
}

/******************************************************************************************************************/

void SceneManager::PushScene(Scene* s)
{
	_scenes.push(s);
	s->SetSceneManager(this);
	s->Initialise();
}

/******************************************************************************************************************/
