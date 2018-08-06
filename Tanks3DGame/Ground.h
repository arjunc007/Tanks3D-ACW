#pragma once
#include "GameObject.h"

class Mesh;
class Material;
class Message;

class Ground :
	public GameObject
{
	//Structors
public:
	Ground(Mesh* mesh, Material* material);
	virtual ~Ground();

	//Functions
public:
	virtual void Update(double deltaTime);
	virtual void OnMessage(Message* msg);
	virtual void Reset();
};

