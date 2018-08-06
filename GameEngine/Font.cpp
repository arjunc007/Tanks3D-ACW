#pragma once
#include <fstream>
#include "Font.h"
#include "Window.h"
#if BUILD_DIRECTX
#include "VBO_DX.h"
#endif

Font::Font()
{
}

/******************************************************************************************************************/

Font::~Font()
{
}

/******************************************************************************************************************/

void Font::BuildVertexArray(void* vertices, const char* sentence, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color)
{
	std::vector<TextVertex>* vertexPtr;
	int numLetters, index, i;

	vertexPtr = static_cast<std::vector<TextVertex>*>(vertices);

	numLetters = strlen(sentence);
	index = 0;
	
	float topLeftScreenX = (pos.x * 2.0f) - 1.0f;
	float topLeftScreenY = ((1.0f - pos.y) * 2.0f) - 1.0f;
	
	float x = topLeftScreenX;
	float y = topLeftScreenY;

	float horizontalPadding = (_leftpadding + _rightpadding);
	float verticalPadding = (_toppadding + _bottompadding);


	wchar_t lastChar = -1; // no last character to start with

						   //Draw each letter onto a quad
	for (i = 0; i < numLetters; i++)
	{
		wchar_t c = sentence[i];

		FontChar* fc = GetChar(c);

		// character not in font char set
		if (fc == nullptr)
			continue;

		// end of string
		if (c == L'\0')
			break;

		float kerning = 0.0f;
		if (i > 0)
			kerning = GetKerning(lastChar, c);

		//First triangle in quad
		TextVertex vtx(color.x, color.y, color.z, color.w,
			fc->u, fc->v, fc->twidth, fc->theight,
			x + fc->xoffset + kerning, y + fc->yoffset + kerning, fc->width, fc->height);

		vertexPtr->push_back(vtx);

		// remove horrizontal padding and advance to next char position
		x += (fc->xadvance - horizontalPadding);

		lastChar = c;
	}
}

/******************************************************************************************************************/

bool Font::Load(Renderer* renderer, const wchar_t* fileName, const wchar_t* texture)
{
	//Load image for font
	_material.LoadPixelShader(renderer, L"TextPixelShader.hlsl");
	_material.LoadVertexShader(renderer, L"TextVertexShader.hlsl");
	_material.LoadTexture(renderer, texture);

	//Load fnt file
	LoadFont(fileName, Window::TheWindow->_width, Window::TheWindow->_height);

	return true;
}

