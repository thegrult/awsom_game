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
	bool IsDead() const { return state.Is( State::Dead ); }
	bool IsAlive() const { return !IsDead() && !state.Is( State::Dying ); }
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
		enum states {
			Normal,
			Invincible,
			Damaged,
			Dying,
			Dead,
			Total
		};
	private:
		int state = states::Normal;
		float stateTime = 0.5f;
	public:
		void ChangeState( int newState, float stateDur );
		bool Is( int isState ) const;
		void Update( float dt );
		float StateTimeLeft() const;
	};

	State state;

	RectI hitBox; //is integer because it is in sprite space
	Vec2 pos;
	Vec2 velocity = { 0,0 };

	static constexpr float fadeOutTime = 1.5f;

	int hp = 10;
	int atk = 2;
};