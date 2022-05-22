#pragma once
#include "Entity.h"
#include "Projectile.h"
#include "Action.h"

class Protagonist {
public:
	Protagonist( Vec2 spawnPos, Surface* Sprite );

	void Update( float dt, const Uint8* kbdStates );
	void Draw( const Camera& camPos );

	RectF GetHitBox() const;
	void ClampToRect( RectI rect );

	int GetAtk() const { return entity.GetAtk(); }
	
	void ApplyDamage( int dmg );
	bool Shoot( std::vector<Projectile>& projectiles );

	void CollideRect( RectF rect ) { entity.CollideRect( rect ); }

	Vec2 GetPos() const;
private:
	void SetDirection( const Vec2& dir );

	void Dash();
private:
	//used for giving direction to the bullets and dashing
	Vec2 dir = { 0.0f,0.0f };

private:
	Action action;
	//some stats for our protagonist

	float walkingSpeed = 50.0f;
	float rollSpeed = 200.0f;
private:
	Surface* sprite;
	Entity entity;
};