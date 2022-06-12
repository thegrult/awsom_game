#pragma once
#include "Background.h"
#include "Entity.h"
#include "Projectile.h"
#include "Action.h"
#include "Inventory.h"
#include "Wrld.h"

class Protagonist : public Entity{
public:
	Protagonist( Vec2 spawnPos, Surface* Sprite );

	void HandleInput( Wrld* wrld, const Uint8* kbd );
	void Update( float dt );

	void Update( float dt, const Uint8* kbdStates );
	void Draw( const Camera& camPos );
	bool Shoot( std::vector<Projectile>& projectiles );

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
	Inventory inv;
public:
	enum action{
		walk,
		dash,
		shoot,
		inventorytoggled
	};
};