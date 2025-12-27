#pragma once
#include "GameObjectComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"

class RenderComponent
	: public GameObjectComponent
{
	// Data
protected:
	Mesh *			_mesh;			// Vertex info
	Material *		_material;		// Material of object
	bool			_shouldDraw;	// Whether or not to draw

									// RenderComponent
public:
	RenderComponent(GameObject* gob);
	virtual ~RenderComponent();


	// Gets/Sets
public:
	Mesh * GetMesh()							const { return _mesh; }
	void SetMesh(Mesh* m) { _mesh = m; }

	Material * GetMaterial()						const { return _material; }
	void SetMaterial(Material* m) { _material = m; }
	void SetColour(DirectX::XMFLOAT4 c) { _material->SetColor(c); }

	bool ShouldDraw()						const { return _shouldDraw; }
	void ShouldDraw(bool v) { _shouldDraw = v; }

	// Get position/scale/angle etc from parent object
	const DirectX::XMFLOAT3& GetScale()						const { return _parent->GetScale(); }
	const DirectX::XMFLOAT3& GetPosition()					const { return _parent->GetPosition(); }
	const DirectX::XMFLOAT4& GetRotation()					const { return _parent->GetRotation(); }


	// Functions
public:

	// Setup function -- called when parent object is initialised (using its own Start method)
	virtual void Start();

	// Main update function (called every frame)
	virtual void Update(double deltaTime);

	// Message handler (called when message occurs)
	virtual void OnMessage(Message* msg);

	// Shutdown function -- called when parent object is destroyed
	virtual void End();

};