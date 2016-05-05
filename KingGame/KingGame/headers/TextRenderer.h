#ifndef TEXT_H
#define TEXT_H

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <Object.h>

using namespace std;


class TextRenderer : public Object
{
	TTF_Font * _font = NULL;
	SDL_Surface * _textSurface;

	int _width;
	int _height;

public:
	TextRenderer();

	~TextRenderer();

	bool LoadFromFile(std::string path);

	bool LoadFromRenderedText(std::string textureText, 
							  SDL_Color textColor);

	void Free();

	bool LoadMedia(std::string path, std::string textOutput, int fontSize, SDL_Color sdl_color);

	void SetColor(Uint8 red, Uint8 green, Uint8 blue);

	void SetBlendMode(SDL_BlendMode blending);

	void SetAlpha(Uint8 alpha);

	void Render(int x, int y, 
				SDL_Rect* clip = NULL, 
				double angle = 0.0, 
				SDL_Point* center = NULL, 
				SDL_RendererFlip flip = SDL_FLIP_NONE);

	int GetWidth();
	int GetHeight();
};

#endif
