#include "Entity.h"

Entity::Entity( const Vec2& spawnPos, const Vei2& readPos, int width, int height, int framecount, int animcount, Surface* sprite, RectI hitBox )
	:
	avatar( readPos, width, height, framecount, animcount, sprite, holdTime ),
	pos(spawnPos),
	hitBox(hitBox),
	drawingBox( { 0,0 }, width, height )
{}

void Entity::Update( const float dt )
{
	state.Update( dt );

	if (IsAlive()) {
		if (velocity != Vec2( 0.0f, 0.0f )) {
			avatar.Update( dt );
			pos += velocity * dt;
		}
	}
}

void Entity::SetVel( const Vec2& vel )
{
	velocity = vel;
}

Drawable Entity::GetDrawable( const Camera& cam ) const
{
	if (drawingBox.GetDisplaced( (Vei2)pos ).IsOverlappingWith( cam.GetFocus() )) {
		if (state.Is( State::Dead )) {
			return Drawable( nullptr, RectI(), RectI(), 0 );
		}
		else if (state.Is( State::Damaged )) {
			return avatar.GetDrawable( (Vei2)pos - cam.GetPos(), 0xff0000ff );
		}
		else if (state.Is( State::Dying )) {
			Uint32 alpha = 0xff;
			alpha *= Uint32( state.StateTimeLeft() * 1000 );
			alpha /= Uint32( deathAnimTime * 1000 );
			return avatar.GetDrawable( (Vei2)pos - cam.GetPos(), 0xffffff00 | Uint8( alpha ) );
		}
		else {
			return avatar.GetDrawable( (Vei2)pos - cam.GetPos() );
		}
	}
	else
	{ 
		return Drawable( nullptr, RectI(), RectI(), 0 );
	}
}

void Entity::ClampToRect( RectF rect )
{
	RectF hbx = GetHitBox();
	if (hbx.left <= rect.left) {
		pos.x += rect.left - hbx.left + 0.1f;
	}
	else if (hbx.right >= rect.right) {
		pos.x += rect.right - hbx.right - 0.1f;
	}

	if (hbx.top <= rect.top) {
		pos.y += rect.top - hbx.top + 0.1f;
	}
	else if (hbx.bottom >= rect.bottom) {
		pos.y += rect.bottom - hbx.bottom - 0.1f;
	}
}

void Entity::CollideRect( RectF rect )
{
	auto hbx = GetHitBox();

	util::DumbWay( &hbx, velocity, &rect );

	SetPos( hbx.TopLeft() );
}

void Entity::ApplyDamage( float dmg )
{
	if (!state.Is( State::Invincible ) && !state.Is( State::Damaged ) && IsAlive()) {
		hp -= dmg;
		if( dmg > 0 )
		state.ChangeState( State::Damaged, 0.1f );

		if (hp <= 0) {
			state.ChangeState( State::Dying, deathAnimTime );
			atk = 0;
		}
	}
}

void Entity::ApplyInvincibility( float dur )
{
	if( IsAlive() )
	state.ChangeState( State::Invincible, dur );
}

RectF Entity::GetHitBox() const
{
	return RectF(hitBox).GetDisplaced(pos);
}

Vec2 Entity::GetPos() const
{
	return GetHitBox().GetCenter();
}

void Entity::SetPos( const Vec2& nPos )
{
	pos = nPos - Vec2(hitBox.TopLeft());
}

void Entity::SetAnim( int animIndex )
{
	avatar.SetAnim( animIndex );
}

void Entity::State::ChangeState( int newState, float stateDur )
{
	state = newState;
	stateTime = stateDur;
}

bool Entity::State::Is( int isState ) const
{
	return state == isState;
}

void Entity::State::Update( float dt )
{
	if ( state != states::Normal && state != states::Dead ) {
		stateTime -= dt;
		if (state != states::Dying) {
			if (stateTime <= 0.0f) {
				stateTime = 0.0f;
				state = states::Normal;
			}
		}
		else if (stateTime <= 0.0f) {
				state = states::Dead;
			}
	}
}

float Entity::State::StateTimeLeft() const
{
	return stateTime;
}
