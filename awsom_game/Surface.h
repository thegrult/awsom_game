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
	RectI GetRect() const;
	void Draw( const Vei2& pos, const SDL_Rect* clip = 0, const SDL_Rect* dst = 0, Uint32 dword = 0xffffffff ) const;
	void FreeData();
	bool LoadData( const std::string& filename );
	void SetRenderer( SDL_Renderer* renderer );
	SDL_Renderer* GetRenderer() const {
		return renderer;
	}
private:
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	int width = 0;
	int height = 0;
};