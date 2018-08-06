#pragma once
// Network message value
#define WM_SOCKET WM_USER + 1
#define _WINSOCKAPI_
// include Windows and Direct3D headers
#include <windows.h>

// Forward declarations
class Game;
class Renderer;

// Generic window class
// Handles window setup and input
class Window
{
	// Constants and statics
public:
	// Singleton (sort of)
	static Window*							TheWindow;

	// Data
public:
	Game * _game;
	Renderer*								_renderer;
	int										_width, _height;


	// Structors
public:
	Window(Game* game, int width, int height);
	virtual ~Window();


	// Gets/sets
public:
	Game * GetGame()					const { return _game; }
	Renderer* GetRenderer()			const { return _renderer; }


	// Functions
public:
	virtual void Initialise() = 0;
	virtual HWND GetHandle() const = 0;
};

