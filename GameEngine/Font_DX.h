#pragma once
#include "Font.h"

#ifdef BUILD_DIRECTX
#include <d3d11.h>

class Renderer;

class Font_DX
	: public Font
{
	//Data
protected:

public:
	Font_DX();
	virtual ~Font_DX();

	//Functions
public:
	virtual bool Load(Renderer* renderer, const wchar_t* fileName);
	virtual void Draw(Renderer* renderer);

	//Extra functions
private:
	void InitialiseShaders(Renderer* renderer);
};
#endif

