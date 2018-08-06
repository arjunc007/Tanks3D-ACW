/***************************************************/
// DX code based on http://www.directxtutorial.com //
/***************************************************/

#pragma once
#if BUILD_DIRECTX
#include "Window.h"

// DirectX window
class Window_DX :
	public Window
{
	// Data
private:

	HWND					_hWnd;

	// Structors
public:
	// Constructor
	Window_DX(Game* game, int width, int height, HINSTANCE hInstance, int nCmdShow);

	// Destructor
	virtual ~Window_DX();


	// Functions
public:

	// Windows Message loop
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	// Initialise window
	virtual void Initialise();

	HWND GetHandle() const { return _hWnd; }
};


#endif