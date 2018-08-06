#pragma once
#include "GameObject.h"

class Mesh;
class Material;
class Message;

class Obstacle
	: public GameObject
{
	DirectX::XMFLOAT3 _initialPos;
public:
	Obstacle(Mesh* mesh, Material* material);
	~Obstacle();

	//Functions
public:
	virtual void Update(double deltaTime);
	virtual void OnMessage(Message* msg);
	virtual void Reset();
};

