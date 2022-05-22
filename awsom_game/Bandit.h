#pragma once
#include "Entity.h"
#include "Projectile.h"
#include "Action.h"

class Bandit : public Entity {
public:
	Bandit( const Vec2& spawnPos, Surface* sprite, std::vector<Projectile>& projectiles );

	void Update( const float dt, const Vec2& protagonistPos ) override;
private:
	static constexpr float speed = 50.0f;
	static constexpr float detectionRadius = 150.0f;
	bool isAngry = false;
	Action action;
	std::vector<Projectile>* projectiles;
	Surface* sprite;
};