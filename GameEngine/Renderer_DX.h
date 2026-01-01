#pragma once
#if BUILD_DIRECTX

#include <d3d11.h>
#include <DirectXMath.h>
#include "Renderer.h"

struct UniformBuffer
{
	DirectX::XMMATRIX modelMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
};

struct MaterialBuffer
{
	MaterialBuffer() : tiling(1.f, 1.f), offset(0.f, 0.f)
	{}
	DirectX::XMFLOAT2 tiling;
	DirectX::XMFLOAT2 offset;
};

struct CameraBuffer
{
	DirectX::XMFLOAT4 eyePos;
};

// DirectX Device & Context
class Renderer_DX :
	public Renderer
{
	// Data
protected:
	DirectX::XMMATRIX		_projectionMatrix;		// the projection matrix

	IDXGISwapChain*			_swapchain;             // the pointer to the swap chain interface
	ID3D11Device*			_device;                // the pointer to our Direct3D device interface
	ID3D11DeviceContext*	_context;				// the pointer to our Direct3D device context
	ID3D11RenderTargetView* _backbuffer;			// the pointer to our back buffer
	ID3D11Texture2D*		_depthStencilBuffer;	// the pointer to our depth stencil buffer
	ID3D11DepthStencilView*	_depthStencilView;		// the pointer to our depth stencil view
	ID3D11InputLayout*		_layout;				// the pointer to the input layout
	ID3D11VertexShader*		_vertexShader;			// the pointer to the vertex shader
	ID3D11PixelShader*		_pixelShader;			// the pointer to the pixel shader
	ID3D11Buffer*			_uniformBuffer;			// Stores the model, view and projection matrices
	ID3D11Buffer*			_materialBuffer;		// Stores the material data
	ID3D11Buffer*			_cameraBuffer;			// Stores the camera position
	ID3D11SamplerState*		_samplerState;			//Store the sampler state

	HWND					_hWnd;					// Window handle

	// Structors
public:
	Renderer_DX(HWND hWnd);
	virtual ~Renderer_DX();

	// Gets/Sets
public:
	ID3D11Device* GetDevice()					const	{ return _device; }
	void SetDevice(ID3D11Device* v)						{ _device = v; }

	ID3D11DeviceContext* GetContext()			const	{ return _context; }
	void SetContext(ID3D11DeviceContext* v)				{ _context = v; }

	// Functions
public:
	void ClearScreen();
	void Destroy();
	void Draw(const Mesh* mesh, const Material* material, const DirectX::XMMATRIX& modelMatrix, const DirectX::XMMATRIX &viewMatrix, const DirectX::XMMATRIX &projectionMatrix);
	//void Draw(const Font* font, std::wstring text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT2 padding, DirectX::XMFLOAT4 color);
	void Initialise(int width, int height);
	void SwapBuffers();
	void ResizeBuffers(int width, int height);

	// Initialise the shaders
	void InitialiseShaders();

};

#endif