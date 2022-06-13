#pragma once
#include "Avatar.h"
#include "Background.h"
#include "Camera.h"
#include "Entity.h"
#include "Wrld.h"

class Projectile {
public:
	//spawnpos: the position at which the projectile is born; readpos: the sprite location on spritesheet; spritewidth and height: the dimensions of the sprite; 
	//framecount: the number of frames the animation has; sprite: the source sprite; Renderer: needed for animation;
	//hitBox: the actual size of the projectile (not the sprite, the actual thing); range: the max distance the projectile can travel before exploding, animation is based on this.
	//velocity: movement speed and direction per second
	Projectile( const Vec2& spawnPos, const Vei2& readPos, int spritewidth, int spriteheight, int normalFramesCount, int explosionFramesCount, float holdTime, Surface* sprite,
				SDL_Renderer* renderer, const RectI& hitBox, float range, const Vec2& velocity, int dmg, bool isFriend );

	void HandleInput( Wrld* wrld );
	void Draw( const Camera& camPos ) const;

	void Update( float dt );
	RectF GetHitBox() const;

	int GetDmg() const { return dmg; }

	//activates exploding sequence (damage to other entities is applied at the start of the explosion)
	void Hits();

	bool operator==( const Projectile& rhs ) const;

	bool IsExploding() const {
		return isExploding;
	}
	bool ToBeRemoved()const;

	bool IsFriend() const { return isFriend; }
private:
	bool isFriend = true;
	int dmg;
	bool isExploding = false;
	bool toRemove = false;
	float expldur;
	float range;
	float expltimer = 0.0f;
	RectI hitBox; //is integer because it's calculated in sprite space, remember to convert!
	Vec2 startPos;
	Vec2 pos;
	Vec2 vel;
	Animation animation;
	Animation explanim;
};