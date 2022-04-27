#include "Protagonist.h"

Protagonist::Protagonist( Vec2 spawnPos, SDL_Renderer* renderer )
	:
	sprite( "imgs/w_g_sprites.png", renderer ),
	entity( spawnPos, {0,0}, 32, 32, 8, 8, sprite, renderer, { 12, 20, 24, 32 } )
{}

void Protagonist::Update( float dt, const Uint8* kbd )
{
	entity.Update( dt );
	if (coolDownTimer > 0)
		coolDownTimer -= dt;
	else coolDownTimer = 0;

	Vec2 direc = { 0.0f,0.0f };

	if (kbd[SDL_SCANCODE_W]) {
		direc.y -= 1.0f;
	}
	if (kbd[SDL_SCANCODE_S]) {
		direc.y += 1.0f;
	}
	if (kbd[SDL_SCANCODE_A]) {
		direc.x -= 1.0f;
	}
	if (kbd[SDL_SCANCODE_D]) {
		direc.x += 1.0f;
	}

	SetDirection( direc );

	if (direc != Vec2( 0.0f, 0.0f )) {
		dir = direc;
	}
}

void Protagonist::SetDirection( const Vec2& dir )
{
	entity.SetDirection( dir );

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
		entity.SetAnim( animindex );
	}
}

void Protagonist::Draw()
{
	entity.Draw();
}

RectI Protagonist::GetHitBox() const
{
	return entity.GetHitBox();
}

void Protagonist::ClampToRect( RectI rect )
{
	entity.ClampToRect( rect );
}

void Protagonist::ApplyDamage( int dmg )
{
	entity.ApplyDamage( dmg );
}

Projectile Protagonist::Shoot()
{
	if (!IsOnCooldown()) {
		coolDownTimer += coolDown;
		float bulletSpeed = 100.0f;

		const Vec2 bullVel = dir.GetNormalized() * bulletSpeed;

		return Projectile( entity.GetHitBox().GetCenter(), { 256, 224 }, 32, 32, 4, 3, 0.1f,
			&sprite, sprite.GetRenderer(), { 12, 21, 21, 31 }, 200.0f, bullVel, GetAtk() );
	}
	else return Projectile::Null();
}