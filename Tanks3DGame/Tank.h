#pragma once
#include "GameObject.h"

class Mesh;
class Material;
class Message;

class Tank :
	public GameObject
{
	//Constants
public:
	static const float MAX_SPEED;
	static const float ACCELERATION;

	//Data
protected:

	//Structors
public:
	Tank(Mesh* mesh, Material* material);
	virtual ~Tank();

	//Functions
public:
	virtual void Update(double deltaTime);
	virtual void OnMessage(Message* msg);
	virtual void Reset();
};

