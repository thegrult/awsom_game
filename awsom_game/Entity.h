#pragma once
#include "Avatar.h"
#include "Camera.h"
#include "Wrld.h"
#include "Utilities.h"
#include "Vec2.h"

class Entity {
public:
	//hitbox is considered in frame space
	Entity( const Vec2& spawnPos, const Vei2& readPos, int spritewidth, int spriteheight, int framecount, int animcount, Surface* sprite, RectI hitBox );

	void Draw( const Camera& cam ) const;

	virtual void HandleInput( Wrld* wrld ) = 0;
	virtual void Update( const float dt );

	RectF GetHitBox() const;
	Vec2 GetPos() const;
	virtual float GetAtk() const { return atk; }
	bool IsDead() const { return state.Is( State::Dead ); }
	bool IsAlive() const { return !IsDead() && !state.Is( State::Dying ); }
protected:
	//does not perform normalization
	void SetVel( const Vec2& vel );
	void SetAnim( int animIndex );

	void ClampToRect( RectF rect );
	void CollideRect( RectF rect );
	void ApplyDamage( float dmg );
	void ApplyInvincibility( float dur );
private:
	void SetPos( const Vec2& nPos );

	void DisplaceBy( const Vec2 delta ) {	pos += delta;	}
protected:
	//graphic things
	static constexpr float holdTime = 0.1f;
	Avatar avatar;
	RectI drawingBox;

	static constexpr float deathAnimTime = 1.5f;
protected:
	//state machine controlling life and death - for actions we'll reuse protagonist::action in each child class
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

	float hp = 10.0f;
	float atk = 2.0f;
};