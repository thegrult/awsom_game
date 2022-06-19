#pragma once
#include "Background.h"
#include "Entity.h"
#include "Projectile.h"
#include "Action.h"
#include "Inventory.h"
#include "Wrld.h"

class Protagonist : public Entity{
public:
	Protagonist( Vec2 spawnPos, Surface* Sprite );

	void HandleInput( Wrld* wrld ) override;
	void Update( float dt );

	void Draw( const Camera& camPos );

	float GetAtk() const override;
public:
	enum action{
		setModeMelee,
		setModeRanged,
		walk,
		dash,
		shoot,
		inventorytoggled
	};
private:
	void Do( int a );
	void SetDirection( const Vec2& dir );

	void Dash();

	void Shoot( Wrld* wrld );

	void ChangeMode( McMode::Modes mode );
private:
	//used for giving direction to the bullets and dashing
	Vec2 dir = { 0.0f,1.0f };
private:
	Action action;
	//some stats for our protagonist

	float walkingSpeed = 50.0f;
	float rollSpeed = 200.0f;
private:
	Surface* sprite;
	Inventory inv;

	McMode mode;
private:
	static constexpr float coolDowns[] = { 0, 0, 0, 1.0f, 0.5f, 0.5f };
private:
	class Combo {
	public:
		Combo( const std::vector<std::pair<int, float>>& moves );
		Combo() = default;

		void AddMove( int move, float waitTime );
		void Update( float dt, Protagonist& mc );

		void Start();
	private:
		//first is protagonist::action to to, second is float time to wait until next action
		std::vector<std::pair<int, float>> moves;
		std::vector<std::pair<int, float>> queue;
		bool active = false;
	};

	Combo combo = Combo( { { action::setModeMelee, 0.01f }, { action::dash, 0.01f }, {action::setModeRanged, 0.0f}, {action::shoot, 0.01f } } );
private:
	//wrld pointer to allow smarter use of Do()
	Wrld* wrld;
};