#pragma once
#include "DirectXMath.h"
#include <vector>
#include "Vertex.h"

class Font;
class Renderer;
class Material;
class VBO;

class Text
{
	//Data
	bool _locked;	// True once we've made the VBO; can no longer add vertices etc unless reset
	VBO* _vbo;
	Font* _font;
	std::vector<TextVertex> _vertices;
	DirectX::XMFLOAT4 _color;
	DirectX::XMFLOAT2 _pos;

	//Structors
public:
	Text(Font* font);
	~Text();

	//Functions
public:
	void SetText(const char* text);
	void SetFont(Font* font) { _font = font; }
	void SetColour(DirectX::XMFLOAT4 c) { _color = c; }
	void SetPosition(DirectX::XMFLOAT2 pos) { _pos = pos; }
	void Draw(Renderer* renderer);
};

