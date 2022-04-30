#pragma once
#include "Entity.h"
#include "Projectile.h"

class Protagonist {
public:
	Protagonist( Vec2 spawnPos, Surface* Sprite, SDL_Renderer* renderer );

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

	void Dash();
private:
	//state machine
	struct Action {
		enum {
			walking,
			dashing
		};
		int action = walking;
		float actionDur = 0.5f;
		float actionTimer = 0.0f;
		void SetAction( int action, float actDur );
		void Update( float dt );
	};

	//used for giving direction to the bullets and dashing
	Vec2 dir = { 0.0f,0.0f };

private:
	//some stats for our protagonist
	float coolDownTimer = 0.0f;
	float coolDown = 0.5f;
	float walkingSpeed = 50.0f;
	float rollSpeed = 200.0f;
private:
	Surface* sprite;
	Entity entity;
};