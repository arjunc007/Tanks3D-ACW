#pragma once
#include "GameObjectComponent.h"

class ImGuiComponent :
    public GameObjectComponent
{
public:
    ImGuiComponent(GameObject* parent);
    ~ImGuiComponent();

	// Functions
public:
	virtual void Start() override;

	virtual void Update(double deltaTime) override;

	virtual void OnMessage(Message* msg) override;

	virtual void End() override;

public:
	void Draw();
};

