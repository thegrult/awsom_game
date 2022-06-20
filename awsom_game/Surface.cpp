#include "Surface.h"
#include <Windows.h>
#include <cassert>
#include <fstream>

Surface::Surface( const std::string& path, SDL_Renderer* gRenderer )
	:
	renderer(gRenderer)
{
	SDL_Texture* newTexture = IMG_LoadTexture( gRenderer, path.c_str() );
	if (!newTexture)
	{
		OutputDebugStringA( ("Unable to load image " + path + "!SDL_image Error : \n" + IMG_GetError() ).c_str() );
	}
	else {
		SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );
		SDL_SetTextureScaleMode( newTexture, SDL_ScaleModeNearest );
		SDL_SetTextureAlphaMod( newTexture, 0xff );
		//Get image dimensions
		SDL_QueryTexture( newTexture, NULL, NULL, &width, &height );
	}

	texture = newTexture;
}

Surface::Surface( Surface&& donor ) noexcept
{
	FreeData();
	*this = std::move( donor );
}

Surface& Surface::operator=( Surface&& rhs ) noexcept
{
	FreeData();
	width = rhs.width;
	height = rhs.height;
	texture = rhs.texture;
	rhs.width = 0;
	rhs.height = 0;
	return *this;
}

Surface::~Surface()
{
	if (texture != nullptr) {
		SDL_DestroyTexture( texture );
		texture = nullptr;
		width = 0;
		height = 0;
	}
}

int Surface::GetWidth() const
{
	return width;
}

int Surface::GetHeight() const
{
	return height;
}

RectI Surface::GetRect() const
{
	return RectI( 0, width, 0, height );
}

void Surface::FreeData()
{
	if (texture != nullptr) {
		SDL_DestroyTexture( texture );
		texture = nullptr;
		width = 0;
		height = 0;
	}
}

bool Surface::LoadData( const std::string& path )
{
	FreeData();
	SDL_Texture* newTexture = IMG_LoadTexture( renderer, path.c_str() );
	if (!newTexture)
	{
		OutputDebugStringA( ("Unable to load image " + path + "!SDL_image Error : \n" + IMG_GetError()).c_str() );
	}
	else {
		SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );
		SDL_SetTextureAlphaMod( newTexture, 0xff );
		//Get image dimensions
		SDL_QueryTexture( newTexture, NULL, NULL, &width, &height );
	}

	texture = newTexture;
	return true;
}

void Surface::SetRenderer( SDL_Renderer* newrenderer )
{
	renderer = newrenderer;
}

void Surface::Draw( const Vei2& pos, const SDL_Rect* clip, const SDL_Rect* dst, Uint32 dword ) const
{
	SDL_SetTextureColorMod( texture, dword >> 24, (dword >> 16) & 0x000000ff, (dword >> 8) & 0x000000ff );
	SDL_SetTextureAlphaMod( texture, dword & 0x000000ff );
	if (dst == NULL) {
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { pos.x, pos.y, width, height };

		//Set clip rendering dimensions
		if (clip != NULL)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		//Render to screen
		SDL_RenderCopy( renderer, texture, clip, &renderQuad );
	}
	else
	{
		SDL_RenderCopy( renderer, texture, clip, dst );
	}
}