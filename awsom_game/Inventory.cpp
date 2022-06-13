#include "Inventory.h"

Inventory::Inventory( SDL_Renderer* renderer, McMode& mode )
	:
	sprite( "imgs\\inventorystolen.png", renderer ),
	mode( mode )
{
	sprite.SetAlpha( 0xee );
	box = sprite.GetRect().GetDisplaced( (Vei2(SCREEN_WIDTH, SCREEN_HEIGHT ) - sprite.GetRect().GetDim())/2 );
}

void Inventory::Draw( RectI screen )
{
	if (shown) {
		sprite.Draw( screen.GetCenter() - (sprite.GetRect().GetDim() / 2) );
	}
}

void Inventory::HandleInput( Wrld* wrld )
{
	if (shown && (wrld->GetMouseKeys() && SDL_BUTTON(1)) && box.Contains( wrld->GetMousePos() )) {
		if (action.Do( changeMode, 0.0f, 1.0f )) {
			if (mode.mode == McMode::Modes::ranged)
				mode.SetMode( McMode::Modes::melee );
			else mode.SetMode( McMode::Modes::ranged );
		}
	}
}