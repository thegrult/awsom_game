#pragma once

#include "Surface.h"
#include "Drawable.h"
#include <vector>
#include "Rect.h"

class Animation
{
public:
	Animation( const Vei2& pos,int width,int height,int count, Surface* sprite,float holdTime, SDL_Renderer* renderer );
	void SetDeltaPos( const Vei2& deltaPos ) {
		srcDeltaPos = deltaPos;
	}

	void operator=( const Animation& rhs );

	Drawable CreateDrawable( const Vei2& pos, const Uint32 dword ) const;

	void Update( float dt );
	void SetRenderer( SDL_Renderer* newRenderer );
	SDL_Renderer* GetRenderer() const {
		return renderer;
	}
	const Surface& GetSprite();
private:
	void Advance();
private:
	SDL_Renderer* renderer = nullptr;
	Surface* sprite;
	std::vector<RectI> frames;
	Vei2 srcDeltaPos = { 0, 0 };
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;
	int height;
	int width;
};