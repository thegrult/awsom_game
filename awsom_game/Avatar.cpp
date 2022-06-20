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

Drawable Avatar::GetDrawable( const Vei2& pos, Uint32 dword ) const
{
	return animation.CreateDrawable( pos, dword );
}

void Avatar::SetRenderer( SDL_Renderer* newRenderer )
{
	animation.SetRenderer( newRenderer );
}

void Avatar::Update( float dt )
{
	animation.Update( dt );
}
