#include "Animation.h"

Animation::Animation( const Vei2& pos, int width, int height, int count, const Surface& sprite, float holdTime, SDL_Renderer* renderer )
	:
	sprite( sprite ),
	holdTime( holdTime ),
	renderer(renderer),
	width(width),
	height(height)
{
	for( int i = 0; i < count; i++ )
	{
		frames.push_back( { Vei2( pos.x + i * width , pos.y ), width, height } );
	}
}

void Animation::Draw( const Vei2& pos ) const
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { pos.x, pos.y, width, height };

	//Render to screen

	SDL_Rect srcRect( frames[iCurFrame].GetDisplaced( srcDeltaPos ) );

	SDL_RenderCopy( renderer, sprite.Data(), &srcRect, &renderQuad );
}

void Animation::Update( float dt )
{
	curFrameTime += dt;
	while( curFrameTime >= holdTime )
	{
		Advance();
		curFrameTime -= holdTime;
	}
}

void Animation::SetRenderer( SDL_Renderer* newRenderer )
{
	renderer = newRenderer;
}

const Surface& Animation::GetSprite()
{
	return sprite;
}

void Animation::Advance()
{
	if( ++iCurFrame >= frames.size() )
	{
		iCurFrame = 0;
	}
}
