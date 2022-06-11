#pragma once

class Wrld {
public:
	enum Sounds {
		music,
		sfxshoot,
		sfxexplosion
	};
public:
	virtual void PlaySnd( Sounds s ) = 0;
	virtual void SpawnBullet( class Projectile& p ) = 0;
};