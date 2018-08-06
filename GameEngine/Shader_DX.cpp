#include "Shader_DX.h"

#if BUILD_DIRECTX
#include "Renderer_DX.h"
#include <d3dcompiler.h>


Shader_DX::Shader_DX()
	: _vertexShader(nullptr),
	_domainShader(nullptr),
	_hullShader(nullptr),
	_geometryShader(nullptr),
	_pixelShader(nullptr),
	_layout(nullptr),
	_blendState(nullptr),
	_depthStencilView(nullptr)
{

}


Shader_DX::~Shader_DX()
{
	_vertexShader->Release();
	_pixelShader->Release();
	_hullShader->Release();
	_domainShader->Release();
	_geometryShader->Release();
	_layout->Release();
	_blendState->Release();
	_depthStencilView->Release();
}

bool Shader_DX::LoadVertexShader(Renderer* renderer, const wchar_t* fileName)
{
	HRESULT result;
	Renderer_DX* renderer_DX = static_cast<Renderer_DX*>(renderer);

	// load and compile the shader
	ID3DBlob *VS, *error;
	result = D3DCompileFromFile(fileName, nullptr, nullptr, "VShader", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &VS, &error);
	result = renderer_DX->GetDevice()->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &_vertexShader);

	return(!FAILED(result));
}

bool Shader_DX::LoadDomainShader(Renderer* renderer, const wchar_t* fileName)
{
	HRESULT result;

	// load and compile the shader
	ID3DBlob *DS, *error;
	result = D3DCompileFromFile(fileName, nullptr, nullptr, "DShader", "ds_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &DS, &error);
	result = static_cast<Renderer_DX*>(renderer)->GetDevice()->CreateDomainShader(DS->GetBufferPointer(), DS->GetBufferSize(), nullptr, &_domainShader);

	return(!FAILED(result));
}

bool Shader_DX::LoadHullShader(Renderer* renderer, const wchar_t* fileName)
{
	HRESULT result;

	// load and compile the shader
	ID3DBlob *HS, *error;
	result = D3DCompileFromFile(fileName, nullptr, nullptr, "HShader", "hs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &HS, &error);
	result = static_cast<Renderer_DX*>(renderer)->GetDevice()->CreateHullShader(HS->GetBufferPointer(), HS->GetBufferSize(), nullptr, &_hullShader);

	return(!FAILED(result));
}

bool Shader_DX::LoadGeometryShader(Renderer* renderer, const wchar_t* fileName)
{
	HRESULT result;

	// load and compile the shader
	ID3DBlob *GS, *error;
	result = D3DCompileFromFile(fileName, nullptr, nullptr, "GShader", "gs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &GS, &error);
	result = static_cast<Renderer_DX*>(renderer)->GetDevice()->CreateGeometryShader(GS->GetBufferPointer(), GS->GetBufferSize(), nullptr, &_geometryShader);

	return(!FAILED(result));
}

bool Shader_DX::LoadPixelShader(Renderer* renderer, const wchar_t* fileName)
{
	HRESULT result;

	// load and compile the shader
	ID3DBlob *PS, *error;
	result = D3DCompileFromFile(fileName, nullptr, nullptr, "PShader", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &PS, &error);
	result = static_cast<Renderer_DX*>(renderer)->GetDevice()->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &_pixelShader);

	return(!FAILED(result));
}

void Shader_DX::Draw(Renderer* renderer)
{
	ID3D11DeviceContext* context = static_cast<Renderer_DX*>(renderer)->GetContext();

	context->VSSetShader(_vertexShader, nullptr, 0);
	context->DSSetShader(_domainShader, nullptr, 0);
	context->HSSetShader(_hullShader, nullptr, 0);
	context->GSSetShader(_geometryShader, nullptr, 0);
	context->PSSetShader(_pixelShader, nullptr, 0);
}

#endif