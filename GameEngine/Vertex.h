#pragma once
#include "DirectXMath.h"

struct Vertex
{
	float _position[3];
	float _texture[2];
	float _normal[3];
	float _tangent[3];
	float _binormal[3];
};

struct TextVertex {
	TextVertex(float r, float g, float b, float a, float u, float v, float tw, float th, float x, float y, float w, float h) : _color(r, g, b, a), _texCoord(u, v, tw, th), _pos(x, y, w, h) {}
	DirectX::XMFLOAT4 _pos;
	DirectX::XMFLOAT4 _texCoord;
	DirectX::XMFLOAT4 _color;
};