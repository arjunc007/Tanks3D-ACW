#pragma once

class Renderer;

//Abstract Texture Class
class Texture
{
	//Structors
public:
	Texture();
	virtual ~Texture();

	virtual bool Load(Renderer* renderer, const wchar_t* fileName) = 0;

	virtual void Draw(Renderer* renderer) = 0;
};

