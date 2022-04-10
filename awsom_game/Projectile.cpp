#include "Projectile.h"

Projectile::Projectile( const Vec2& spawnPos, const Vei2& readPos, int spritewidth, int spriteheight, int normalFramesCount, int explosionFrameCount,
	float holdTime, Surface& sprite, SDL_Renderer* renderer, RectI hitBox, float range, Vec2 velocity, int dmg )
	:
	range( range ),
	startPos( spawnPos - Vec2( (float)spritewidth / 2, (float)spriteheight / 2 ) ),
	animation( readPos, spritewidth, spriteheight, normalFramesCount, sprite, holdTime, renderer ),
	explanim( readPos + Vei2( spritewidth * normalFramesCount, 0 ), spritewidth, spriteheight, explosionFrameCount, sprite, holdTime, renderer ),
	expldur( explosionFrameCount* holdTime ),
	pos( spawnPos - Vec2( (float)spritewidth / 2, (float)spriteheight / 2 ) ),
	hitBox( hitBox ),
	vel( velocity ),
	dmg(dmg)
{}

void Projectile::Draw() const
{
	if (IsExploding())
		explanim.Draw( (Vei2)pos );
	else
		animation.Draw( (Vei2)pos );
}

void Projectile::Update( float dt )
{
	if (!ToBeRemoved()) {
		if (!isExploding) {
			pos += vel * dt;
			animation.Update( dt );

			if ((pos - startPos).LenSq() >= range * range)
				toRemove = true;
		}
		else {
			expltimer += dt;
			explanim.Update( dt );
			if (expltimer >= expldur) {
				toRemove = true;
			}
		}
	}
}

RectF Projectile::GetHitBox()
{
	if (!ToBeRemoved()) {
		return RectF( hitBox ).GetDisplaced( pos );
	}
	else
	{
		return RectF( -1.0f, +1.0f, 1.0f, -1.0f );
	}
}

void Projectile::Hits()
{
	isExploding = true;
}

 bool Projectile::ToBeRemoved() const
{
	return toRemove;
}