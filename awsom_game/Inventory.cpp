#include "Inventory.h"

Inventory::Inventory( SDL_Renderer* renderer )
	:
	sprite( "imgs\\inventorystolen.png", renderer )
{
	sprite.SetAlpha( 0xee );
}

void Inventory::Draw( RectI screen )
{
	if (shown) {
		sprite.Draw( screen.GetCenter() - (sprite.GetRect().GetDim() / 2) );
	}
}