#include "Animation.h"

Animation::Animation( const Vei2& pos, int width, int height, int count, Surface* sprite, float holdTime, SDL_Renderer* renderer )
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

void Animation::operator=( const Animation& rhs )
{
	renderer = rhs.GetRenderer();
	sprite = rhs.sprite;
	frames = rhs.frames;
	srcDeltaPos = rhs.srcDeltaPos;
	holdTime = rhs.holdTime;
	height = rhs.height;
	width = rhs.width;
}

void Animation::Draw( const Vei2& pos, const Uint32 dword ) const
{
	const auto j = frames[iCurFrame].GetDisplaced( srcDeltaPos );
	SDL_Rect srcRect = { (int)j.TopLeft().x, (int)j.TopLeft().y, (int)j.GetDim().x, (int)j.GetDim().y };

	sprite->Draw( pos, &srcRect, 0, dword );
}

void Animation::DrawColorMod( const Vei2& pos, Uint8 r, Uint8 g, Uint8 b ) const
{
	const auto j = frames[iCurFrame].GetDisplaced( srcDeltaPos );
	SDL_Rect srcRect = { (int)j.TopLeft().x, (int)j.TopLeft().y, (int)j.GetDim().x, (int)j.GetDim().y };

	Draw( pos, (r << 24) | (g << 16) | (b << 8) );
}

void Animation::DrawBlend( const Vei2& pos, const Uint8 alpha ) const
{
	const auto j = frames[iCurFrame].GetDisplaced( srcDeltaPos );
	SDL_Rect srcRect = { (int)j.TopLeft().x, (int)j.TopLeft().y, (int)j.GetDim().x, (int)j.GetDim().y };

	sprite->Draw( pos, &srcRect, 0, 0xffffff00 | alpha );
}

Drawable Animation::CreateDrawable( const Vei2& pos, const Uint32 dword ) const
{
	const auto j = frames[iCurFrame].GetDisplaced( srcDeltaPos );
	SDL_Rect srcRect = { (int)j.TopLeft().x, (int)j.TopLeft().y, (int)j.GetDim().x, (int)j.GetDim().y };

	return Drawable( sprite, srcRect, RectI( pos, pos + j.GetDim() ), dword );
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
	return *sprite;
}

void Animation::Advance()
{
	if( ++iCurFrame >= frames.size() )
	{
		iCurFrame = 0;
	}
}
