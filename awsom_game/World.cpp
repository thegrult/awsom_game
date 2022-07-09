#include "World.h"

World::World( SDL_Renderer* gRenderer, const Uint8* kbd, const Uint32& mouseKeys, const Vei2& mousePos )
	:
	cam( { 0,0 }, RectI( { 0,0 }, SCREEN_WIDTH, SCREEN_HEIGHT ), RectI( { 0,0 }, LEVEL_WIDTH, LEVEL_HEIGHT ) ),
	kbd(kbd),
	mouseKeys(mouseKeys),
	mousePos(mousePos)
{
	spriteSheet = new Surface;
	spriteSheet->SetRenderer( gRenderer );
	if (!spriteSheet->LoadData( "imgs\\sprites.png" ))
	{
		OutputDebugStringA( "Failed to load sprite sheet texture!\n" );
	}

	backgroundsheet = new Surface;
	backgroundsheet->SetRenderer( gRenderer );
	if (!backgroundsheet->LoadData( "imgs\\bgtiles.png" ))
	{
		OutputDebugStringA( "Failed to load background sheet texture!\n" );
	}

	music = Mix_LoadMUS( "audio\\SuperMarioBros.wav" );
	if (music == NULL)
	{
		OutputDebugStringA( (std::string( "Failed to load music! SDL_mixer Error: \n" ) + Mix_GetError()).c_str() );
	}

	sfxshoot = Mix_LoadWAV( "audio\\smb_fireball.wav" );
	if (sfxshoot == NULL)
	{
		OutputDebugStringA( (std::string( "Failed to load shooting sfx! SDL_mixer Error: \n" ) + Mix_GetError()).c_str() );
	}

	sfxexplosion = Mix_LoadWAV( "audio\\smb_fireworks.wav" );
	if (sfxexplosion == NULL)
	{
		OutputDebugStringA( (std::string( "Failed to load explosion sfx! SDL_mixer Error: \n" ) + Mix_GetError()).c_str() );
	}

	elia = new Protagonist( { float( SCREEN_WIDTH / 2 ), float( SCREEN_HEIGHT / 2 ) }, spriteSheet );

	std::ifstream bgs( "resources\\bgmap.txt" );

	bg = new Background( backgroundsheet, 32, 32, { 0,0 }, { 0,0 }, LEVEL_WIDTH / 32, LEVEL_HEIGHT / 32, bgs );

	std::ifstream fgs( "resources\\fgmap.txt" );

	fg = new Background( backgroundsheet, 32, 32, { 0,0 }, { 0,0 }, LEVEL_WIDTH / 32, LEVEL_HEIGHT / 32, fgs );

	for (int i = 0; i < nEnemies; i++) {
		enemies.push_back( new Bandit( { xDist( rng ), yDist( rng ) }, spriteSheet ) );
	}

	PlaySnd( Wrld::Sounds::music );
}

World::~World()
{
	delete spriteSheet;
	delete backgroundsheet;
	Mix_FreeChunk( sfxshoot );
	Mix_FreeChunk( sfxexplosion );
	sfxshoot = NULL;
	sfxexplosion = NULL;
}

void World::Update( const float dt )
{
	elia->Update( dt );
	for (Entity* e : enemies) {
		e->Update( dt );
	}
	for (auto p : projectiles) {
		p->Update( dt );
	}

	util::remove_erase_if( projectiles,
		[]( Projectile* p ) {
			return p->ToBeRemoved();
		} );

	util::remove_erase_if( enemies,
		[]( Entity* e ) {
			return e->IsDead();
		} );
}

void World::ProcessInput()
{
	elia->HandleInput( this );
	cam.CenterOnPoint( (Vei2)elia->GetPos() );
	for (auto e : enemies) {
		e->HandleInput( this );
	}
	for (auto p : projectiles)
	{
		p->HandleInput( this );
	}

	if (enemies.size() < nEnemies)
	{
		enemies.push_back( new Bandit( { xDist( rng ), yDist( rng ) }, spriteSheet ) );
	}
}

void World::Render() const
{
	std::vector<Drawable> drawables;
	bg->Draw( cam );
	drawables.push_back( elia->GetDrawable( cam ) );
	for (const auto e : enemies)
	{
		drawables.push_back( e->GetDrawable( cam ) );
	}
	for (const auto p : projectiles) {
		drawables.push_back( p->GetDrawable( cam ) );
	}

	std::sort( drawables.begin(), drawables.end(),
		[]( const Drawable& d1, const Drawable& d2 ) {
			return d2.GetY() > d1.GetY();
		} );

	for (const auto& d : drawables) {
		d.Draw();
	}

	fg->Draw( cam );

	elia->DrawInv();
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

void World::SpawnBullet( Projectile* p )
{
	projectiles.push_back( std::move( p ) );
}

const std::vector<Projectile*>* World::GetProjConst() const
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

const Uint8* World::GetKbd() const
{
	return kbd;
}

const Uint32& World::GetMouseKeys() const
{
	return mouseKeys;
}

const Vei2& World::GetMousePos() const
{
	return mousePos;
}
