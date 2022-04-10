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
	void ApplyDamage( int dmg );
	int GetAtk() const { return atk; }
	bool IsDead() const { return state.isDead(); }
private:
	//graphic things
	static constexpr float holdTime = 0.1f;
	Avatar avatar;

	//state machine for invicibility and damage effect
	class State {
	public:
		enum class states {
			Normal,
			Invincible,
			Died,
			Total
		};
	public:
		states state = states::Normal;
		float stateTime;
		float invDur = 0.5f;
		void Damage() {
			state = states::Invincible;
		}
		bool isInvincible() const{
			return state == states::Invincible;
		}
		void Update( float dt ) {
			if (state == states::Invincible) {
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