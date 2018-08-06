#pragma once
#if BUILD_DIRECTX
#include <d3d11.h>

#include "VBO.h"

// DirectX specific VBO class
class VBO_DX :
	public VBO
{
	// Data
protected:
	ID3D11Buffer* _vbo;

	// Structors
public:
	VBO_DX();
	virtual ~VBO_DX();

	// Functions
public:
	virtual void Create(const Renderer* renderer, Vertex vertices[], int numVertices);
	virtual void Create(const Renderer* renderer, TextVertex vertices[], int numVertices);
	virtual void Draw(Renderer* renderer);
	virtual void DrawInstanced(Renderer* renderer);
};

#endif