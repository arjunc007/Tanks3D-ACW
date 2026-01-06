#pragma once
#include "GameObject.h"

class ImGuiDebug :
    public GameObject
{
	//Structors
public:
	ImGuiDebug();
	~ImGuiDebug();

public:
	void Draw();

private:
	//void OnGUI();
};

