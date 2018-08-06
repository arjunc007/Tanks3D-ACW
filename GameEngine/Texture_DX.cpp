#if BUILD_DIRECTX
#include "Texture_DX.h"
#include "Renderer_DX.h"
#include "DDSTextureLoader.h"

#if _DEBUG
#include "Logger.h"
#endif

Texture_DX::Texture_DX()
	:
	_texture(nullptr),
	_textureView(nullptr)
{
}


Texture_DX::~Texture_DX()
{
	_texture->Release();
	_textureView->Release();
}

bool Texture_DX::Load(Renderer* renderer, const wchar_t* fileName)
{
	Renderer_DX* rendererDX = static_cast<Renderer_DX*>(renderer);

	HRESULT hr = DirectX::CreateDDSTextureFromFile(rendererDX->GetDevice(), fileName, nullptr, &_textureView);
	
	return(!FAILED(hr));
}

void Texture_DX::Draw(Renderer* renderer)
{
	Renderer_DX* rendererDX = (Renderer_DX*)renderer;
	rendererDX->GetContext()->PSSetShaderResources(0, 1, &_textureView);
}
#endif // BUILD_DIRECTX