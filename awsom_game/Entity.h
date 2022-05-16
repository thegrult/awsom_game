#pragma once
#include "Avatar.h"
#include "Utilities.h"

class Entity {
public:
	//hitbox is considered in frame space
	Entity( const Vec2& spawnPos, const Vei2& readPos, int spritewidth, int spriteheight, int framecount, int animcount, Surface* sprite, SDL_Renderer* renderer, RectI hitBox );

	void Draw();

	void Update( const float dt );

	void SetVel( const Vec2& vel );
	void SetAnim( int animIndex );

	void ClampToRect( RectF rect );
	void CollideRect( RectF rect );
	void ApplyDamage( int dmg );
	void ApplyInvincibility( float dur );

	RectF GetHitBox() const;
	int GetAtk() const { return atk; }
	bool IsDead() const { return state.isDead(); }

private:
	void SetPos( const Vec2& nPos );

	void DisplaceBy( const Vec2 delta ) {	pos += delta;	}
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
			Damaged,
			Dead,
			Total
		};
	private:
		states state = states::Normal;
		float stateTime;
		float effectDur = 0.5f;
	public:
		void Damage( float dur) {
			state = states::Damaged;
			effectDur = dur;
		}
		bool IsDamaged() {
			return state == states::Damaged;
		}
		void ApplyInvincibility( float dur ) {
			state = states::Invincible;
			effectDur = dur;
		}
		bool IsInvincible() const{
			return state == states::Invincible;
		}
		void Update( float dt ) {
			if (state != states::Normal && state != states::Dead) {
				stateTime += dt;
				if (stateTime >= effectDur) {
					stateTime = 0.0f;
					effectDur = 0.0f;
					state = states::Normal;
				}
			}
		}
		void Dead() {
			state = states::Dead;
		}
		bool isDead()const {
			return state == states::Dead;
		}
	};

	State state;

	RectI hitBox; //is integer because it is in sprite space
	Vec2 pos;
	Vec2 velocity = { 0,0 };
	//last frame time for rect collision
	float deltat = 0.0f;

	int hp = 10;
	int atk = 2;
};