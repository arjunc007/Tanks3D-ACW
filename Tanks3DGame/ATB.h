#pragma once
#include "GameObject.h"

class Renderer;

class ATB
	: public GameObject
{
	//Data
private:

	//Structors
public:
	ATB();
	~ATB();

	void AddDoubleVar(const char* name, double* var, const char* desc);
	void AddIntVar(const char* name, int* var, const char* desc);
	void Draw();
};

