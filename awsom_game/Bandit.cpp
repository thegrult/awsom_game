#include "Bandit.h"

Bandit::Bandit( const Vec2& spawnPos, Surface* sprite, std::vector<Projectile>& projectiles )
	:
	Entity( spawnPos, { 256,0 }, 32, 32, 8, 4, sprite, { 11, 21, 24, 32 } ),
	projectiles(&projectiles),
	sprite(sprite)
{}

void Bandit::Update( const float dt, const Vec2 & protagonistPos )
{
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

	Entity::Update( dt, protagonistPos );
}

void Bandit::HandleInput( Wrld* wrld )
{
	const auto protagonist = wrld->GetProtagonistConst();
	if ((GetPos() - protagonist->GetPos()).LenSq() <= detectionRadius * detectionRadius) {
		isAngry = true;
	}
	else isAngry = false;

	if (isAngry) {
		SetVel( (protagonist->GetPos() - GetPos()).GetNormalized() * speed );

		if (IsAlive() && action.Do( action::shoot, 0.0f, 2.0f )) {
			float bulletSpeed = 100.0f;

			//dividing by speed instead of normalizing because we just set the velocity and we're sure it's normal, may have to change later
			const Vec2 bullVel = velocity/speed * bulletSpeed;

			wrld->SpawnBullet( Projectile( GetHitBox().GetCenter(), { 256, 224 }, 32, 32, 4, 3, 0.1f,
				sprite, sprite->GetRenderer(), { 12, 21, 21, 31 }, 200.0f, bullVel, GetAtk(), false ) );
			wrld->PlaySnd( Wrld::Sounds::sfxshoot );
		}
	}
	else SetVel( { 0.0f,0.0f } );

	{
		if (GetHitBox().IsOverlappingWith( protagonist->GetHitBox() )) {
			ApplyDamage( protagonist->GetAtk() );
		}
	}

	{
		auto bgobs = wrld->GetBackandForeGround().first->GetObstacles();
		auto hbx = GetHitBox();

		for (auto ob : bgobs) {
			if (ob.IsOverlappingWith( hbx )) {
				CollideRect( ob );
			}
		}
	}
	{
		auto bgobs = wrld->GetBackandForeGround().second->GetObstacles();
		auto hbx = GetHitBox();

		for (auto ob : bgobs) {
			if (ob.IsOverlappingWith( hbx )) {
				CollideRect( ob );
			}
		}
	}
}

void Bandit::Update( const float dt )
{
	action.Update( dt );
	Entity::Update( dt );
}
