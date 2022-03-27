#include "Game.h"

Game::Game()
{
	init();
	//BEWARE! gRenderer is null before the call to loadMedia!
	loadMedia();
	
	elia = new Protagonist( { float( SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2 )},  gRenderer );

	for (int i = 0; i < nEnemies; i++) {
		enemies.push_back( Entity{ { xDist( rng ), yDist( rng ) }, { 256,0 }, 32, 32, 8, 4, spriteSheet, gRenderer, {11, 21, 24, 32} } );
	}

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
		dt = 1.0f / 30.0f;
#else // DEBUG
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
	}



	if (keyStates[SDL_SCANCODE_UP]) {
		elia->SetDirection( { 0,-1 } );
	}
	else if (keyStates[SDL_SCANCODE_DOWN]) {
		elia->SetDirection( { 0,1 } );
	}
	else if (keyStates[SDL_SCANCODE_LEFT]) {
		elia->SetDirection( { -1,0 } );
	}
	else if (keyStates[SDL_SCANCODE_RIGHT]) {
		elia->SetDirection( { 1,0 } );
	}
	else {
		elia->SetDirection( { 0,0 } );
	}

	if (keyStates[SDL_SCANCODE_SPACE]) {
		if (!elia->IsOnCooldown())
		{
			projectiles.emplace_back( std::move( elia->Shoot() ) );
			Mix_PlayChannel( -1, sfxshoot, 0 );
		}
	}

	elia->Update( dt );

	const int toleranceregion = -50;
	elia->ClampToRect( GetScreenRect().GetExpanded(toleranceregion) );

	for (Entity&e : enemies) {
		e.Update( dt );
		e.ClampToRect( GetScreenRect().GetExpanded( toleranceregion ) );

		if (elia->GetHitBox().IsOverlappingWith( e.GetHitBox() )) {
			elia->ApplyDamage();
			e.ApplyDamage();
		}
	}

	projectiles.erase( 
		std::remove_if( 
			projectiles.begin(), projectiles.end(),
				[]( Projectile p ) {
					return p.ToBeRemoved();
				} ), projectiles.end()
	);

	for (int i = 0; i < projectiles.size(); i++) {
		
		projectiles[i].Update(dt);

		RectF phitbox = projectiles[i].GetHitBox();

		for (Entity& e : enemies) {
			if (e.GetHitBox().IsOverlappingWith( phitbox )) {
				projectiles[i].Hits();
				e.ApplyDamage();
				Mix_PlayChannel( -1, sfxexplosion, 0 );
			}
		}
	}




	return quit;
}

void Game::Draw()
{
	//Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	//draw character
	elia->Draw();

	//elia->ApplyDamage();

	for (Entity& e : enemies) {
		e.Draw();
	}

	for (const Projectile& p : projectiles) {
		p.Draw();
	}

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
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ))
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "awsom gaem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if (gWindow == NULL)
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if (gRenderer == NULL)
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
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
					printf( "SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError() );
					success = false;
				}

				//Initialize SDL_mixer
				//1st argument is sound frequency, Sample format, n of hardware channels, sample size (in bytes)
				if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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
	spriteSheet.SetRenderer( gRenderer );
	if (!spriteSheet.LoadData( "imgs/w_g_sprites.png" ))
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}

	music = Mix_LoadMUS( "audio\\SuperMarioBros.wav" );
	if (music == NULL)
	{
		printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	sfxshoot = Mix_LoadWAV( "audio/smb_fireball.wav" );
	if (sfxshoot == NULL)
	{
		printf( "Failed to load shooting sfx! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	sfxexplosion = Mix_LoadWAV( "audio/smb_fireworks.wav" );
	if (sfxexplosion == NULL)
	{
		printf( "Failed to load explosion sfx! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	return success;
}

void Game::close()
{
	//Free loaded images
	spriteSheet.FreeData();
	Mix_FreeChunk( sfxshoot );
	Mix_FreeChunk( sfxexplosion );
	sfxshoot = NULL;
	sfxexplosion = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();

	SDL_Quit();
}

RectI Game::GetScreenRect() const
{
	return RectI( { 0,0 },SCREEN_WIDTH, SCREEN_HEIGHT );
}
