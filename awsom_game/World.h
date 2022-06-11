#pragma once
#include "Wrld.h"
#include "Protagonist.h"
#include "Background.h"
#include "Bandit.h"
#include "Vec2.h"
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

class World : public Wrld{
public:
	void Update( const float dt );
	void ProcessInput( const Uint8* kbd, const Uint32 mouseKeys, const Vec2 mousePos );
	void PlaySnd( Sounds s ) override;
	void SpawnBullet( class Projectile& p ) override;

	const std::vector<Projectile>* GetProjConst() const;
	const std::vector<Entity*>* GetEntitiesConst() const;
	const Protagonist* GetProtagonistConst() const;
private:
	//entities
	Protagonist* elia = nullptr;

	std::vector<Entity*> enemies;

	std::vector<Projectile> projectiles;

	//other game stuff
	Background* bg = nullptr;
	Background* fg = nullptr;

	Camera cam;

	//sprites
	Surface* spriteSheet = nullptr;
	Surface* backgroundsheet = nullptr;

	//sounds and music
	Mix_Music* music = NULL;

	Mix_Chunk* sfxshoot = NULL;
	Mix_Chunk* sfxexplosion = NULL;
};