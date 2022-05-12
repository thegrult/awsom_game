#include "Background.h"

Background::Background( Surface* sprite, int width, int height, Vei2 readPos, Vei2 drawStartPos, int gridw, int gridh, std::string map )
	:
	sprite( sprite ),
	tilew( width ),
	tileh( height ),
	gridw( gridw ),
	gridh( gridh ),
	drawStartPos( drawStartPos )
{
	tiles.reserve( gridw * gridh );

	auto mi = map.cbegin();
	
	for (int n = 0; n < gridw * gridh; n++, mi++) {
		auto entry = BgStringConverter::Convert( *mi );
		tiles.push_back( std::move( entry.first.GetDisplaced( readPos ) ) );
		if (!entry.second.IsDegenerate())
		{
			int y = n / gridw;
			int x = n % gridw;
			obstacles.push_back( std::move( entry.second.GetDisplaced( {x * width, y*height}) ) );
		}
	}
}

void Background::Draw()
{
	for (int y = 0; y < gridh; y++) {
		for (int x = 0; x < gridw; x++) {
			const auto j = tiles[x + y * gridw];
			SDL_Rect clip = { j.TopLeft().x, j.TopLeft().y, j.GetDim().x, j.GetDim().y };
			sprite->Draw( Vei2( x * tilew, y * tileh ) + drawStartPos, &clip );
		}
	}
}

bool Background::IsColliding( RectI hitBox )
{
	return std::any_of( obstacles.begin(), obstacles.end(), 
		[&hitBox]( RectI ob )
		{
			return ob.IsOverlappingWith( hitBox );
		});
}
