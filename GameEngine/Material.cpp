#include "Material.h"
#if BUILD_DIRECTX
#include "Texture_DX.h"
#include "Shader_DX.h"
#endif

/******************************************************************************************************************/

Material::Material()
	:
	_color(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)),
	_texture(nullptr),
	_shader(nullptr)
{
}

/******************************************************************************************************************/

Material::Material(float r, float g, float b, float a) : _color(DirectX::XMFLOAT4(r, g, b, a))
{

}

/******************************************************************************************************************/

Material::~Material()
{
}

/******************************************************************************************************************/

bool Material::LoadTexture(Renderer* renderer, const wchar_t* fileName)
{
#if BUILD_DIRECTX
	_texture = new Texture_DX();
#endif
	return _texture->Load(renderer, fileName);
}

/******************************************************************************************************************/

bool Material::LoadVertexShader(Renderer* renderer, const wchar_t* fileName)
{
	if (!_shader)
	{
#if BUILD_DIRECTX
		_shader = new Shader_DX();
#endif
	}

	return _shader->LoadVertexShader(renderer, fileName);
}

/******************************************************************************************************************/

bool Material::LoadDomainShader(Renderer* renderer, const wchar_t* fileName)
{
	if (!_shader)
	{
#if BUILD_DIRECTX
		_shader = new Shader_DX();
#endif
	}

	return _shader->LoadDomainShader(renderer, fileName);
}

/******************************************************************************************************************/

bool Material::LoadHullShader(Renderer* renderer, const wchar_t* fileName)
{
	if (!_shader)
	{
#if BUILD_DIRECTX
		_shader = new Shader_DX();
#endif
	}

	return _shader->LoadHullShader(renderer, fileName);
}

/******************************************************************************************************************/

bool Material::LoadGeometryShader(Renderer* renderer, const wchar_t* fileName)
{
	if (!_shader)
	{
#if BUILD_DIRECTX
		_shader = new Shader_DX();
#endif
	}

	return _shader->LoadGeometryShader(renderer, fileName);
}

/******************************************************************************************************************/

bool Material::LoadPixelShader(Renderer* renderer, const wchar_t* fileName)
{
	if (!_shader)
	{
#if BUILD_DIRECTX
		_shader = new Shader_DX();
#endif
	}

	return _shader->LoadPixelShader(renderer, fileName);
}

/******************************************************************************************************************/