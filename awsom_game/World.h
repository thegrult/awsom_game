#pragma once
#include "Wrld.h"
#include "Protagonist.h"
#include "Background.h"
#include "Bandit.h"
#include <random>
#include "Vec2.h"
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

class World : public Wrld{
public:
	World( SDL_Renderer* gRenderer, const Uint8* kbd, const Uint32& mouseKeys, const Vei2& mousePos );
	~World();
	void Update( const float dt );
	void ProcessInput();
	void Render() const;

	void AdaptCam( Window w ) { cam.AdaptToWnd( w ); }
public:
	void PlaySnd( Sounds s ) override;
	void SpawnBullet( Projectile* p ) override;

	const std::vector<Projectile*>* GetProjConst() const override;
	const std::vector<Entity*>* GetEntitiesConst() const override;
	const Protagonist* GetProtagonistConst() const override;
	const std::pair<const Background*,const Background*> GetBackandForeGround() const override;

	const Uint8* GetKbd() const override;
	const Uint32& GetMouseKeys() const override;
	const Vei2& GetMousePos() const override;
private:
	//references to kbd, mouse and its pos
	const Uint8* kbd;
	const Uint32& mouseKeys;
	const Vei2& mousePos;

	const int nEnemies = 10;
	//entities
	Protagonist* elia = nullptr;

	std::vector<Entity*> enemies;

	std::vector<Projectile*> projectiles;

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

	//rnd int generator
	mutable std::mt19937 rng{ std::random_device()() };
	mutable std::uniform_real_distribution<float> xDist = std::uniform_real_distribution<float>( 0.0f, float( LEVEL_WIDTH ) );
	mutable std::uniform_real_distribution<float> yDist = std::uniform_real_distribution<float>( 0.0f, float( LEVEL_HEIGHT ) );
};