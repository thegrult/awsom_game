#include "Game.h"

Game::Game()
	:
	cam( GetScreenRect().GetCenter(), RectI({0,0}, SCREEN_WIDTH, SCREEN_HEIGHT), RectI( {0,0}, LEVEL_WIDTH, LEVEL_HEIGHT) )
{
	init();
	//BEWARE! gRenderer is null before the call to loadMedia!
	loadMedia();
	
	elia = new Protagonist( { float( SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2 )}, spriteSheet );

	std::ifstream bgs( "resources\\bgmap.txt" );

	bg = new Background( backgroundsheet, 32, 32, { 0,0 }, { 0,0 }, LEVEL_WIDTH/32, LEVEL_HEIGHT/32, bgs );

	std::ifstream fgs( "resources\\fgmap.txt" );

	fg = new Background( backgroundsheet, 32, 32, { 0,0 }, { 0,0 }, LEVEL_WIDTH/32, LEVEL_HEIGHT/32, fgs );

	for (int i = 0; i < nEnemies; i++) {
		enemies.push_back( Bandit({ xDist( rng ), yDist( rng ) }, spriteSheet, projectiles ) );
	}

	//starts music with indefinite loops
	Mix_PlayMusic( music, -1 );
}

Game::~Game()
{
	close();
}

bool Game::Go()
{
#ifdef _DEBUG
	float dt = ft.Mark();
	if (dt > 1.0f / 30.0f)
		dt = 0.001f;
#else // RELEASE
	const float dt = ft.Mark();
#endif
	bool quit = UpdateGame( dt );
	Draw();
	return quit;
}

bool Game::UpdateGame( const float dt )
{
	bool quit = false;

	const Uint8* keyStates = SDL_GetKeyboardState( NULL );
	//Handle events on queue
	while (SDL_PollEvent( &e ) != 0)
	{
		switch (e.type) {
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
			break;
		case SDL_QUIT:
			quit = true;
			break;
		}
		window.handleEvent( e );
	}

	elia->Update( dt, keyStates );

	if ( keyStates[SDL_SCANCODE_SPACE] ) {
		if ( elia->Shoot( projectiles ) )
		{
			Mix_PlayChannel( -1, sfxshoot, 0 );
		}
	}

	for (Entity&e : enemies) {

		const auto projectilesNOld = projectiles.size();
		e.Update( dt, elia->GetPos() );

		if (projectilesNOld != projectiles.size()) {
			Mix_PlayChannel( -1, sfxshoot, 0 );
		}

		if (elia->GetHitBox().IsOverlappingWith( e.GetHitBox() )) {
			elia->ApplyDamage(e.GetAtk());
			e.ApplyDamage(elia->GetAtk());
		}

		{
			auto bgobs = bg->GetObstacles();
			auto hbx = e.GetHitBox();

			for (auto ob : bgobs) {
				if (ob.IsOverlappingWith( hbx )) {
					//e.ApplyDamage( 1 );
					e.CollideRect( ob );
				}
			}
		}
		{
			auto bgobs = fg->GetObstacles();
			auto hbx = e.GetHitBox();

			for (auto ob : bgobs) {
				if (ob.IsOverlappingWith( hbx )) {
					//e.ApplyDamage( 1 );
					e.CollideRect( ob );
				}
			}
		}
	}

	//collision with back and foreground
	{
		const auto bgobs = bg->GetObstacles();
		auto hbx = elia->GetHitBox();
		for (auto ob : bgobs) {
			if (ob.IsOverlappingWith( hbx )) {
				elia->CollideRect( ob );
			}
		}
	}
	{
		const auto bgobs = fg->GetObstacles();
		auto hbx = elia->GetHitBox();
		for (auto ob : bgobs) {
			if (ob.IsOverlappingWith( hbx )) {
				elia->CollideRect( ob );
			}
		}
	}


	for (auto p = projectiles.begin(); p != projectiles.end(); p++)
	{
		p->Update(dt);

		if (p->IsFriend()) {
			RectF phitbox = p->GetHitBox();

			for (Entity& e : enemies) {
				if (e.GetHitBox().IsOverlappingWith( phitbox )) {
					p->Hits();
					e.ApplyDamage( p->GetDmg() );
					Mix_PlayChannel( -1, sfxexplosion, 0 );
				}
			}
		}
		else {
			if (p->GetHitBox().IsOverlappingWith( elia->GetHitBox() )) {
				p->Hits();
				elia->ApplyDamage( p->GetDmg() );
				Mix_PlayChannel( -1, sfxexplosion, 0 );
			}
		}
	}

	util::remove_erase_if( projectiles, 
		[]( Projectile p ) {
		return p.ToBeRemoved();
		} );

	util::remove_erase_if( enemies,
		[]( Entity e ) {
			return e.IsDead();
		} );

	cam.CenterOnPoint( (Vei2)elia->GetHitBox().GetCenter() );

	return quit;
}

