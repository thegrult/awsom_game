#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "Vec2.h"

class Surface
{
public:
	Surface( const std::string& filename, SDL_Renderer* );
	Surface( Surface&& donor ) noexcept;
	Surface( const Surface& ) = default;
	Surface& operator=( Surface&& rhs ) noexcept;
	Surface& operator=( const Surface& ) = default;
	Surface() = default;
	int GetWidth() const;
	int GetHeight() const;
	SDL_Rect GetRect() const;
	SDL_Texture* Data() const;
	void Draw(const Vei2& pos, SDL_Rect* clip = 0) const;
	void FreeData();
	bool LoadData( const std::string& filename );
	void SetRenderer( SDL_Renderer* renderer );
private:
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	int width = 0;
	int height = 0;
};