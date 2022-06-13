#pragma once
#include <vector>
#include "Vec2.h"

class Wrld {
public:
	enum Sounds {
		music,
		sfxshoot,
		sfxexplosion
	};
public:
	virtual void PlaySnd( Sounds s ) = 0;
	virtual void SpawnBullet( class Projectile* p ) = 0;

	virtual const std::vector<class Projectile*>* GetProjConst() const = 0;
	virtual const std::vector<class Entity*>* GetEntitiesConst() const = 0;
	virtual const class Protagonist* GetProtagonistConst() const = 0;

	virtual const std::pair<const class Background*,const class Background*> GetBackandForeGround() const = 0;

	virtual const uint8_t* GetKbd() const = 0;
	virtual const uint32_t& GetMouseKeys() const = 0;
	virtual const Vec2& GetMousePos() const = 0;
};