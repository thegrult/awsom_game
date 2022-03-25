#pragma once
#include "Avatar.h"

class Entity {
public:
	//hitbox is considered in frame space
	Entity( const Vec2& spawnPos, const Vei2& readPos, int spritewidth, int spriteheight, int framecount, int animcount, Surface& sprite, SDL_Renderer* renderer, RectI hitBox );

	void Draw();

	void Update( const float dt );
	void SetDirection( const Vec2& dir );
	Vec2 GetVel() const;
	RectF GetHitBox() const;
	void SetAnim( int animIndex );

	//I don't think this is used anymore, could maybe remove it as it is no longer needed due to better handling of the media loading/unloading
	void SetAvatarRenderer( SDL_Renderer* newRenderer );

	void ClampToRect( RectF rect );
	void ApplyDamage();
private:
	//graphic things
	const float holdTime = 0.1f;
	Avatar avatar;

	//state machine for invicibility and damage effect
	class State {
	public:
		enum class states {
			Normal,
			Damaged,
			Died,
			Total
		};
	public:
		states state = states::Normal;
		float stateTime;
		float invDur = 0.5f;
		void Damage() {
			state = states::Damaged;
		}
		bool isDamaged() const{
			return state == states::Damaged;
		}
		void Update( float dt ) {
			if (state == states::Damaged) {
				stateTime += dt;
				if (stateTime >= invDur) {
					stateTime = 0.0f;
					state = states::Normal;
				}
			}
		}
		void Dead() {
			state = states::Died;
		}
		bool isDead()const {
			return state == states::Died;
		}
	};

	State state;

	RectI hitBox; //is integer because it is in sprite space
	Vec2 pos;
	Vec2 velocity = { 0,0 };

	int hp = 10;
	int atk = 2;
	float speed = 50.0f;
};