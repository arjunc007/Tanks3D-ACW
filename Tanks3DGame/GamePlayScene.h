#pragma once
#include "Scene.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include "NetworkSystem.h"
#include <thread>

// Game classes
#include "Camera.h"
#include "Tank.h"
#include "ATB.h"
#include "ImGuiDebug.h"

class GamePlayScene :
	public Scene
{
	// Constants
public:
													
	// Data
protected:
	PhysicsSystem						_physicsSystem;
	CollisionSystem						_collisionSystem;
	NetworkSystem						_networkSystem;		// To handle networking
	bool								_online;			// Networking flag
	std::vector<std::thread*>			_threadList;

	Camera*								_camera;
	Tank*								_tank;
	ATB*								_antTweakBar;
	ImGuiDebug*							_debugUI;
	//ScoreDisplay*						_scoreDisplay;

	double								_restartCounter;
	int									_frequency;

	// Structors
public:
	GamePlayScene();
	virtual ~GamePlayScene();


	// Gets/Sets
public:
	// Connection flag
	bool GetOnline()						const { return _online; }
	void SetOnline(bool v) { _online = v; }

	// Functions
public:

	// Setup
	virtual void Initialise();

	/// Respond to input
	virtual void OnKeyboard(int key, bool down);

	///Respond to clicks
	virtual void OnClick(int x, int y, bool down);

	/// Update current scene
	virtual void Update(double deltaTime);

	/// Render current scene
	virtual void Render(RenderSystem* renderer);


	// Reset the game
	void Reset();
};
