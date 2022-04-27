#pragma once
#include "Entity.h"
#include "Projectile.h"

class Protagonist {
public:
	Protagonist( Vec2 spawnPos, SDL_Renderer* renderer );

	void Update( float dt, const Uint8* kbdStates );
	void Draw();

	RectI GetHitBox() const;
	void ClampToRect( RectI rect );

	int GetAtk() const { return entity.GetAtk(); }
	
	void ApplyDamage( int dmg );
	Projectile Shoot();
private:
	void SetDirection( const Vec2& dir );

	bool IsOnCooldown() const {
		return coolDownTimer > 0;
	}
private:
	Vec2 dir = { 0.0f,0.0f };
	float coolDownTimer = 0.0f;
	float coolDown = 0.5f;
	Surface sprite;
	Entity entity;
};