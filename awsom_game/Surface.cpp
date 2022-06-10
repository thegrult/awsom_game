#include "Surface.h"
#include <Windows.h>
#include <cassert>
#include <fstream>
#include <algorithm>

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
		SDL_SetTextureAlphaMod( newTexture, 0xff );
		//Get image dimensions
		SDL_QueryTexture( newTexture, NULL, NULL, &width, &height );
	}

	//old code for loading texture based on optimization of a surface
	/*//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if (loadedSurface == NULL)
	{
		OutputDebugStringA( (std::string("Unable to load image ") + path + std::string("!SDL_image Error : \n") + std::string(IMG_GetError())).c_str() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if (newTexture == NULL)
		{
			OutputDebugStringA(( std::string( "Unable to create texture from ")+ path + "!SDL Error : \n" + SDL_GetError() ).c_str());
		}
		else
		{
			SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );
			SDL_SetTextureAlphaMod( newTexture, 0xff );
			//Get image dimensions
			width = loadedSurface->w;
			height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}*/

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

void Surface::SetAlpha( const Uint8& newalpha )
{
	alpha = newalpha;

	SDL_SetTextureAlphaMod( texture, alpha );
}

Uint8 Surface::GetAlpha()
{
	return alpha;
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

	//old code for loading texture based on optimization of a surface
	/*//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if (loadedSurface == NULL)
	{
		OutputDebugStringA( (std::string("Unable to load image ") + path + std::string("!SDL_image Error : \n") + std::string(IMG_GetError())).c_str() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if (newTexture == NULL)
		{
			OutputDebugStringA(( std::string( "Unable to create texture from ")+ path + "!SDL Error : \n" + SDL_GetError() ).c_str());
		}
		else
		{
			SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );
			SDL_SetTextureAlphaMod( newTexture, 0xff );
			//Get image dimensions
			width = loadedSurface->w;
			height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}*/

	texture = newTexture;
	return true;
}

void Surface::SetRenderer( SDL_Renderer* newrenderer )
{
	renderer = newrenderer;
}

void Surface::DrawColorMod( const Vei2& pos, Uint8 r, Uint8 g, Uint8 b, SDL_Rect* clip )
{
	SDL_SetTextureColorMod( texture, r, g, b );
	Draw( pos, clip );
	SDL_SetTextureColorMod( texture, 0xff, 0xff, 0xff );
}