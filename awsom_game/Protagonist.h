#pragma once
#include "Entity.h"
#include "Projectile.h"

class Protagonist {
public:
	Protagonist( Vec2 spawnPos, SDL_Renderer* renderer );

	void Update( float dt );
	void SetDirection( const Vec2& dir );
	void Draw();

	RectI GetHitBox() const;
	void ClampToRect( RectI rect );

	void ApplyDamage( int dmg );
	Projectile Shoot();
	bool IsOnCooldown() const {
		return coolDownTimer > 0;
	}
	int GetAtk() const { return entity.GetAtk(); }
private:
	enum direction {
		down,
		left,
		up,
		right
	};
	int direc = down;
	float coolDownTimer = 0.0f;
	float coolDown = 0.5f;
	Surface sprite;
	Entity entity;
};