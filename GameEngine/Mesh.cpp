#include "Mesh.h"
#if BUILD_DIRECTX
#include "VBO_DX.h"
#endif
#if BUILD_OPENGL
#include "VBO_GL.h"
#endif
#include "DirectXMath.h"
#include "Game.h"
#include <iostream>
#include <fstream>
using namespace std;

/******************************************************************************************************************/

Mesh::Mesh()
	: _locked(false),
	_vbo(nullptr)

{
}

/******************************************************************************************************************/

Mesh::~Mesh()
{
	delete _vbo;
	_vbo = nullptr;
}

/******************************************************************************************************************/

bool Mesh::AddVertex(Vertex v)
{
	if (!_locked)
	{
		_vertices.push_back(v);
		return true;
	}
	else
	{
		return false;
	}
}

/******************************************************************************************************************/

bool Mesh::DeleteVertex(int i)
{
	if (!_locked)
	{
		_vertices.erase(_vertices.begin() + i);
		return true;
	}
	else
	{
		return false;
	}
}

/******************************************************************************************************************/

bool Mesh::Clear()
{
	if (!_locked)
	{
		_vertices.clear();
		return true;
	}
	return false;
}

/******************************************************************************************************************/

void Mesh::Reset()
{
	delete _vbo;
	_vbo = NULL;
	_locked = false;
}

/******************************************************************************************************************/

VBO* Mesh::CreateVBO(Renderer* renderer)
{
	_locked = true;

#if BUILD_DIRECTX
	_vbo = new VBO_DX();
#else
	_vbo = new VBO_GL();
#endif

	_vbo->Create(renderer, _vertices.data(), NumVertices());

	return _vbo;
}

/******************************************************************************************************************/

float Mesh::CalculateMaxSize() const
{
	float length_x = (_bounds.max.x - _bounds.min.x)/2;
	float length_z = (_bounds.max.z - _bounds.min.z)/2;

	//Return diagonal length on xz plane
	return sqrt(length_x *  length_x + length_z * length_z);
}

/******************************************************************************************************************/

bool Mesh::LoadFromFile(std::string filename)
{
	ifstream fin(filename);
	
	if (fin.fail())
		return false;

	char input = ' ';

	fin.get(input);

	while (input != ':')
	{
		fin.get(input);
	}

	int count;

	fin >> count;

	Vertex vertex;

	fin.get(input);

	while (input != ':')
	{
		fin.get(input);
	}

	fin.get(input);
	fin.get(input);

	float x, y, z;

	float minX, maxX, minY, maxY, minZ, maxZ;

	for (int i = 0; i < count; i++)
	{
		//Position
		fin >> x >> y >> z;
		vertex._position[0] = x;
		vertex._position[1] = y; 
		vertex._position[2] = z;

		//Find edges of the model to calculate bounding box
		if (i > 0)
		{
			if (x > maxX)
				maxX = x;
			else if (x < minX)
				minX = x;
			if (y > maxY)
				maxY = y;
			else if (y < minY)
				minY = y;
			if (z > maxZ)
				maxZ = z;
			else if (z < minZ)
				minZ = z;
		}
		else
		{
			minX = maxX = x;
			minY = maxY = y;
			minZ = maxZ = z;
		}

		//Texture
		fin >> x >> y;
		vertex._texture[0] = x;
		vertex._texture[1] = y;

		//Normal
		fin >> x >> y >> z;
		vertex._normal[0] = x;
		vertex._normal[1] = y;
		vertex._normal[2] = z;
		
		//Tangent
		fin >> x >> y >> z;
		
		vertex._tangent[0] = x;
		vertex._tangent[1] = y;
		vertex._tangent[2] = z;

		//Binormal
		fin >> x >> y >> z;
		vertex._binormal[0] = x;
		vertex._binormal[1] = y;
		vertex._binormal[2] = z;

		AddVertex(vertex);
	}

	_bounds.min = DirectX::XMFLOAT3(minX, minY, minZ);
	_bounds.max = DirectX::XMFLOAT3(maxX, maxY, maxZ);

	fin.close();

	return true;
}

/******************************************************************************************************************/

bool Mesh::LoadFromStream(std::ifstream& in)
{
	int numVertices;
	in >> numVertices;

	float minX, maxX, minY, maxY, minZ, maxZ;

	for (int i = 0; i < numVertices; i++)
	{
		Vertex v;

		//Position
		in >> v._position[0];
		in >> v._position[1];
		in >> v._position[2];

		//Find edges of the model to calculate bounding box
		if (i > 0)
		{
			if (v._position[0] > maxX)
				maxX = v._position[0];
			else if (v._position[0] < minX)
				minX = v._position[0];
			if (v._position[1] > maxY)
				maxY = v._position[1];
			else if (v._position[1] < minY)
				minY = v._position[1];
			if (v._position[2] > maxZ)
				maxZ = v._position[2];
			else if (v._position[2] < minZ)
				minZ = v._position[2];
		}
		else
		{
			minX = maxX = v._position[0];
			minY = maxY = v._position[1];
			minZ = maxZ = v._position[2];
		}

		//Texture
		in >> v._texture[0];
		in >> v._texture[1];

		//Normal
		in >> v._normal[0];
		in >> v._normal[1];
		in >> v._normal[2];

		//Tangent
		in >> v._tangent[0];
		in >> v._tangent[1];
		in >> v._tangent[2];

		//Binormal
		in >> v._binormal[0];
		in >> v._binormal[1];
		in >> v._binormal[2];

		AddVertex(v);
	}

	_bounds.min = DirectX::XMFLOAT3(minX, minY, minZ);
	_bounds.max = DirectX::XMFLOAT3(maxX, maxY, maxZ);

	return true;
}

/******************************************************************************************************************/

void Mesh::SetTextureTiling(float tileFactor)
{
	for (Vertex& v : _vertices)
	{
		v._texture[0] *= tileFactor;
		v._texture[1] *= tileFactor;
	}

	Reset();
	CreateVBO(Game::TheGame->GetRenderer());
}