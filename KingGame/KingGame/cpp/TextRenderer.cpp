
#include "TextRenderer.h"


TextRenderer::TextRenderer()
{
	_width = 0;
	_height = 0;

	if (TTF_Init() == -1)
	{
		SDL_Quit();
		throw std::runtime_error("TTF_Init returned error. " + string(TTF_GetError()));
	}
}

TextRenderer::~TextRenderer()
{

}

bool TextRenderer::LoadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	if (_textSurface != NULL) 
		SDL_FreeSurface(_textSurface);

	_textSurface = TTF_RenderText_Solid(_font, textureText.c_str(), textColor);
	if (_textSurface == NULL)
	{
		SDL_Quit();
		throw std::runtime_error("TTF_RenderText_Solid returned error. " + string(TTF_GetError()));
	}
	else
	{

		if (_texture != NULL) {

			SDL_DestroyTexture(_texture);
			_texture = NULL;
		}

		_texture = SDL_CreateTextureFromSurface(_renderer, _textSurface);

		if (GetTexture() == NULL)
		{
			SDL_Quit();
			throw std::runtime_error("SDL_CreateTextureFromSurface returned error. " + string(SDL_GetError()));
		}
		else
		{
			_width = _textSurface->w;
			_height = _textSurface->h;
		}

		
		
	}

	return GetTexture() != NULL;
}

void TextRenderer::Free()
{
}

void TextRenderer::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(GetTexture(), red, green, blue);
}

void TextRenderer::SetBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(GetTexture(), blending);
}

void TextRenderer::SetAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(GetTexture(), alpha);
}

void TextRenderer::Render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { x, y, _width, _height };

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(GetRenderer(), GetTexture(), clip, &renderQuad, angle, center, flip);
}

int TextRenderer::GetWidth()
{
	return _width;
}

int TextRenderer::GetHeight()
{
	return _height;
}

bool TextRenderer::LoadMedia(std::string path, std::string textOutput, int fontSize, SDL_Color sdl_color)
{
	_font = TTF_OpenFont(path.c_str(), fontSize);
	if (_font == NULL)
	{
		SDL_Quit();
		throw std::runtime_error("TTF_OpenFont returned error. " + string(TTF_GetError()));
	}
	else
	{
		if (!LoadFromRenderedText(textOutput, sdl_color))
		{
			SDL_Quit();
			throw std::runtime_error("Failed to render text. ");
		}
	}

	return true;
}