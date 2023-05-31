//==============================================================================
#include "Texture.h"
//#include "Log.h"

//==============================================================================
Texture::Texture() {
}

//------------------------------------------------------------------------------
Texture::~Texture() 
{
	if (SDLTexture) 
	{
		SDL_DestroyTexture(SDLTexture);
		SDLTexture = NULL;
	}
}

//==============================================================================
bool Texture::Load(SDL_Renderer* Renderer, std::string Filename) {
	if (Renderer == NULL) 
	{
		//Log("Bad SDL renderer passed");
		return false;
	}

	this->Renderer = Renderer;
	this->Filename = Filename;

	SDL_Surface* TempSurface = IMG_Load(Filename.c_str());
	if (TempSurface == NULL) {
		//Log("Unable to load image : %s : %s", Filename.c_str(), IMG_GetError());
		return false;
	}

	// Convert SDL surface to a texture
	if ((SDLTexture = SDL_CreateTextureFromSurface(Renderer, TempSurface)) == NULL) {
		//Log("Unable to create SDL Texture : %s : %s", Filename.c_str(), IMG_GetError());
		return false;
	}

	// Grab dimensions
	SDL_QueryTexture(SDLTexture, NULL, NULL, &Width, &Height);

	//Log("Texture Dimensions: %s : %d %d", Filename.c_str(), Width, Height);

	SDL_FreeSurface(TempSurface);

	return true;
}

//------------------------------------------------------------------------------
void Texture::Render(int X, int Y) {
	Render(X, Y, Width, Height);
}

//------------------------------------------------------------------------------
void Texture::Render(int X, int Y, int Width, int Height) {
	SDL_Rect Destination = { X, Y, Width, Height };

	SDL_RenderCopy(Renderer, SDLTexture, NULL, &Destination);
}

//------------------------------------------------------------------------------
void Texture::Render(int X, int Y, int Width, int Height, int SX, int SY, int SWidth, int SHeight) {
	SDL_Rect Source = { SX, SY, SWidth, SHeight };
	SDL_Rect Destination = { X, Y, Width, Height };

	SDL_RenderCopy(Renderer, SDLTexture, &Source, &Destination);
}

void Texture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(SDLTexture, alpha);
}

void Texture::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(SDLTexture, r,g,b);
}

void Texture::setBlendMode(SDL_BlendMode blendMode)
{
	SDL_SetTextureBlendMode(SDLTexture, blendMode);
}

int Texture::GetLineWrapLimit()
{
	return LINE_WRAP_LIMIT;
}

bool Texture::loadFromRenderedText(SDL_Renderer* renderer, const std::string& textureText, SDL_Color textColor, TTF_Font* gFont)
{

	if (SDLTexture)
	{
		SDL_DestroyTexture(SDLTexture);
		SDLTexture = NULL;
	}

	Renderer = renderer;
	//Get rid of preexisting texture
	//free();
	//Render text surface

	// TODO RESOLUTION HARDCODE:
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(gFont, textureText.c_str(), textColor, LINE_WRAP_LIMIT);
	//TTF_SetFontOutline(gFont, 1);
	//SDL_Surface* textSurface = TTF_RenderUTF8_Solid(gFont, textureText.c_str(), textColor);
	
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		SDLTexture = SDL_CreateTextureFromSurface(Renderer, textSurface);
		if (SDLTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			 Width = textSurface->w;
			 Height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return SDLTexture != NULL;
}

//------------------------------------------------------------------------------
int Texture::GetWidth() { return Width; }
int Texture::GetHeight() { return Height; }

//==============================================================================