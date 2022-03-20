#include "Game.h"

Game::Game()
{
	init();
	//BEWARE! gRenderer is null before the call to loadMedia!
	loadMedia();
	
	elia = new Protagonist( { float( SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2 )},  gRenderer );

	for (int i = 0; i < nEnemies; i++) {
		//for some reason while in debug mode starts reading a couple of pixels to the left, or maybe it's the other way around...
		enemies.push_back( Entity{ { xDist( rng ), yDist( rng ) }, { 256,0 }, 32, 32, 8, 4, spriteSheet, gRenderer, {11, 21, 24, 32} } );
	}
}

Game::~Game()
{
	close();
}

bool Game::Go()
{
	const float dt = ft.Mark();
	//const float dt = 1.0f / 60.0f;

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
	
	if (keyStates[SDL_SCANCODE_SPACE])
	{

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

	//Update screen
	SDL_RenderPresent( gRenderer );
}

bool Game::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO ) < 0)
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
	else
	{
		//do stuff
	}

	return success;
}

void Game::close()
{
	//Free loaded images
	spriteSheet.FreeData();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

RectI Game::GetScreenRect() const
{
	return RectI( { 0,0 },SCREEN_WIDTH, SCREEN_HEIGHT );
}
