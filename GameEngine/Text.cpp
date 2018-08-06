#include "Text.h"
#include "Material.h"
#include "Font.h"
#include "Window.h"
#if BUILD_DIRECTX
#include "VBO_DX.h"
#endif


Text::Text(Font* font)
	: _font(font),
	_color(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f))
{
}


Text::~Text()
{
	delete _vbo;
	_vbo = nullptr;
}

void Text::SetText(const char* sentence)
{
	_locked = true;
	Renderer* renderer = Window::TheWindow->_renderer;

	_font->BuildVertexArray(&_vertices, sentence, _pos, _color);

#if BUILD_DIRECTX
	_vbo = new VBO_DX();
#else
	_vbo = new VBO_GL();
#endif

	_vbo->Create(renderer, _vertices.data(), _vertices.size());

}

void Text::Draw(Renderer* renderer)
{
	_vbo->DrawInstanced(renderer);

}