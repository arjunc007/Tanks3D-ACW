#pragma once
#include <ctime>
#include <map>
#include <vector>

// Systems
#include "PhysicsSystem.h"
#include "RenderSystem.h"
//#include "CollisionSystem.h"
#include "Timer.h"


#include "SceneManager.h"

// Forward declarations
class GameObject;
class Mesh;
class Material;
class Font;
class Message;
class Renderer;
class Window;

// Typedefs
typedef std::map<std::string, Mesh*> MeshMap;
typedef std::map<std::string, Mesh*>::iterator MeshMapIterator;
typedef std::map<std::string, Material*> MaterialMap;
typedef std::map<std::string, Material*>::iterator MaterialMapIterator;
typedef std::map<std::string, Font*> FontMap;
typedef std::map<std::string, Font*>::iterator FontMapIterator;

// Generic game class
class Game
{
	// "Singleton"
public:
	static Game* TheGame;

	// Data
protected:
	bool							_keyStates[256];	// Keyboard status
	int								_clickLocation[2];	// Mouse click coordinates
	bool							_quitFlag;			// Quit flag; when true, we quit
	Timer*							_timer;			// Time since last frame
	Renderer*						_renderer;			// The renderer
	Window*							_window;			// The game window

														// Meshes
	MeshMap							_meshes;			// The map of meshes

														// Materials
	MaterialMap						_materials;			// The map of materials

	FontMap							_fonts;				//The map of fonts

														// Systems
	RenderSystem					_renderSystem;		// To handle rendering

														// Scene Manager
	SceneManager					_sceneManager;

	// Structors
public:
	Game();
	virtual ~Game();


	// Gets/sets
public:
	//Timer
	Timer * GetTimer() const { return _timer; }

	// Meshes
	Mesh * GetMesh(std::string name);
	void AddMesh(std::string name, Mesh* mesh) { _meshes[name] = mesh; }

	// Materials
	Material * GetMaterial(std::string name);
	void AddMaterial(std::string name, Material* material) { _materials[name] = material; }

	//Fonts
	Font * GetFont(std::string name);
	void AddFont(std::string name, Font* font) { _fonts[name] = font; }

	// Quit flag
	bool GetQuitFlag()						const { return _quitFlag; }
	void SetQuitFlag(bool v) { _quitFlag = v; }

	// Renderer
	Renderer* GetRenderer()					const { return _renderer; }

	//Window
	Window* GetWindow()						const { return _window; }

	// Functions
public:
	void AddGameObject(GameObject* obj) { _sceneManager.AddGameObject(obj); }
	const std::vector<GameObject*>& GetGameObjects() const { return _sceneManager.GetGameObjects(); }

	// Initialise game
	virtual void Initialise(Window* w);

	// Keyboard input
	virtual void OnKeyboard(int key, bool down);

	//Mouse Click
	virtual void OnClick(int x, int y, bool down);

	// Draw everything
	virtual void Render() = 0;

	// Main game loop (update)
	virtual void Run();

	// Message system
	void BroadcastMessage(Message* msg);

	// The game can respond to messages too
	virtual void ListenToMessage(Message* msg) {}

	virtual void LoadResources();
};


inline Mesh* Game::GetMesh(std::string name)
{
	// Found
	MeshMapIterator i = _meshes.find(name);
	if (i != _meshes.end())
	{
		return i->second;
	}
	// Not found
	return nullptr;
}

inline Material* Game::GetMaterial(std::string name)
{
	// Found
	MaterialMapIterator i = _materials.find(name);
	if (i != _materials.end())
	{
		return i->second;
	}
	// Not found
	return nullptr;
}

inline Font* Game::GetFont(std::string name)
{
	// Found
	FontMapIterator i = _fonts.find(name);
	if (i != _fonts.end())
	{
		return i->second;
	}
	// Not found
	return nullptr;
}