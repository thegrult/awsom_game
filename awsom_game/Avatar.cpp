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

void Avatar::Draw( const Vei2& drawPos, Uint32 dword ) const
{
	animation.Draw( drawPos );
}

void Avatar::DrawColorMod( const Vei2& pos, Uint8 r, Uint8 g, Uint8 b ) const
{
	animation.DrawColorMod( pos, r, g, b );
}

void Avatar::DrawBlend( const Vei2& drawPos, const Uint8 alpha ) const
{
	animation.DrawBlend( drawPos, alpha );
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
