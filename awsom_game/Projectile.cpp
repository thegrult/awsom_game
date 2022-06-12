#include "Projectile.h"

Projectile::Projectile( const Vec2& spawnPos, const Vei2& readPos, int spritewidth, int spriteheight, int normalFramesCount, int explosionFrameCount,
	float holdTime, Surface* sprite, SDL_Renderer* renderer, const RectI& hitBox, float range, const Vec2& velocity, int dmg, bool isFriend )
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
			if (GetHitBox().IsOverlappingWith( phitbox )) {
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

	{
		auto bgobs = wrld->GetBackandForeGround().second->GetObstacles();
		auto hbx = GetHitBox();

		for (auto ob : bgobs) {
			if (ob.IsOverlappingWith( hbx )) {
				Hits();
			}
		}
	}
}

void Projectile::Draw( const Camera& cam ) const
{
	const auto camPos = cam.GetPos();
	if (IsExploding())
		explanim.Draw( (Vei2)pos - camPos );
	else
		animation.Draw( (Vei2)pos - camPos );

#ifdef _DEBUG
	const auto j = GetHitBox().GetDisplaced( (Vec2)-camPos );
	SDL_Rect HitBox = { (int)j.TopLeft().x, (int)j.TopLeft().y, (int)j.GetDim().x, (int)j.GetDim().y };
	SDL_Renderer* renderer = animation.GetRenderer();
	SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );
	SDL_RenderDrawRect( renderer, &HitBox );
#endif
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
		return RectF( -1.0f, +1.0f, 1.0f, -1.0f );
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

 Projectile Projectile::Null()
 {
	 return Projectile( Vec2( -1.0f,-1.0f ), Vei2( -1,-1 ), -1, -1, -1, -1, -1.0f, nullptr, nullptr, {-1,-1,-1,-1}, -1.0f, {0.0f, 0.0f}, -1, false);
 }

 bool Projectile::IsNull( Projectile p )
 {
	 return p == Null();
}