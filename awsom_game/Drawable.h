#pragma once
#include "Surface.h"

class Drawable{
public:
	//dword is in rgba format
	Drawable( const Surface* pSurf, const RectI& srcRect, const RectI& dstRect, const Uint32 dword );
	void Draw() const;
	int GetY() const { return dstRect.y + dstRect.h; }
private:
	const Surface* pSurf;
	SDL_Rect srcRect;
	SDL_Rect dstRect;
	Uint32 dword;
};