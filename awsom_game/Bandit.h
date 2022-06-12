#pragma once
#include "Entity.h"
#include "Projectile.h"
#include "Protagonist.h"
#include "Action.h"

class Bandit : public Entity {
public:
	enum action{
		walk,
		dash,
		shoot
	};
public:
	Bandit( const Vec2& spawnPos, Surface* sprite, std::vector<Projectile>& projectiles );

	void Update( const float dt, const Vec2& protagonistPos ) override;

	void HandleInput( Wrld* wrld ) override;
	void Update( const float dt ) override;
private:
	static constexpr float speed = 50.0f;
	static constexpr float detectionRadius = 150.0f;
	bool isAngry = false;
	Action action;
	std::vector<Projectile>* projectiles;
	Surface* sprite;
};