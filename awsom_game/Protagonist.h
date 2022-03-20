#pragma once
#include "Entity.h"

class Protagonist {
public:
	Protagonist( Vec2 spawnPos, SDL_Renderer* renderer );

	void Update( float dt );
	void SetDirection( const Vec2& dir );
	void Draw();

	RectI GetHitBox() const;
	void ClampToRect( RectI rect );

	void ApplyDamage();
private:
	Surface sprite;
	Entity entity;
};