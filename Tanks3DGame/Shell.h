#pragma once
#include "GameObject.h"
#include "Renderer.h"

class Shell :
	public GameObject
{
	//Constants
private:
	static const float SHELL_SPEED;
	static const float MAX_LIFE;

	//Data
private:

	//Structors
public:
	Shell(Mesh* mesh, Material* material);
	virtual ~Shell();

	//Functions
public:
	virtual void Update(double deltaTime);
	virtual void OnMessage(Message* msg);
	virtual void Reset();

	//Extra functions
public:
	void Spawn(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 rotation);
};

