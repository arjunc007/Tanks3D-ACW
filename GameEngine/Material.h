#pragma once
#include "Texture.h"
#include "Shader.h"
#include "DirectXMath.h"

struct MaterialData
{
	MaterialData(): tiling(1.f, 1.f), offset(0.f, 0.f)
	{ }

	DirectX::XMFLOAT2 tiling;
	DirectX::XMFLOAT2 offset;
};

class Material
{
	//Data
protected:
	DirectX::XMFLOAT4 _color;
	MaterialData _data;
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

	const MaterialData& GetData() const { return _data; }

	void SetData(const DirectX::XMFLOAT2& tiling, const DirectX::XMFLOAT2& offset);
	void SetData(float tileX, float tileY, float offsetX = 0.f, float offsetY = 0.f);

	bool LoadVertexShader(Renderer* renderer, const wchar_t* fileName);
	bool LoadDomainShader(Renderer* renderer, const wchar_t* fileName);
	bool LoadHullShader(Renderer* renderer, const wchar_t* fileName);
	bool LoadGeometryShader(Renderer* renderer, const wchar_t* fileName);
	bool LoadPixelShader(Renderer* renderer, const wchar_t* fileName);

};

