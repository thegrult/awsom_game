#pragma once
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Window.h"
#include "Protagonist.h"
#include "Bandit.h"
#include "FrameTimer.h"
#include "Background.h"
#include "Camera.h"
#include <random>
#include "World.h"

class Game {
public:
	Game();
	~Game();
	//returns true if quit
	bool Go();
private:
	//important functions
	bool UpdateGame(const float dt);
	void Draw();
	bool init();
	bool loadMedia();
	void close();
private:
	//helper functions
	RectI GetScreenRect() const;
private:
	//The window we'll be rendering to
	Window window;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Event handler
	SDL_Event e;

	//utilities
	SDL_Cursor* cursor = nullptr;

	FrameTimer ft;

	const Uint8* kbd = SDL_GetKeyboardState( NULL );
	Uint32 mouseKeys;
	Vei2 mousePos;
private:
	World* world;
};