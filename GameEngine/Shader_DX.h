#pragma once
#include "Shader.h"

#if BUILD_DIRECTX
#include <d3d11.h>

class Renderer;

class Shader_DX
	: public Shader
{
	//Data
protected:
	ID3D11VertexShader*		_vertexShader;			// the pointer to the vertex shader
	ID3D11DomainShader*		_domainShader;			// the pointer to the domain shader
	ID3D11HullShader*		_hullShader;			// the pointer to the hull shader
	ID3D11GeometryShader*	_geometryShader;		// the pointer to the geometry shader
	ID3D11PixelShader*		_pixelShader;			// the pointer to the pixel shader
	ID3D11InputLayout*		_layout;				// the pointer to input layout
	ID3D11BlendState*		_blendState;			// The pointer to the blend state
	ID3D11DepthStencilView* _depthStencilView;		// the pointer to the depth stencil view

	//Structors
public:
	Shader_DX();
	virtual ~Shader_DX();

	//Funtions
public:
	virtual bool LoadVertexShader(Renderer* renderer, const wchar_t* fileName);
	virtual bool LoadDomainShader(Renderer* renderer, const wchar_t* fileName);
	virtual bool LoadHullShader(Renderer* renderer, const wchar_t* fileName);
	virtual bool LoadGeometryShader(Renderer* renderer, const wchar_t* fileName);
	virtual bool LoadPixelShader(Renderer* renderer, const wchar_t* fileName);

	virtual void Draw(Renderer* renderer);

	//Extra functions
};

#endif