#pragma once
#include "Texture.h"
#include "Shader.h"
#include "DirectXMath.h"

class Material
{
	//Data
protected:
	DirectX::XMFLOAT4 _color;
	Texture* _texture;
	Shader* _shader;

	//Structors
public:
	Material();
	Material(float, float, float, float);
	virtual ~Material();

	//Gets/Sets
public:
	void SetColor(DirectX::XMFLOAT4 c) { _color = c; }

	bool LoadTexture(Renderer* renderer, const wchar_t* fileName);

	Texture* GetTexture() const { return _texture; }

	Shader* GetShader() const { return _shader; }

	bool LoadVertexShader(Renderer* renderer, const wchar_t* fileName);
	bool LoadDomainShader(Renderer* renderer, const wchar_t* fileName);
	bool LoadHullShader(Renderer* renderer, const wchar_t* fileName);
	bool LoadGeometryShader(Renderer* renderer, const wchar_t* fileName);
	bool LoadPixelShader(Renderer* renderer, const wchar_t* fileName);

};

