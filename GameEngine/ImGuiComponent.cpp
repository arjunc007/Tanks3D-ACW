#include "ImGuiComponent.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "Window.h"
#ifdef BUILD_DIRECTX
#include "Renderer_DX.h"
#endif

ImGuiComponent::ImGuiComponent(GameObject* parent): 
	GameObjectComponent("UIDebug", parent)
{
}

ImGuiComponent::~ImGuiComponent()
{
}

void ImGuiComponent::Start()
{
	ImGui_ImplWin32_EnableDpiAwareness();
	float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = main_scale;

#ifdef BUILD_DIRECTX
	const Renderer_DX* renderer = static_cast<const Renderer_DX*>(Window::TheWindow->GetRenderer());
#endif
	ImGui_ImplWin32_Init(Window::TheWindow->GetHandle());
	ImGui_ImplDX11_Init(renderer->GetDevice(), renderer->GetContext());
}

void ImGuiComponent::Update(double deltaTime)
{
}

void ImGuiComponent::OnMessage(Message* msg)
{
}

void ImGuiComponent::End()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiComponent::Draw()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	{
		ImGui::Begin("Debug Window"); // Create a window called "Debug Window"

		static float clearColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };

		// Add a text label
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		// Add a slider for a variable
		// This directly modifies the 'clearColor' array! No callbacks needed.
		ImGui::ColorEdit3("Background Color", clearColor);

		// Add a slider for your Ground Tiling (Connects to your Material system)
		// ImGui::SliderFloat("Ground Tiling", &myMaterialData.Tiling.x, 1.0f, 100.0f);

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}