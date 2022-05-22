#include "Avatar.h"

Avatar::Avatar( const Vei2& pos, int width, int height, int framecount, int animcount, Surface* sprite, float holdTime )
	:
	width(width),
	height(height),
	animCount(animcount),
	animation( pos, width, height, framecount, sprite, holdTime, sprite->GetRenderer() )
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

void Avatar::DrawBlend( const Vei2& drawPos, const Uint8 alpha )
{
	animation.DrawBlend( drawPos, alpha );
}

void Avatar::SetRenderer( SDL_Renderer* newRenderer )
{
	animation.SetRenderer( newRenderer );
}

void Avatar::Update( float dt )
{
	animation.Update( dt );
}
