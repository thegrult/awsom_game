#include "Protagonist.h"

Protagonist::Protagonist( Vec2 spawnPos, Surface* sprite )
	:
	entity( spawnPos, {0,0}, 32, 32, 8, 9, sprite, { 12, 20, 24, 32 } ),
	sprite(sprite),
	inv( sprite->GetRenderer() )
{}

void Protagonist::HandleInput( Wrld* wrld, const Uint8 * kbd )
{
	if (action.IsDoing( action::dash )) {
		entity.SetVel( dir.GetNormalized() * rollSpeed );
	}
	else if (action.IsDoing( action::walk )) {
		Vei2 direc = { 0,0 };

		if (kbd[SDL_SCANCODE_W]) {
			direc.y -= 1;
		}
		if (kbd[SDL_SCANCODE_S]) {
			direc.y += 1;
		}
		if (kbd[SDL_SCANCODE_A]) {
			direc.x -= 1;
		}
		if (kbd[SDL_SCANCODE_D]) {
			direc.x += 1;
		}

		entity.SetVel( Vec2( direc ).GetNormalized() * walkingSpeed );
		SetDirection( (Vec2)direc );

		if (direc != Vei2( 0, 0 )) {
			dir = (Vec2)direc;
		}

		if (kbd[SDL_SCANCODE_LSHIFT]) {
			Dash();
		}
	}
	else action.Do( action::walk, 1.0f );

	if (kbd[SDL_SCANCODE_SPACE]) {
		if (action.Do( action::shoot, 0.0f, 0.5f )) {
			float bulletSpeed = 100.0f;

			const Vec2 bullVel = dir.GetNormalized() * bulletSpeed;

			wrld->SpawnBullet( Projectile( entity.GetHitBox().GetCenter(), { 256, 224 }, 32, 32, 4, 3, 0.1f,
				sprite, sprite->GetRenderer(), { 12, 21, 21, 31 }, 200.0f, bullVel, GetAtk(), true ) );
		}
	}

	if (kbd[SDL_SCANCODE_E]) {
		if (action.Do( inventorytoggled, 0.0f, 0.5f )) {
			inv.ToggleShown();
		}
	}

	{
		const auto entities = *(wrld->GetEntitiesConst());
		const auto hbx = GetHitBox();

		for (auto e : entities) {
			if (hbx.IsOverlappingWith( e->GetHitBox() ))
			{
				ApplyDamage( e->GetAtk() );
			}
		}
	}

	{
		const auto bgobs = wrld->GetBackandForeGround().first->GetObstacles();
		auto hbx = GetHitBox();
		for (auto ob : bgobs) {
			if (ob.IsOverlappingWith( hbx )) {
				CollideRect( ob );
			}
		}
	}
	{
		const auto bgobs = wrld->GetBackandForeGround().second->GetObstacles();
		auto hbx = GetHitBox();
		for (auto ob : bgobs) {
			if (ob.IsOverlappingWith( hbx )) {
				CollideRect( ob );
			}
		}
	}
}

void Protagonist::Update( float dt )
{
	action.Update( dt );
	entity.Update( dt, GetPos() );
}

void Protagonist::Update( float dt, const Uint8* kbd )
{
	action.Update( dt );
	entity.Update( dt, GetPos() );

	if (action.IsDoing( action::dash )) {
		entity.SetVel( dir.GetNormalized() * rollSpeed );
	}
	else if (action.IsDoing( action::walk )) {
		Vei2 direc = { 0,0 };

		if (kbd[SDL_SCANCODE_W]) {
			direc.y -= 1;
		}
		if (kbd[SDL_SCANCODE_S]) {
			direc.y += 1;
		}
		if (kbd[SDL_SCANCODE_A]) {
			direc.x -= 1;
		}
		if (kbd[SDL_SCANCODE_D]) {
			direc.x += 1;
		}

		entity.SetVel( Vec2( direc ).GetNormalized() * walkingSpeed );
		SetDirection( (Vec2)direc );

		if (direc != Vei2( 0, 0 )) {
			dir = (Vec2)direc;
		}

		if (kbd[SDL_SCANCODE_LSHIFT]) {
			Dash();
		}
	}
	else action.Do( action::walk, 1.0f );

	if (kbd[SDL_SCANCODE_E]) {
		if (action.Do( inventorytoggled, 0.0f, 0.5f )) {
			inv.ToggleShown();
		}
	}
}

Vec2 Protagonist::GetPos() const
{
	return GetHitBox().GetCenter();
}

void Protagonist::SetDirection( const Vec2& dir )
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
		entity.SetAnim( animindex );
	}
}

void Protagonist::Dash()
{
	if (action.Do( action::dash, 0.4f, 1.0f )) {
		entity.ApplyInvincibility( 0.2f );
	}
}

void Protagonist::Draw( const Camera& camPos )
{
	entity.Draw( camPos );
	inv.Draw( { 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT } );
}

RectF Protagonist::GetHitBox() const
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

bool Protagonist::Shoot( std::vector<Projectile>& projectiles )
{
	if (action.Do( action::shoot, 0.0f, 0.5f )) {
		float bulletSpeed = 100.0f;

		const Vec2 bullVel = dir.GetNormalized() * bulletSpeed;

		projectiles.emplace_back( Projectile( entity.GetHitBox().GetCenter(), { 256, 224 }, 32, 32, 4, 3, 0.1f,
			sprite, sprite->GetRenderer(), { 12, 21, 21, 31 }, 200.0f, bullVel, GetAtk(), true ) );
		return true;
	}
	else return false;
}