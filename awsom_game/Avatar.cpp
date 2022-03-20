#include "Avatar.h"

Avatar::Avatar( const Vei2& pos, int width, int height, int framecount, int animcount, const Surface& sprite, float holdTime, SDL_Renderer* renderer )
	:
	width(width),
	height(height),
	animCount(animcount),
	animation( pos, width, height, framecount, sprite, holdTime, renderer )
{}

void Avatar::SetAnim( int newAnimIndex )
{
	animIndex = newAnimIndex;
	animation.SetDeltaPos( { 0, newAnimIndex * height } );
}

void Avatar::Draw( const Vei2& drawPos ) const
{
	animation.Draw( drawPos );
}

void Avatar::SetRenderer( SDL_Renderer* newRenderer )
{
	animation.SetRenderer( newRenderer );
}

void Avatar::Update( float dt )
{
	animation.Update( dt );
}
