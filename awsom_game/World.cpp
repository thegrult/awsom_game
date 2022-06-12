#include "World.h"

void World::Update( const float dt )
{
	elia->Update( dt );
	for (Entity* e : enemies) {
		e->Update( dt, elia->GetPos() );
	}
	for (auto p : projectiles) {
		p.Update( dt );
	}
}

void World::ProcessInput( const Uint8* kbd, const Uint32 mouseKeys, const Vec2 mousePos )
{
	elia->HandleInput( this, kbd );
	for (auto e : enemies) {
		e->HandleInput( this );
	}
}

void World::Draw() const
{
	elia->Draw( cam );
	for (const auto e : enemies)
	{
		e->Draw( cam );
	}
	for (const auto p : projectiles) {
		p.Draw( cam );
	}
}

void World::PlaySnd( Sounds s )
{
	switch ( s )
	{
	case Wrld::music:
		Mix_PlayMusic( music, -1 );
		break;
	case Wrld::sfxshoot:
		Mix_PlayChannel( -1, sfxshoot, 0 );
		break;
	case Wrld::sfxexplosion:
		Mix_PlayChannel( -1, sfxexplosion, 0 );
		break;
	default:
		OutputDebugStringA( "Error: tried playing a non-existant sound effect or music" );
		break;
	}
}

void World::SpawnBullet( Projectile&& p )
{
	projectiles.push_back( std::move( p ) );
}

const std::vector<Projectile>* World::GetProjConst() const
{
	return &projectiles;
}

const std::vector<Entity*>* World::GetEntitiesConst() const
{
	return &enemies;
}

const Protagonist* World::GetProtagonistConst() const
{
	return elia;
}

const std::pair<const Background*,const Background*> World::GetBackandForeGround() const
{
	return { bg, fg };
}
