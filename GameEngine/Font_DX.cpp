#ifdef BUILD_DIRECTX
#include "Font_DX.h"
#include "Renderer_DX.h"
#include "Window.h"

Font_DX::Font_DX()
{
}

Font_DX::~Font_DX()
{
}

bool Font_DX::Load(Renderer* renderer, const wchar_t* fileName)
{
	InitialiseShaders(renderer);

	return true;
}

void Font_DX::Draw(Renderer* renderer)
{
}

void Font_DX::InitialiseShaders(Renderer* renderer)
{
}
#endif