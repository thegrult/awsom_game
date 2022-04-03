#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Window.h"
#include "Entity.h"
#include "Protagonist.h"
#include "FrameTimer.h"
#include <random>

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
	//Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	//The window we'll be rendering to
	Window window;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//sounds and music
	Mix_Music* music = NULL;

	Mix_Chunk* sfxshoot = NULL;
	Mix_Chunk* sfxexplosion = NULL;
	//Event handler
	SDL_Event e;

	//rnd int generator
	std::mt19937 rng{ std::random_device()() };
	std::uniform_real_distribution<float> xDist = std::uniform_real_distribution<float>(0.0f, float(SCREEN_WIDTH));
	std::uniform_real_distribution<float> yDist = std::uniform_real_distribution<float>(0.0f, float(SCREEN_HEIGHT));

	//utilities
	FrameTimer ft;
	Surface spriteSheet;
private:
	//actual game stuff
	Protagonist* elia;

	std::vector<Entity> enemies;

	std::vector<Projectile> projectiles;

	const int nEnemies = 3;
};