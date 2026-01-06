#pragma once
#include "Game.h"

//STL
#include <vector>

//Engine classes
#include "Window.h"
#include "Renderer.h"
#include "Font.h"
//#include "ResourceLoader.h"

// Simple game state enum
enum GameState
{
	MainMenu,
	Playing,
	Paused,
	Dead,
	Resetting,
	GameOver
};

class Tanks3DGame :
	public Game
{
	//Constants and static
public:

	//Data
private:
	double _renderTime = 0.0;
	double _targetFrameRate = 1 / 60.0;

	//Structors
public:
	Tanks3DGame();
	virtual ~Tanks3DGame();

	//Functions
public:

	//Initialise the game
	void Initialise(Window* w);

	//Keyboard input
	virtual void OnKeyboard(int key, bool down);

	//Mouse clicks
	virtual void OnClick(int x, int y, bool down);
	
	//Render everything
	virtual void Render();

	//Main game loop
	virtual void Run();
};