void Game::Draw()
{
	//Clear screen
	SDL_RenderClear( gRenderer );

	bg->Draw( cam );

	for (Entity& e : enemies) {
		e.Draw( cam );
	}

	//draw character
	elia->Draw( cam );

	for (const Projectile& p : projectiles) {
		p.Draw( cam );
	}

	fg->Draw( cam );
	//Update screen
	SDL_RenderPresent( gRenderer );
}

bool Game::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0)
	{
		OutputDebugStringA( (std::string("SDL could not initialize! SDL Error: \n") + SDL_GetError()).c_str() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ))
		{
			OutputDebugStringA( "Warning: Linear texture filtering not enabled! \n" );
		}

		//Create window
		if (!window.init( SCREEN_WIDTH, SCREEN_HEIGHT ))
		{
			OutputDebugStringA( (std::string( "Window could not be created! SDL Error: \n") + SDL_GetError()).c_str() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = window.createRenderer();
			if (gRenderer == NULL)
			{
				OutputDebugStringA( (std::string("Renderer could not be created! SDL Error: \n") + SDL_GetError()).c_str());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init( imgFlags ) & imgFlags))
				{
					OutputDebugStringA( (std::string( "SDL_image could not initialize! SDL_mage Error: \n") + IMG_GetError()).c_str() );
					success = false;
				}

				//Initialize SDL_mixer
				//1st argument is sound frequency, Sample format, n of hardware channels, sample size (in bytes)
				if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)
				{
					OutputDebugStringA( (std::string( "SDL_mixer could not initialize! SDL_mixer Error: \n") + Mix_GetError()).c_str() );
					success = false;
				}
			}
		}
	}

	

	return success;
}

bool Game::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	spriteSheet = new Surface;
	spriteSheet->SetRenderer( gRenderer );
	if (!spriteSheet->LoadData( "imgs\\sprites.png" ))
	{
		OutputDebugStringA( "Failed to load sprite sheet texture!\n" );
		success = false;
	}

	backgroundsheet = new Surface;
	backgroundsheet->SetRenderer( gRenderer );
	if (!backgroundsheet->LoadData( "imgs\\bgtiles.png" ))
	{
		OutputDebugStringA( "Failed to load background sheet texture!\n" );
		success = false;
	}

	music = Mix_LoadMUS( "audio\\SuperMarioBros.wav" );
	if (music == NULL)
	{
		OutputDebugStringA( (std::string( "Failed to load music! SDL_mixer Error: \n") + Mix_GetError()).c_str() );
		success = false;
	}

	sfxshoot = Mix_LoadWAV( "audio\\smb_fireball.wav" );
	if (sfxshoot == NULL)
	{
		OutputDebugStringA( (std::string( "Failed to load shooting sfx! SDL_mixer Error: \n") + Mix_GetError()).c_str() );
		success = false;
	}

	sfxexplosion = Mix_LoadWAV( "audio\\smb_fireworks.wav" );
	if (sfxexplosion == NULL)
	{
		OutputDebugStringA( (std::string( "Failed to load explosion sfx! SDL_mixer Error: \n") + Mix_GetError()).c_str() );
		success = false;
	}

	return success;
}

void Game::close()
{
	//Free loaded images
	delete spriteSheet;
	delete backgroundsheet;
	Mix_FreeChunk( sfxshoot );
	Mix_FreeChunk( sfxexplosion );
	sfxshoot = NULL;
	sfxexplosion = NULL;

	//Destroy window
	window.free();

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();

	SDL_Quit();
}

RectI Game::GetScreenRect() const
{
	return window.GetWindowRect();
}