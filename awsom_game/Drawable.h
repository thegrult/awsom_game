#pragma once
#include "Surface.h"

class Drawable{
public:
	//dword is in rgba format
	Drawable( const Surface* pSurf, const RectI& srcRect, const RectI& dstRect, const Uint32 dword );
	void Draw() const;
private:
	const Surface* pSurf;
	const RectI srcRect;
	const RectI dstRect;
	const Uint32 dword;
};