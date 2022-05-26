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

	const int LEVEL_WIDTH = 1024;
	const int LEVEL_HEIGHT = 1024;

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
	std::uniform_real_distribution<float> xDist = std::uniform_real_distribution<float>(0.0f, float(LEVEL_WIDTH));
	std::uniform_real_distribution<float> yDist = std::uniform_real_distribution<float>(0.0f, float(LEVEL_HEIGHT));

	//utilities
	FrameTimer ft;
	Surface* spriteSheet = nullptr;
	Surface* backgroundsheet = nullptr;
private:
	const int toleranceregion = -32;

	Background* bg = nullptr;
	Background* fg = nullptr;

	Camera cam;

	//actual game stuff
	Protagonist* elia = nullptr;

	std::vector<Bandit> enemies;

	std::vector<Projectile> projectiles;

	const int nEnemies = 5;
};