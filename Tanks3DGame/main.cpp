#include "Tanks3DGame.h"
static const int		SCREEN_WIDTH = 1920;
static const int		SCREEN_HEIGHT = 1080;

#if BUILD_DIRECTX

/**********************************/
// http://www.directxtutorial.com //
/**********************************/

// include the basic windows header files and the Direct3D header files
#define _WINSOCKAPI_
#include <windows.h>
#include <d3d11.h>

#include "Window_DX.h"

// include the Direct3D Library files
#pragma comment(lib, "d3d11.lib")

//Link with game engine library
#pragma comment(lib, "GameEngine.lib")

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// Create the Game object
	Tanks3DGame game;

	// Create a Window object
	Window_DX application(&game, SCREEN_WIDTH, SCREEN_HEIGHT, hInstance, nCmdShow);

	application.Initialise();
}

#endif