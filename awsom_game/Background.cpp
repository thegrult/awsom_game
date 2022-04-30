#include "Background.h"

Background::Background( Surface* sprite, int width, int height, Vei2 readPos, Vei2 drawStartPos, int gridw, int gridh, std::string map )
	:
	sprite(sprite),
	tilew(width),
	tileh(height),
	gridw(gridw),
	gridh(gridh),
	drawStartPos(drawStartPos)
{
	tiles.reserve( gridw * gridh );

	auto mi = map.cbegin();
	
	for (int n = 0; n < gridw * gridh; n++, mi++) {
		int i = *mi - 'A';
		tiles.push_back({ Vei2(i * width, 0) + readPos, width, height });
	}
}

void Background::Draw()
{
	for (int y = 0; y < gridh; y++) {
		for (int x = 0; x < gridw; x++) {
			SDL_Rect clip = SDL_Rect( tiles[x + y * gridw] );
			sprite->Draw( Vei2( x * tilew, y * tileh ) + drawStartPos, &clip );
		}
	}
}