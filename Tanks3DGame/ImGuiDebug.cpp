#include "ImGuiDebug.h"
#include "Components.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

void OnGUI();

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
	imc->Draw(OnGUI);
}

void OnGUI()
{
	ImGui::Begin("Debug Window"); // Create a window called "Debug Window"

	static float clearColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };

	//ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("FPS (%.1f FPS)", ImGui::GetIO().Framerate);

	static float tilingScale = 5.f;
	ImGui::SliderFloat("Tiling", &tilingScale, 0.f, 100.f);

	ImGui::ColorEdit3("Background Color", clearColor);

	// ImGui::SliderFloat("Ground Tiling", &myMaterialData.Tiling.x, 1.0f, 100.0f);

	ImGui::End();
}