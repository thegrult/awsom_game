#include "Bandit.h"

Bandit::Bandit( const Vec2& spawnPos, Surface* sprite, std::vector<Projectile>& projectiles )
	:
	Entity( spawnPos, { 256,0 }, 32, 32, 8, 4, sprite, { 11, 21, 24, 32 } ),
	projectiles(&projectiles),
	sprite(sprite)
{}

void Bandit::Update( const float dt, const Vec2 & protagonistPos )
{
	Entity::Update( dt, protagonistPos );
	action.Update( dt );
	if ((GetPos() - protagonistPos).LenSq() <= detectionRadius * detectionRadius) {
		isAngry = true;
	}
	else isAngry = false;

	if (isAngry) {
		SetVel( (protagonistPos - GetPos()).GetNormalized() * speed );

		if ( IsAlive() && action.Do( action::shoot, 0.0f, 2.0f )) {
			float bulletSpeed = 100.0f;

			const Vec2 bullVel = velocity.GetNormalized() * bulletSpeed;

			projectiles->emplace_back( Projectile( GetHitBox().GetCenter(), { 256, 224 }, 32, 32, 4, 3, 0.1f,
				sprite, sprite->GetRenderer(), { 12, 21, 21, 31 }, 200.0f, bullVel, GetAtk(), false ) );
		}
	}
	else SetVel( { 0.0f,0.0f } );
}