#pragma once

#include "Animation.h"
#include "Vec2.h"

class Avatar{
public:
	Avatar( const Vei2& readPos, int width, int height, int framecount, int animcount, Surface* sprite, float holdTime );
	int CurIndex() const {
		return animIndex;
	}
	int NAnim() const {
		return animCount;
	}
	void SetAnim( int animNum );
	void Draw( const Vei2& drawPos, Uint32 dword = 0xffffffff ) const;
	void DrawColorMod( const Vei2& pos, Uint8 r, Uint8 g, Uint8 b ) const;
	void DrawBlend( const Vei2& drawPos, const Uint8 alpha ) const;

	Drawable GetDrawable( const Vei2& pos, Uint32 dword = 0xffffffff ) const;

	void SetRenderer( SDL_Renderer* newRenderer );
	void Update( float dt );
	SDL_Renderer* GetRenderer() const {
		return animation.GetRenderer();
	}
private:
	int animIndex = 0;
	int width;
	int height;
	int animCount;
	Animation animation;
};