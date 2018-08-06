#pragma once
#if BUILD_DIRECTX
#include <d3d11.h>
#include "Texture.h"

class Texture_DX :
	public Texture
{
	//Data
protected:
	ID3D11Texture2D * _texture;
	ID3D11ShaderResourceView* _textureView;

public:
	Texture_DX();
	~Texture_DX();

	bool Load(Renderer* renderer, const wchar_t* fileName);
	void Draw(Renderer* renderer);
};
#endif //BUILD_DIRECTX

