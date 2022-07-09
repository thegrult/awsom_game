#include "Bandit.h"

Bandit::Bandit( const Vec2& spawnPos, Surface* sprite )
	:
	Entity( spawnPos, { 256,0 }, 32, 32, 8, 4, sprite, { 11, 21, 24, 32 } ),
	sprite(sprite)
{}

void Bandit::HandleInput( Wrld* wrld )
{
	const auto protagonist = static_cast<const Entity*>(wrld->GetProtagonistConst());
	if (((GetPos() - protagonist->GetPos()).LenSq() <= detectionRadius * detectionRadius) && protagonist->IsAlive()) {
		isAngry = true;
	}
	else isAngry = false;

	if (isAngry) {
		const auto dir = (protagonist->GetPos() - GetPos()).GetNormalized();
		SetVel( dir * speed );
		SetDirection( dir );

		if (IsAlive() && action.Do( action::shoot, 0.0f, 2.0f )) {
			float bulletSpeed = 100.0f;

			//dividing by speed instead of normalizing because we just set the velocity and we're sure it's normal, may have to change later
			const Vec2 bullVel = velocity/speed * bulletSpeed;

			wrld->SpawnBullet( new Projectile( GetHitBox().GetCenter(), { 0, 288 }, 32, 32, 4, 3, 0.1f,
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

	{
		const auto proj = wrld->GetProjConst();
		const auto hbx = GetHitBox();
		for (const auto p : *proj)
		{
			if (p->IsFriend() && !p->IsExploding() && hbx.IsOverlappingWith( p->GetHitBox() )) {
				ApplyDamage( p->GetDmg() );
			}
		}
	}
}

void Bandit::Update( const float dt )
{
	action.Update( dt );
	Entity::Update( dt );
}

void Bandit::SetDirection( const Vec2& dir )
{
	//selects the animation based on direction (right = +1, left = +2, down = +3, up = +6), when dmged +9 
	int animindex = 0;

	if (dir.y < -0.05f) {
		animindex += 6;
	}
	else if (dir.y > 0.05f) {
		animindex += 3;
	}
	if (dir.x < -0.05f) {
		animindex += 2;
	}
	else if (dir.x > 0.05f) {
		animindex += 1;
	}

	if (animindex != 0) {
		SetAnim( animindex );
	}
}