#include "Projectile.h"

Projectile::Projectile( const Vec2& spawnPos, const Vei2& readPos, int spritewidth, int spriteheight, int normalFramesCount, int explosionFrameCount,
	float holdTime, Surface* sprite, SDL_Renderer* renderer, const RectI& hitBox, float range, const Vec2& velocity, float dmg, bool isFriend )
	:
	hitBox( hitBox ),
	range( range ),
	startPos( spawnPos - (Vec2)hitBox.GetCenter() ),
	animation( readPos, spritewidth, spriteheight, normalFramesCount, sprite, holdTime, renderer ),
	explanim( readPos + Vei2( spritewidth * normalFramesCount, 0 ), spritewidth, spriteheight, explosionFrameCount, sprite, holdTime, renderer ),
	expldur( explosionFrameCount* holdTime ),
	pos( startPos ),
	vel( velocity ),
	dmg(dmg),
	isFriend(isFriend)
{}

void Projectile::HandleInput( Wrld* wrld )
{
	if (IsFriend()) {
		RectF phitbox = GetHitBox();
		const auto enemies = wrld->GetEntitiesConst();
		for (auto e : *enemies) {
			if (e->GetHitBox().IsOverlappingWith( phitbox )) {
				Hits();
				wrld->PlaySnd( Wrld::Sounds::sfxexplosion );
			}
		}
	}
	else {
		//MAY BE VERY DANGEROUS AND GO TEERRIBLY WRONGGG
		const auto prot = reinterpret_cast<const Entity*>(wrld->GetProtagonistConst());
		if (GetHitBox().IsOverlappingWith( prot->GetHitBox() )) {
			Hits();
			wrld->PlaySnd( Wrld::Sounds::sfxexplosion );
		}
	}

	auto bg = wrld->GetBackandForeGround().first;

	if (bg->IsColliding( GetHitBox() ))
	{
		Hits();
		wrld->PlaySnd( Wrld::Sounds::sfxexplosion );
	}
	auto fg = wrld->GetBackandForeGround().second;

	if (fg->IsColliding( GetHitBox() ))
	{
		Hits();
		wrld->PlaySnd( Wrld::Sounds::sfxexplosion );
	}
}

Drawable Projectile::GetDrawable( const Camera& cam ) const
{
	const auto camPos = cam.GetPos();
	if (IsExploding())
		return explanim.CreateDrawable( (Vei2)pos - camPos, 0xffffffff );
	else
		return animation.CreateDrawable( (Vei2)pos - camPos, 0xffffffff );
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

RectF Projectile::GetHitBox() const
{
	if (!ToBeRemoved()) {
		return RectF( hitBox ).GetDisplaced( pos );
	}
	else
	{
		return RectF( +1.0f, -1.0f, +1.0f, -1.0f );
	}
}

void Projectile::Hits()
{
	isExploding = true;
}

bool Projectile::operator==( const Projectile& rhs ) const
{
	return startPos == rhs.startPos
		&& pos == rhs.startPos
		&& range == rhs.range
		&& vel == rhs.vel
		&& isExploding == rhs.isExploding
		&& toRemove == rhs.toRemove
		&& dmg == rhs.dmg
		;
}

 bool Projectile::ToBeRemoved() const
{
	return toRemove;
}