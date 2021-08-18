#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

#include "Texture.h"

class Texture 
{
private:
	std::string Filename;

	int Width = 0;
	int Height = 0;

	SDL_Renderer* Renderer = NULL;
	SDL_Texture* SDLTexture = NULL;

	// TODO: VALUE DEPENDS ON RESOLUTION
	static const int LINE_WRAP_LIMIT = 1100;

public:
	Texture();
	~Texture();

	bool Load(SDL_Renderer* Renderer, std::string Filename);

	void Render(int X, int Y);

	void Render(int X, int Y, int Width, int Height);

	void Render(int X, int Y, int Width, int Height, int SX, int SY, int SWidth, int SHeight);

	void setAlpha(Uint8 alpha);

	void setColor(Uint8 r, Uint8 g, Uint8 b);

	void setBlendMode(SDL_BlendMode blendMode);

	static int GetLineWrapLimit();

	//Creates image from font string
	bool loadFromRenderedText(SDL_Renderer* renderer, const std::string& textureText, SDL_Color textColor, TTF_Font* gFont);

	int GetWidth();
	int GetHeight();
};

#endif