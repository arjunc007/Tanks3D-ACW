#pragma once
#include "Vertex.h"

class Renderer;

// Abstract VBO class
class VBO
{
	// Data
protected:
	int _numVertices;
	int _numInstances;

	// Constructors
public:
	VBO();
	virtual ~VBO();

	// Functions
public:
	virtual void Create(const Renderer* renderer, Vertex vertices[], int numVertices) = 0;
	virtual void Create(const Renderer* renderer, TextVertex vertices[], int numVertices) = 0;
	virtual void Draw(Renderer* renderer) = 0;
	virtual void DrawInstanced(Renderer* renderer) = 0;
};

