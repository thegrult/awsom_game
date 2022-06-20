#include "Inventory.h"

Inventory::Inventory( SDL_Renderer* renderer, McMode& mode )
	:
	sprite( "imgs\\inventorystolen.png", renderer ),
	mode( mode )
{
	box = sprite.GetRect().GetDisplaced( (Vei2( SCREEN_WIDTH, SCREEN_HEIGHT ) - sprite.GetRect().GetDim()) / 2 );
	imgs.push_back( { McMode::Modes::melee,{ RectI( box.left, box.GetCenter().x, box.top, box.bottom ), new Surface( "imgs\\sword_small.png", renderer )} } );
	imgs.push_back( { McMode::Modes::ranged,{ RectI( box.GetCenter().x, box.right, box.top, box.bottom ), new Surface( "imgs\\bow_small.png", renderer )} } );
}

void Inventory::Draw( RectI screen )
{
	if (shown) {
		sprite.Draw( screen.GetCenter() - (sprite.GetRect().GetDim() / 2), 0, 0, 0xffffffd9 );
		for (const auto& a : imgs) {
			const auto dst = (SDL_Rect)a.second.first.GetSquare();
			a.second.second->Draw( a.second.first.TopLeft(), 0, &dst );
		}
	}
}

void Inventory::HandleInput( Wrld* wrld )
{
	if (shown && (wrld->GetMouseKeys() && SDL_BUTTON(1)) && box.Contains( wrld->GetMousePos() )) {
		const auto mpos = wrld->GetMousePos();
		for (const auto& a : imgs) {
			if (a.second.first.Contains( mpos )) {
				SetMode( a.first );
				break;
			}
		}
	}
}

void Inventory::SetMode( McMode::Modes newMode )
{
	mode.SetMode( newMode );
}

void Inventory::SetMode( int newMode )
{
	mode.SetMode( newMode );
}
