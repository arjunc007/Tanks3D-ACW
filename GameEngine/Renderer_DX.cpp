#if BUILD_DIRECTX
#include <d3dcompiler.h>
#include "Renderer_DX.h"
#include "Shader_DX.h"
#include "Mesh.h"
#include "Font.h"
#include "Material.h"
#include "GameObject.h"

/******************************************************************************************************************/

Renderer_DX::Renderer_DX(HWND hWnd)
	: _hWnd(hWnd)
{
}

/******************************************************************************************************************/

Renderer_DX::~Renderer_DX()
{
}

/******************************************************************************************************************/

void Renderer_DX::ClearScreen()
{
	float colour[4] = { _clearColour.x, _clearColour.y, _clearColour.z, _clearColour.w };
	_context->ClearRenderTargetView(_backbuffer, colour);

	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/******************************************************************************************************************/

void Renderer_DX::Destroy()
{
	_swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

	// close and release all existing COM objects
	_layout->Release();
	_vertexShader->Release();
	_pixelShader->Release();
	_swapchain->Release();
	_backbuffer->Release();
	_device->Release();
	_context->Release();
	_depthStencilView->Release();

	if (_uniformBuffer)	
	{
		_uniformBuffer->Release();
	}
}

/******************************************************************************************************************/

void Renderer_DX::Draw(const Mesh* mesh, const Material* material, const DirectX::XMMATRIX &modelMatrix, const DirectX::XMMATRIX &viewMatrix, const DirectX::XMMATRIX &projectionMatrix)
{
	//Set Input Layout
	_context->IASetInputLayout(_layout);

	//Set the shaders
	if (material)
	{
		if (material->GetShader())
		{
			material->GetShader()->Draw(this);
		}
		else
		{
			//Default shaders
			_context->VSSetShader(_vertexShader, nullptr, 0);
			_context->PSSetShader(_pixelShader, nullptr, 0);
		}
	}

	UniformBuffer uniforms;
	uniforms.modelMatrix = DirectX::XMMatrixTranspose(modelMatrix);
	uniforms.viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	uniforms.projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	DirectX::XMFLOAT4 eyePos;
	DirectX::XMStoreFloat4(&eyePos, DirectX::XMVectorNegate(viewMatrix.r[3]));

	CameraBuffer cameraBuffer;
	cameraBuffer.eyePos = eyePos;

	//Set sampler state in the pixel shader
	_context->PSSetSamplers(0, 1, &_samplerState);

	// Need to update uniform buffer here
	D3D11_MAPPED_SUBRESOURCE ms;
	_context->Map(_uniformBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);		// map the buffer
	memcpy(ms.pData, &uniforms, sizeof(UniformBuffer));								// copy the data
	_context->Unmap(_uniformBuffer, NULL);											// unmap the buffer

	_context->VSSetConstantBuffers(0, 1, &_uniformBuffer);
	_context->PSSetConstantBuffers(0, 1, &_uniformBuffer);

	_context->Map(_cameraBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, &cameraBuffer, sizeof(CameraBuffer));
	_context->Unmap(_cameraBuffer, NULL);

	_context->VSSetConstantBuffers(1, 1, &_cameraBuffer);

	if(material)
		material->GetTexture()->Draw(this);

	mesh->GetVBO()->Draw(this);
}

/******************************************************************************************************************/

void Renderer_DX::Initialise(int width, int height)
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//Create swap chain description
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = width; // use window's client area dims
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = _hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&sd,
		&_swapchain,
		&_device,
		NULL,
		&_context);
	
	//Create a render Target View
	ID3D11Texture2D* backBuffer = nullptr;
	hr = _swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (FAILED(hr))
		return;

	hr = _device->CreateRenderTargetView(backBuffer, nullptr, &_backbuffer);
	backBuffer->Release();
	if (FAILED(hr))
		return;	

	//Create depth stencil texture
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = _device->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
	if (FAILED(hr))
		return;

	//Create depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = depthStencilDesc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = _device->CreateDepthStencilView(_depthStencilBuffer, &descDSV, &_depthStencilView);
	if (FAILED(hr))
		return;

	//Bind views to output merger
	_context->OMSetRenderTargets(1, &_backbuffer, _depthStencilView);

	//Setup the viewport
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	_context->RSSetViewports(1, &vp);

	// Create the projection matrix for 3D rendering.
	_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, static_cast<float>(width) / static_cast<float>(height), 0.001f, 1000.0f);

	// Initialise shaders
	InitialiseShaders();
}

/******************************************************************************************************************/

void Renderer_DX::SwapBuffers()
{
	_swapchain->Present(0, 0);
}

/******************************************************************************************************************/

// Load and prepare shaders
void Renderer_DX::InitialiseShaders()
{
	HRESULT result;
	// load and compile the two shaders
	ID3DBlob *VS, *PS, *error;
	result = D3DCompileFromFile(L"shaders.hlsl", nullptr, nullptr, "VShader", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &VS, &error);
	result = D3DCompileFromFile(L"shaders.hlsl", nullptr, nullptr, "PShader", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &PS, &error);

	// encapsulate both shaders into shader objects
	result = _device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &_vertexShader);

	result = _device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &_pixelShader);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int numElements = ARRAYSIZE(ied);

	result = _device->CreateInputLayout(ied, numElements, VS->GetBufferPointer(), VS->GetBufferSize(), &_layout);

	//Create texture sampler state description
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create texture sampler state
	_device->CreateSamplerState(&samplerDesc, &_samplerState);

	// Create uniform buffer
	UniformBuffer uniforms;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(uniforms);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &uniforms;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer.
	_device->CreateBuffer(&cbDesc, &InitData, &_uniformBuffer);

	//Camera buffer
	CameraBuffer camera;

	cbDesc.ByteWidth = sizeof(camera);
	InitData.pSysMem = &camera;

	_device->CreateBuffer(&cbDesc, &InitData, &_cameraBuffer);
}

/******************************************************************************************************************/

void Renderer_DX::ResizeBuffers(int width, int height)
{
	if (_swapchain)
	{
		_context->OMSetRenderTargets(0, 0, 0);

		//Release all outstanding references to the swap chain's buffers
		_backbuffer->Release();

		HRESULT hr;
		// Preserve the existing buffer count and format.
		// Automatically choose the width and height to match the client rect for HWNDs.
		hr = _swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		// Perform error handling here!

		// Get buffer and create a render-target-view.
		// get the address of the back buffer
		ID3D11Texture2D *p_backbuffer;
		_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&p_backbuffer);
		// Perform error handling here!

		hr = _device->CreateRenderTargetView(p_backbuffer, NULL,
			&_backbuffer);
		// Perform error handling here!
		p_backbuffer->Release();

		//Create depth stencil texture
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		hr = _device->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
		if (FAILED(hr))
			return;

		//Create depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthStencilDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = _device->CreateDepthStencilView(_depthStencilBuffer, &descDSV, &_depthStencilView);
		if (FAILED(hr))
			return;

		_context->OMSetRenderTargets(1, &_backbuffer, _depthStencilView);

		// Set up the viewport.
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<float>(width);
		vp.Height = static_cast<float>(height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		_context->RSSetViewports(1, &vp);
	}
}

/******************************************************************************************************************/

#endif