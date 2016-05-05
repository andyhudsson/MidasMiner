#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Object
{

	SDL_Rect _rectPos;

protected:
	SDL_Renderer * _renderer = NULL;
	SDL_Texture * _texture = NULL;

public:

	Object();
	Object(SDL_Renderer * renderer);

	~Object();

	SDL_Rect GetRectPos() const;
	SDL_Renderer * GetRenderer();

	SDL_Texture * GetTexture();

	void LoadTexture(std::string fileName);

	void Render();

	void SetRectPos(SDL_Rect rect);

	void SetRenderer(SDL_Renderer * renderer);

	void SetTexture(SDL_Texture * texture);
};

#endif