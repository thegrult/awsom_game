#pragma once
#include "Entity.h"
#include "Projectile.h"
#include <vector>

class Protagonist {
public:
	Protagonist( Vec2 spawnPos, Surface* Sprite, SDL_Renderer* renderer );

	void Update( float dt, const Uint8* kbdStates );
	void Draw( const Camera& camPos );

	RectI GetHitBox() const;
	void ClampToRect( RectI rect );

	int GetAtk() const { return entity.GetAtk(); }
	
	void ApplyDamage( int dmg );
	Projectile Shoot();

	void CollideRect( RectF rect ) { entity.CollideRect( rect ); }
private:
	void SetDirection( const Vec2& dir );

	void Dash();
private:
	//state machine
	class Action {
	public:
		enum{
			walk,
			dash,
			shoot
		};
		//cooldown starts counting as soon as the action starts
		bool Do( int action, float actDur, float coolDown = 0.0f );
		bool IsDoing( int action );
		void Update( float dt );
	private:
		std::vector<std::pair<int, float>> active;
		std::vector<std::pair<int, float>> cooldowns;
	};

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