#pragma once
#include <string>
//DirectX Math
#include <DirectXMath.h>

// Forward declarations
class Game;
class RenderComponent;
class Mesh;
class Font;
class Material;
class Camera;

// Platform independent renderer base class
// Basically represents a graphics context and its active shaders
class Renderer
{
	// Data
protected:
	DirectX::XMFLOAT4 _clearColour;

	// Structors
public:
	Renderer();
	virtual ~Renderer();

	// Gets/sets
public:
	DirectX::XMFLOAT4 GetClearColour()		const { return _clearColour; }
	void SetClearColour(DirectX::XMFLOAT4 c) { _clearColour = c; }
	void SetClearColour(float r, float g, float b, float a) { _clearColour = DirectX::XMFLOAT4(r, g, b, a); }

	// Functions
public:
	virtual void ClearScreen() = 0;

	virtual void Draw(const Mesh* mesh, const Material* material, const DirectX::XMMATRIX &modelMatrix, const DirectX::XMMATRIX &viewMatrix, const DirectX::XMMATRIX &projectionMatrix) = 0;
	//virtual void Draw(const Font* font, std::wstring text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT2 padding, DirectX::XMFLOAT4 color) = 0;
	virtual void Draw(RenderComponent* rc, const DirectX::XMMATRIX &viewMatrix, const DirectX::XMMATRIX &projectionMatrix);
	
	virtual void Destroy() = 0;
	
	virtual void Initialise(int width, int height) = 0;
	
	virtual void SwapBuffers() = 0;
	
	virtual void ResizeBuffers(int width, int height) = 0;
	
};

