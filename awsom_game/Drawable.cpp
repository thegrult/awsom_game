#include "Drawable.h"

Drawable::Drawable( const Surface* pSurf, const RectI& srcRect, const RectI& dstRect, const Uint32 dword )
	:
	pSurf(pSurf),
	srcRect(srcRect),
	dstRect(dstRect),
	dword(dword)
{}

void Drawable::Draw() const
{
	pSurf->Draw( { dstRect.x, dstRect.y }, &srcRect, &dstRect, dword );
}