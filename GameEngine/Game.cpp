#include "Game.h"
#include "Window.h"
#include "Mesh.h"
#include "Material.h"
#include "Font.h"
#include "GameObject.h"
#include "KeyPressMessage.h"
#include "MouseClickMessage.h"
#include "SceneManager.h"
#include "Logger.h"
#include "TCPSocket.h"

#include <fstream>
#include <string>

Game* Game::TheGame = nullptr;

/******************************************************************************************************************/

Game::Game()
	: _quitFlag(false),
	_sceneManager(this)
{
	for (int i = 0; i < 256; i++) _keyStates[i] = false;

	_timer = new Timer();

	TheGame = this;
}

/******************************************************************************************************************/

Game::~Game()
{
	for (MeshMapIterator i = _meshes.begin();
		i != _meshes.end();
		++i)
	{
		delete i->second;
	}
	_meshes.clear();

	delete _timer;
}

/******************************************************************************************************************/

void Game::Initialise(Window* w)
{
	_window = w;
	_renderer = w->GetRenderer();

	_renderSystem.SetRenderer(_renderer);

	_timer->Start();
}

/******************************************************************************************************************/

void Game::OnKeyboard(int key, bool down)
{
	_keyStates[key] = down;

	// Create keypress message and send it to all objects
	KeyPressMessage msg(key, down);

	BroadcastMessage(&msg);
}

void Game::OnClick(int x, int y, bool down)
{
	_clickLocation[0] = x;
	_clickLocation[1] = y;
	
	//Create mouseclick message and send to all objects
	MouseClickMessage msg(x, y, down);
	BroadcastMessage(&msg);
}

/******************************************************************************************************************/

void Game::Run()
{
	// Get delta time
	_timer->Tick();
}

/******************************************************************************************************************/

void Game::BroadcastMessage(Message* msg)
{
	ListenToMessage(msg);

	_sceneManager.OnMessage(msg);
}

/******************************************************************************************************************/

void Game::LoadResources()
{
	std::ifstream fin("Resources.txt");

	std::string tmp, path, label;
	int startpos;

	if (fin.good())
	{
		//Start reading file till Meshes list
		do
		{
			fin >> tmp;
		}while (tmp.compare("Meshes"));

		//Read meshes
		while (1)
		{
			fin >> tmp; //label="something"
			if (!tmp.compare("END"))
				break;

			startpos = tmp.find("\"") + 1;
			label = tmp.substr(startpos, tmp.size() - startpos - 1);
			fin >> tmp; // path="something else"
			startpos = tmp.find("\"") + 1;
			path = tmp.substr(startpos, tmp.size() - startpos - 1);
			Mesh* mesh = new Mesh();
			if(mesh->LoadFromFile(path))
				AddMesh(label, mesh);
			else
			{
				delete mesh;
				Log("Failed to read mesh " + label);
			}
		}

		//Read file till Materials list
		do
		{
			fin >> tmp;
		} while (tmp.compare("Materials"));

		while (1)
		{
			fin >> tmp; //label="something"
			if (!tmp.compare("END"))
				break;

			startpos = tmp.find("\"") + 1;
			label = tmp.substr(startpos, tmp.size() - startpos - 1);

			fin >> tmp; // path="something else"
			startpos = tmp.find("\"") + 1;
			path = tmp.substr(startpos, tmp.size() - startpos - 1);
			std::wstring wPath(path.begin(), path.end());
			
			fin >> tmp;	//PS=""
			startpos = tmp.find("\"") + 1;
			path = tmp.substr(startpos, tmp.size() - startpos - 1);
			std::wstring wPS(path.begin(), path.end());

			fin >> tmp;	//VS=""
			startpos = tmp.find("\"") + 1;
			path = tmp.substr(startpos, tmp.size() - startpos - 1);
			std::wstring wVS(path.begin(), path.end());

			Material* material = new Material();
			if (!material->LoadTexture(_renderer, wPath.c_str()))
			{
				delete material;
				Log("Failed to read material " + label);
				return;
			}
			if(!wPS.empty())
				material->LoadPixelShader(_renderer, wPS.c_str());
			if (!wVS.empty())
				material->LoadVertexShader(_renderer, wVS.c_str());
			
			AddMaterial(label, material);
		}

		//Read file till Fonts list
		do
		{
			fin >> tmp;
		} while (tmp.compare("Fonts"));

		//Load Fonts
		while (1)
		{
			fin >> tmp; //label="something"
			if (!tmp.compare("END"))
				break;

			startpos = tmp.find("\"") + 1;
			label = tmp.substr(startpos, tmp.size() - startpos - 1);

			fin >> tmp; //path=""
			startpos = tmp.find("\"") + 1;
			path = tmp.substr(startpos, tmp.size() - startpos - 1);
			std::wstring wPath(path.begin(), path.end());

			fin >> tmp; //desc=""
			startpos = tmp.find("\"") + 1;
			path = tmp.substr(startpos, tmp.size() - startpos - 1);
			std::wstring wDesc(path.begin(), path.end());

			Font* font = new Font();
			if (font->Load(_renderer, wDesc.c_str(), wPath.c_str()))
				AddFont(label, font);
		}
		
	}
	else
	{
		Log("Resource file failed to load");
	}
}