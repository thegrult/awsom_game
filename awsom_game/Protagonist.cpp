#include "Protagonist.h"

Protagonist::Protagonist( Vec2 spawnPos, SDL_Renderer* renderer )
	:
	sprite( "imgs/w_g_sprites.png", renderer ),
	entity( spawnPos, {0,0}, 32, 32, 8, 8, sprite, renderer, { 12, 20, 24, 32 } )
{}

void Protagonist::Update( float dt )
{
	entity.Update( dt );
}

void Protagonist::SetDirection( const Vec2& dir )
{
	entity.SetDirection( dir );

	if (dir.y < 0) {
		entity.SetAnim( 4 );
	}
	else if (dir.y > 0) {
		entity.SetAnim( 0 );
	}
	else if (dir.x < 0) {
		entity.SetAnim( 6 );
	}
	else if (dir.x > 0) {
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

void Protagonist::ApplyDamage()
{
	entity.ApplyDamage();
}
