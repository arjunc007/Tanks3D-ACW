#pragma once

class Renderer;

class Shader
{
	//Structors
public:
	Shader();
	virtual ~Shader();

	//Funtions
public:
	virtual bool LoadVertexShader(Renderer* renderer, const wchar_t* fileName) = 0;
	virtual bool LoadDomainShader(Renderer* renderer, const wchar_t* fileName) = 0;
	virtual bool LoadHullShader(Renderer* renderer, const wchar_t* fileName) = 0;
	virtual bool LoadGeometryShader(Renderer* renderer, const wchar_t* fileName) = 0;
	virtual bool LoadPixelShader(Renderer* renderer, const wchar_t* fileName) = 0;

	virtual void Draw(Renderer* renderer) = 0;
};

