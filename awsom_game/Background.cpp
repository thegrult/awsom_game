#include "Background.h"

Background::Background( Surface tiles, int width, int height, Vei2 readPos )
	:
	readPos(readPos),
	tiles(tiles),
	tilew(width),
	tileh(height)	
{}

void Background::Draw()
{
	SDL_Rect clip = RectI( readPos, tilew, tileh );
	tiles.Draw( { 0,0 }, &clip );
}