#include "Protagonist.h"

Protagonist::Protagonist( Vec2 spawnPos, Surface* sprite, SDL_Renderer* renderer )
	:
	entity( spawnPos, {0,0}, 32, 32, 8, 9, sprite, renderer, { 12, 20, 24, 32 } ),
	sprite(sprite)
{}

void Protagonist::Update( float dt, const Uint8* kbd )
{
	action.Update( dt );
	entity.Update( dt );

	if (action.IsDoing( Action::dash )) {
		entity.SetVel( dir.GetNormalized() * rollSpeed );
	}
	else if (action.IsDoing( Action::walk )) {
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
	else action.SetAction( Action::walk, 1.0f );
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
	if (action.SetAction( Action::dash, 0.2f, 1.0f )) {
		entity.ApplyInvincibility( 0.2f );
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
	if (action.SetAction( Action::shoot, 0.0f, 0.5f )) {
		float bulletSpeed = 100.0f;

		const Vec2 bullVel = dir.GetNormalized() * bulletSpeed;

		return Projectile( entity.GetHitBox().GetCenter(), { 256, 224 }, 32, 32, 4, 3, 0.1f,
			sprite, sprite->GetRenderer(), { 12, 21, 21, 31 }, 200.0f, bullVel, GetAtk() );
	}
	else return Projectile::Null();
}

bool Protagonist::Action::SetAction( int act, float actDur, float cooldown )
{
	bool valid = true;
	for (auto a : cooldowns) {
		valid = valid && a.first != act;
	}
	if (valid) {
		active.push_back( { act, actDur } );
		cooldowns.push_back( { act, cooldown } );
	}
	return valid;
}

bool Protagonist::Action::IsDoing( int action )
{
	return std::any_of( active.begin(), active.end(), [&action](std::pair<int,float> p)
		{
			return p.first == action;
		}
	);
}

void Protagonist::Action::Update( float dt )
{
	for (int i = 0; i < cooldowns.size(); i++) {
		cooldowns[i].second -= dt;
		if (cooldowns[i].second <= 0.0f) {
			cooldowns.erase( cooldowns.begin() + i );
		}
	}
	for (int i = 0; i < active.size(); i++) {
		active[i].second -= dt;
		if (active[i].second <= 0.0f) {
			active.erase( active.begin() + i );
		}
	}
}