#include "Background.h"

Background::Background( Surface* sprite, int width, int height, Vei2 readPos, Vei2 drawStartPos, int gridw, int gridh, std::ifstream& map )
	:
	sprite( sprite ),
	tilew( width ),
	tileh( height ),
	gridw( gridw ),
	gridh( gridh ),
	drawStartPos( drawStartPos )
{
	tiles.reserve( gridw * gridh );

	char tile;

	map >> tile;

	for (int n = 0; n < gridw * gridh; n++, map >> tile) {
		auto entry = BgStringConverter::Convert( tile );
		tiles.push_back( std::move( entry.first.GetDisplaced( readPos ) ) );
		if (!entry.second.IsDegenerate())
		{
			int y = n / gridw;
			int x = n % gridw;
			obstacles.push_back( std::move( entry.second.GetDisplaced( {x * width, y*height}) ) );
		}
	}
}

void Background::Draw( const Vei2& camPos ) const
{
	for (int y = 0; y < gridh; y++) {
		for (int x = 0; x < gridw; x++) {
			const auto j = tiles[x + y * gridw];
			SDL_Rect clip = { j.TopLeft().x, j.TopLeft().y, j.GetDim().x, j.GetDim().y };
			sprite->Draw( Vei2( x * tilew, y * tileh ) + drawStartPos - camPos, &clip );
		}
	}
#ifdef _DEBUG //debug mode
	for (auto a : obstacles) {
		const auto r = SDL_Rect( a.GetDisplaced( -camPos ) );
		SDL_RenderDrawRect( sprite->GetRenderer(), &r );
	}
#endif
}

bool Background::IsColliding( RectI hitBox )
{
	return std::any_of( obstacles.begin(), obstacles.end(), 
		[&hitBox]( RectI ob )
		{
			return ob.IsOverlappingWith( hitBox );
		});
}