void Font::LoadFont(const wchar_t* filename, int windowWidth, int windowHeight)
{
	std::wifstream fs;
	fs.open(filename);

	Font font;
	std::wstring tmp;
	int startpos;

	// extract font name
	fs >> tmp >> tmp; // info face="Capture It"
	startpos = tmp.find(L"\"") + 1;
	_name = tmp.substr(startpos, tmp.size() - startpos - 1);

	// get font size
	fs >> tmp; // size=73
	startpos = tmp.find(L"=") + 1;
	_size = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

	// bold, italic, charset, unicode, stretchH, smooth, aa, padding, spacing
	fs >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp; // bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 

														 // get padding
	fs >> tmp; // padding=5,5,5,5 
	startpos = tmp.find(L"=") + 1;
	tmp = tmp.substr(startpos, tmp.size() - startpos); // 5,5,5,5

													   // get up padding
	startpos = tmp.find(L",") + 1;
	_toppadding = std::stoi(tmp.substr(0, startpos)) / (float)windowWidth;

	// get right padding
	tmp = tmp.substr(startpos, tmp.size() - startpos);
	startpos = tmp.find(L",") + 1;
	_rightpadding = std::stoi(tmp.substr(0, startpos)) / (float)windowWidth;

	// get down padding
	tmp = tmp.substr(startpos, tmp.size() - startpos);
	startpos = tmp.find(L",") + 1;
	_bottompadding = std::stoi(tmp.substr(0, startpos)) / (float)windowWidth;

	// get left padding
	tmp = tmp.substr(startpos, tmp.size() - startpos);
	_leftpadding = std::stoi(tmp) / (float)windowWidth;

	fs >> tmp; // spacing=0,0

	fs >> tmp; //outline=1
			   // get lineheight (how much to move down for each line), and normalize (between 0.0 and 1.0 based on size of font)
	fs >> tmp >> tmp; // common lineHeight=95
	startpos = tmp.find(L"=") + 1;
	_lineHeight = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)windowHeight;

	// get base height (height of all characters), and normalize (between 0.0 and 1.0 based on size of font)
	fs >> tmp; // base=68
	startpos = tmp.find(L"=") + 1;
	_baseHeight = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)windowHeight;

	// get texture width
	fs >> tmp; // scaleW=512
	startpos = tmp.find(L"=") + 1;
	_textureWidth = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

	// get texture height
	fs >> tmp; // scaleH=512
	startpos = tmp.find(L"=") + 1;
	_textureHeight = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

	// get pages, packed, page id
	fs >> tmp >> tmp; // pages=1 packed=0

	fs >> tmp >> tmp >> tmp >> tmp;//alphaChnl = 1 redChnl = 0 greenChnl = 0 blueChnl = 0

	fs >> tmp >> tmp; // page id=0

					  // get texture filename
	std::wstring wtmp;
	fs >> wtmp; // file="Arial.png"
	startpos = wtmp.find(L"\"") + 1;
	_fontImage = wtmp.substr(startpos, wtmp.size() - startpos - 1);

	// get number of characters
	fs >> tmp >> tmp; // chars count=97
	startpos = tmp.find(L"=") + 1;
	_numCharacters = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

	// initialize the character list
	_CharList = new FontChar[_numCharacters];

	for (int c = 0; c < _numCharacters; ++c)
	{
		// get unicode id
		fs >> tmp >> tmp; // char id=0
		startpos = tmp.find(L"=") + 1;
		_CharList[c].id = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

		// get x
		fs >> tmp; // x=392
		startpos = tmp.find(L"=") + 1;
		_CharList[c].u = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)_textureWidth;

		// get y
		fs >> tmp; // y=340
		startpos = tmp.find(L"=") + 1;
		_CharList[c].v = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)_textureHeight;

		// get width
		fs >> tmp; // width=47
		startpos = tmp.find(L"=") + 1;
		tmp = tmp.substr(startpos, tmp.size() - startpos);
		_CharList[c].width = (float)std::stoi(tmp) / (float)windowWidth;
		_CharList[c].twidth = (float)std::stoi(tmp) / (float)_textureWidth;

		// get height
		fs >> tmp; // height=57
		startpos = tmp.find(L"=") + 1;
		tmp = tmp.substr(startpos, tmp.size() - startpos);
		_CharList[c].height = (float)std::stoi(tmp) / (float)windowHeight;
		_CharList[c].theight = (float)std::stoi(tmp) / (float)_textureHeight;

		// get xoffset
		fs >> tmp; // xoffset=-6
		startpos = tmp.find(L"=") + 1;
		_CharList[c].xoffset = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)windowWidth;

		// get yoffset
		fs >> tmp; // yoffset=16
		startpos = tmp.find(L"=") + 1;
		_CharList[c].yoffset = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)windowHeight;

		// get xadvance
		fs >> tmp; // xadvance=65
		startpos = tmp.find(L"=") + 1;
		_CharList[c].xadvance = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)windowWidth;

		// get page
		// get channel
		fs >> tmp >> tmp; // page=0    chnl=0
	}

	// get number of kernings
	fs >> tmp >> tmp; // kernings count=96
	startpos = tmp.find(L"=") + 1;
	_numKernings = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

	// initialize the kernings list
	_KerningsList = new FontKerning[_numKernings];

	for (int k = 0; k < _numKernings; ++k)
	{
		// get first character
		fs >> tmp >> tmp; // kerning first=87
		startpos = tmp.find(L"=") + 1;
		_KerningsList[k].firstid = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

		// get second character
		fs >> tmp; // second=45
		startpos = tmp.find(L"=") + 1;
		_KerningsList[k].secondid = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

		// get amount
		fs >> tmp; // amount=-1
		startpos = tmp.find(L"=") + 1;
		int t = (int)std::stoi(tmp.substr(startpos, tmp.size() - startpos));
		_KerningsList[k].amount = (float)t / (float)windowWidth;
	}
}

/******************************************************************************************************************/

// this will return a FontChar given a wide character
Font::FontChar* Font::GetChar(wchar_t c)
{
	for (int i = 0; i < _numCharacters; ++i)
	{
		if (c == (wchar_t)_CharList[i].id)
			return &_CharList[i];
	}
	return nullptr;
}

/******************************************************************************************************************/

// this will return the amount of kerning we need to use for two characters
float Font::GetKerning(wchar_t first, wchar_t second)
{
	for (int i = 0; i < _numKernings; ++i)
	{
		if ((wchar_t)_KerningsList[i].firstid == first && (wchar_t)_KerningsList[i].secondid == second)
			return _KerningsList[i].amount;
	}
	return 0.0f;
}