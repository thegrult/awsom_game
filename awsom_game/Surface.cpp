#include "Surface.h"
#include <cassert>
#include <fstream>
#include <algorithm>

Surface::Surface( const std::string& path, SDL_Renderer* gRenderer )
	:
	renderer(gRenderer)
{
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if (loadedSurface == NULL)
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if (newTexture == NULL)
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			width = loadedSurface->w;
			height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
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

int Surface::GetWidth() const
{
	return width;
}

int Surface::GetHeight() const
{
	return height;
}

SDL_Rect Surface::GetRect() const
{
	return{ 0,width,0,height };
}

SDL_Texture* Surface::Data() const
{
	return texture;
}

void Surface::Draw( const Vei2& pos, SDL_Rect* clip ) const
{
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
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if (loadedSurface == NULL)
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
		return false;
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if (newTexture == NULL)
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			width = loadedSurface->w;
			height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	texture = newTexture;
	return true;
}

void Surface::SetRenderer( SDL_Renderer* newrenderer )
{
	renderer = newrenderer;
}