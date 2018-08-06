#pragma once
#include "System.h"
#include "Renderer.h"
#include <vector>

class RenderComponent;

// Moves objects around based on PhysicsComponents
class RenderSystem :
	public System
{
	// Data
protected:
	Renderer * _renderer;

	// Structors
public:
	RenderSystem();
	virtual ~RenderSystem();

	// Get / sets
public:
	Renderer * GetRenderer()		const { return _renderer; }
	void SetRenderer(Renderer* r) { _renderer = r; }

	// Functions
public:
	virtual void Process(std::vector<GameObject*>& list, double deltaTime, const DirectX::XMMATRIX &viewMatrix, const DirectX::XMMATRIX &projectionMatrix);
	virtual void Process(std::vector<GameObject*>& list, double deltaTime) {}
};

