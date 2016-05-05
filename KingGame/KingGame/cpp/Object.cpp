
#include "Object.h"

Object::Object(SDL_Renderer * renderer) :_renderer(renderer) {}

Object::Object()
{
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);

	if ((initted&flags) != flags)
	{
		SDL_Quit();
		throw std::runtime_error("IMG_Init returned error. " + std::string(SDL_GetError()));
	}
}

Object::~Object()
{
  	SDL_DestroyTexture(_texture);
}

SDL_Rect Object::GetRectPos() const
{
	return _rectPos;
}

void Object::LoadTexture(std::string fileName)
{
	_texture = IMG_LoadTexture(_renderer, fileName.c_str());

	if (_texture == NULL)
	{
		SDL_Quit();
		throw std::runtime_error("IMG_LoadTexture returned error. " + std::string(SDL_GetError()));
	}
}

void Object::Render()
{
	SDL_RenderCopy(_renderer, _texture, NULL, &_rectPos);
}

void Object::SetRectPos(SDL_Rect rect)
{ _rectPos = rect; }

void Object::SetRenderer(SDL_Renderer * renderer)
{ _renderer = renderer; }


SDL_Renderer * Object::GetRenderer()
{
	return _renderer;
}

SDL_Texture * Object::GetTexture()
{
	return _texture;
}

void Object::SetTexture(SDL_Texture * texture)
{
	_texture = texture;
}
