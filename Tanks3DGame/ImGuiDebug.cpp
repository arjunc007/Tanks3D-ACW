#include "ImGuiDebug.h"
#include "Components.h"

ImGuiDebug::ImGuiDebug() : GameObject("UIDebug")
{
	new ImGuiComponent(this);
}

ImGuiDebug::~ImGuiDebug()
{
}

void ImGuiDebug::Draw()
{
	ImGuiComponent* imc = static_cast<ImGuiComponent*>(GetComponent("UIDebug"));
	imc->Draw();
}
