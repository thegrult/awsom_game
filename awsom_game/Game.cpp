#include "Game.h"

Game::Game()
{
	assert( SCREEN_WIDTH < LEVEL_WIDTH );
	assert( SCREEN_HEIGHT < LEVEL_HEIGHT );
	init();
	//BEWARE! gRenderer is null before the call to loadMedia!
	loadMedia();

	world = new World( gRenderer, kbd, mouseKeys, mousePos );
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

	const Uint8* kbd = SDL_GetKeyboardState( NULL );
	mouseKeys = SDL_GetMouseState( &mousePos.x, &mousePos.y );

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
		if (window.handleEvent( e ))
			world->AdaptCam( window );
	}

	world->ProcessInput();
	world->Update( dt );

	return quit;
}

void Game::Draw()
{
	//Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xff );
	SDL_RenderClear( gRenderer );

	world->Render();

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

	auto cursorSprite = IMG_Load( "imgs\\roundcursor.png" );
	if (cursorSprite == NULL) {
		OutputDebugStringA( "Failed to load cursor texture" );
		success = false;
	}
	else
	{
		//Color key image
		SDL_SetColorKey( cursorSprite, SDL_TRUE, SDL_MapRGB( cursorSprite->format, 0, 0xFF, 0xFF ) );

		//create new cursor from sprite and set it
		cursor = SDL_CreateColorCursor( cursorSprite, 2, 2 );
		SDL_SetCursor( cursor );
	}

	return success;
}

void Game::close()
{
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