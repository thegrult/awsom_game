#include "Protagonist.h"

Protagonist::Protagonist( Vec2 spawnPos, Surface* sprite )
	:
	Entity( spawnPos, {0,0}, 32, 32, 8, 9, sprite, { 12, 20, 24, 32 } ),
	sprite(sprite),
	inv( sprite->GetRenderer(), mode )
{}

void Protagonist::HandleInput( Wrld* wrld )
{
	if (IsAlive()) {
		this->wrld = wrld;
		inv.HandleInput( wrld );
		const auto kbd = wrld->GetKbd();
		if (action.IsDoing( action::walk ) && !action.IsDoing( action::dash )) {
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

			SetVel( Vec2( direc ).GetNormalized() * walkingSpeed );
			SetDirection( (Vec2)direc );

			if (direc != Vei2( 0, 0 )) {
				dir = (Vec2)direc;
			}

			if (kbd[SDL_SCANCODE_LSHIFT]) {
				Do( dash );
			}
		}
		else action.Do( action::walk, 1.0f );

		if (kbd[SDL_SCANCODE_SPACE]) {
			Do( shoot );
		}

		if (kbd[SDL_SCANCODE_E]) {
			Do( inventorytoggled );
		}

		if (kbd[SDL_SCANCODE_C]) {
			combo.Start();
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

		{
			const auto proj = wrld->GetProjConst();
			const auto hbx = GetHitBox();
			for (const auto p : *proj)
			{
				if (!p->IsFriend() && !p->IsExploding() && hbx.IsOverlappingWith( p->GetHitBox() )) {
					ApplyDamage( p->GetDmg() );
				}
			}
		}
	}
}

void Protagonist::Update( float dt )
{
	action.Update( dt );
	Entity::Update( dt );
	inv.Update( dt );
	combo.Update( dt, *this );
}

void Protagonist::Do( int a )
{
	switch (a)
	{
	case setModeMelee:
		ChangeMode( McMode::Modes::melee );
		break;
	case setModeRanged:
		ChangeMode( McMode::Modes::ranged );
		break;
	case walk:
		action.Do( walk, 0.01f, coolDowns[walk] );
		break;
	case dash:
		Dash();
		break;
	case shoot:
		Shoot( wrld );
		break;
	case inventorytoggled:
		if( action.Do( action::inventorytoggled, 0.0f, 0.5f ))
		inv.ToggleShown();
		break;
	}
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
		SetAnim( animindex );
	}
}

void Protagonist::Dash()
{
	if (action.Do( action::dash, 0.4f, coolDowns[action::dash] )) {
		ApplyInvincibility( 0.2f );
		SetVel( dir.GetNormalized() * rollSpeed * mode.DiscernMode( McMode::Modes::ranged, 0.8f, 1.0f ) );
	}
}

void Protagonist::Shoot( Wrld* wrld )
{
	if (action.Do( action::shoot, 0.0f, coolDowns[action::shoot] )) {
		float bulletSpeed = 100.0f;

		const Vec2 bullVel = dir.GetNormalized() * bulletSpeed;

		wrld->SpawnBullet( new Projectile( GetHitBox().GetCenter(), { 256, 224 }, 32, 32, 4, 3, 0.1f,
			sprite, sprite->GetRenderer(), { 12, 21, 21, 31 }, 200.0f * mode.DiscernMode( McMode::Modes::ranged, 1.5f, 1.0f ), bullVel, Entity::GetAtk() * mode.DiscernMode( McMode::Modes::ranged, 2.0f, 1.0f ), true ) );
		wrld->PlaySnd( Wrld::Sounds::sfxshoot );
	}
}

void Protagonist::ChangeMode( McMode::Modes mode )
{
	inv.SetMode( mode );
}

void Protagonist::Draw( const Camera& camPos )
{
	Entity::Draw( camPos );
	inv.Draw( { 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT } );
}

float Protagonist::GetAtk() const
{
	return Entity::GetAtk() * mode.DiscernMode(McMode::Modes::melee, 4, 1 );
}

Protagonist::Combo::Combo( const std::vector<std::pair<int, float>>& moves )
	:
	moves(moves)
{}

void Protagonist::Combo::AddMove( int move, float waitTime )
{
	moves.push_back( { move, waitTime } );
}

void Protagonist::Combo::Update( float dt, Protagonist& mc )
{
	if (active) {
		queue[0].second -= dt;
		if (queue[0].second <= 0.0f) {
			queue.erase( queue.begin() );
			if (queue.size() > 0) {
				mc.Do( queue[0].first );
			}
			else active = false;
		}
	}
}

void Protagonist::Combo::Start()
{
	if (!active) {
		active = true;
		queue = moves;
	}
}
