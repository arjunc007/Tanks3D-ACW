#pragma once
#include "GameObject.h"

class Mesh;
class Material;
class Message;

class Arena
	: public GameObject
{
public:
	Arena(Mesh* mesh, Material* material);
	~Arena();

	//Functions
public:
	virtual void Update(double deltaTime);
	virtual void OnMessage(Message* msg);
	virtual void Reset();
};

