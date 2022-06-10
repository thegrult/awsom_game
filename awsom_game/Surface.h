#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <assert.h>
#include "Rect.h"

class Surface
{
public:
	Surface( const std::string& filename, SDL_Renderer* );
	Surface( Surface&& donor ) noexcept;
	Surface( const Surface& ) = default;
	Surface& operator=( Surface&& rhs ) noexcept;
	Surface& operator=( const Surface& ) = default;
	Surface() = default;
	~Surface();
	int GetWidth() const;
	int GetHeight() const;
	SDL_Rect GetRect() const;
	SDL_Texture* Data() const;
	void Draw(const Vei2& pos, SDL_Rect* clip = 0) const;
	void SetAlpha( const Uint8& alpha );
	Uint8 GetAlpha();
	void FreeData();
	bool LoadData( const std::string& filename );
	void SetRenderer( SDL_Renderer* renderer );
	SDL_Renderer* GetRenderer() const {
		return renderer;
	}
	void DrawColorMod( const Vei2& pos, Uint8 r, Uint8 g, Uint8 b, SDL_Rect* clip = 0 );
private:
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	int width = 0;
	int height = 0;
	Uint8 alpha = 0xff;
};