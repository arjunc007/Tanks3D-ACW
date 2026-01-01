#include "ATBComponent.h"
#include "GameObject.h"
#include "Window.h"
#ifdef BUILD_DIRECTX
#include "Renderer_DX.h"
#endif


ATBComponent::ATBComponent(GameObject* parent)
	: GameObjectComponent("ATB", parent)
{

}


ATBComponent::~ATBComponent()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void ATBComponent::Start()
{
#ifdef BUILD_DIRECTX
	TwInit(TW_DIRECT3D11, static_cast<const Renderer_DX*>(Window::TheWindow->GetRenderer())->GetDevice());
#endif
	TwDefine(" GLOBAL fontSize=3 ");
	TwDefine(" GLOBAL fontResizable=true ");
	TwDefine(" GLOBAL iconPos=1 ");
	TwWindowSize(Window::TheWindow->_width / 2, Window::TheWindow->_height / 2);

	myBar = TwNewBar(_parent->GetType().c_str());
}

/******************************************************************************************************************/

// Main update function (called every frame)
void ATBComponent::Update(double deltaTime)
{


}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void ATBComponent::OnMessage(Message* msg)
{
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void ATBComponent::End()
{
}

/******************************************************************************************************************/

void ATBComponent::Draw()
{
	TwDraw();
}