//https://www.braynzarsoft.net/viewtutorial/q16390-11-drawing-text-in-directx-12

#pragma once
#include <string>
#include "Mesh.h"
#include "DirectXMath.h"
#include "Material.h"

class Font
{
	//structs
private:

	struct FontChar
	{
		// the unicode id
		int id;

		// these need to be converted to texture coordinates 
		// (where 0.0 is 0 and 1.0 is textureWidth of the font)
		float u; // u texture coordinate
		float v; // v texture coordinate
		float twidth; // width of character on texture
		float theight; // height of character on texture

		float width; // width of character in screen coords
		float height; // height of character in screen coords

					  // these need to be normalized based on size of font
		float xoffset; // offset from current cursor pos to left side of character
		float yoffset; // offset from top of line to top of character
		float xadvance; // how far to move to right for next character
	};

	struct FontKerning
	{
		int firstid; // the first character
		int secondid; // the second character
		float amount; // the amount to add/subtract to second characters x
	};

	//Data
private:
	std::wstring _name; //Name of the font
	std::wstring _fontImage;
	int _size; // size of font, lineheight and baseheight will be based on this as if this is a single unit (1.0)
	float _lineHeight; // how far to move down to next line, will be normalized
	float _baseHeight; // height of all characters, will be normalized
	int _textureWidth; // width of the font texture
	int _textureHeight; // height of the font texture
	int _numCharacters; // number of characters in the font
	FontChar* _CharList; // list of characters
	int _numKernings; // the number of kernings
	FontKerning* _KerningsList; // list to hold kerning values
	Material _material;

	// these are how much the character is padded in the texture. We
	// add padding to give sampling a little space so it does not accidentally
	// padd the surrounding characters. We will need to subtract these paddings
	// from the actual spacing between characters to remove the gaps you would otherwise see
	float _leftpadding;
	float _toppadding;
	float _rightpadding;
	float _bottompadding;

	//Structors
public:
	Font();
	virtual ~Font();

	// Functions
public:

	// Loads the Mesh from a file (returns true if loaded OK)
	virtual bool Load(Renderer* renderer, const wchar_t* fileName, const wchar_t* texture); // load a font

	// this will return the amount of kerning we need to use for two characters
	float GetKerning(wchar_t first, wchar_t second);

	// this will return a FontChar given a wide character
	FontChar* GetChar(wchar_t c);

	//Load fnt file
	void LoadFont(const wchar_t* filename, int windowWidth, int windowHeight);

	void BuildVertexArray(void* vertices, const char* sentence, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color);

	//virtual void RenderText(std::wstring text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT2 padding, DirectX::XMFLOAT4 color);
};

