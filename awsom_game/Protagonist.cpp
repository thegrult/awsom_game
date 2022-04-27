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



	if (kbd[SDL_SCANCODE_UP]) {
		SetDirection( { 0,-1 } );
	}
	else if (kbd[SDL_SCANCODE_DOWN]) {
		SetDirection( { 0,1 } );
	}
	else if (kbd[SDL_SCANCODE_LEFT]) {
		SetDirection( { -1,0 } );
	}
	else if (kbd[SDL_SCANCODE_RIGHT]) {
		SetDirection( { 1,0 } );
	}
	else {
		SetDirection( { 0,0 } );
	}
}

void Protagonist::SetDirection( const Vec2& dir )
{
	entity.SetDirection( dir );

	if (dir.y < -0.05f) {
		direc = up;
		entity.SetAnim( 4 );
	}
	else if (dir.y > 0.05f) {
		direc = down;
		entity.SetAnim( 0 );
	}
	else if (dir.x < -0.05f) {
		direc = left;
		entity.SetAnim( 6 );
	}
	else if (dir.x > 0.05f) {
		direc = right;
		entity.SetAnim( 2 );
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
		Vec2 velocity = { 0.0f,0.0f };
		float bulletSpeed = 20.0f;
		if (direc == up) {
			velocity.y = -bulletSpeed;
		}
		else if (direc == down) {
			velocity.y = bulletSpeed;
		}
		else if (direc == left) {
			velocity.x = -bulletSpeed;
		}
		else if (direc == right) {
			velocity.x = bulletSpeed;
		}

		return Projectile( entity.GetHitBox().GetCenter(), { 256, 224 }, 32, 32, 4, 3, 0.1f,
			&sprite, sprite.GetRenderer(), { 12, 21, 21, 31 }, 200.0f, velocity, GetAtk() );
	}
	else return Projectile::Null();
}