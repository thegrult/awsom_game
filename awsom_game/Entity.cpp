#include "Entity.h"

Entity::Entity( const Vec2& spawnPos, const Vei2& readPos, int width, int height, int framecount, int animcount, Surface* sprite, SDL_Renderer* renderer, RectI hitBox )
	:
	avatar( readPos, width, height, framecount, animcount, sprite, holdTime, renderer),
	pos(spawnPos),
	hitBox(hitBox),
	drawingBox( { 0,0 }, width, height )
{}

void Entity::Update( const float dt )
{
	state.Update( dt );

	if (IsAlive()) {
		if( velocity != Vec2( 0.0f, 0.0f )) {
			avatar.Update( dt );
			pos += velocity * dt;
		}
	}
}

void Entity::SetVel( const Vec2& vel )
{
	velocity = vel;
}

void Entity::Draw( const Camera& cam )
{
	if ( drawingBox.GetDisplaced( (Vei2)pos ).IsOverlappingWith( cam.GetFocus() ) ) {
		if (state.Is( State::Dead )) {}
		else if (state.Is( State::Damaged )) {
			const int index = avatar.CurIndex();
			avatar.SetAnim( index + avatar.NAnim() );
			avatar.Draw( (Vei2)pos - cam.GetPos() );
			avatar.SetAnim( index );
		}
		else if (state.Is( State::Dying )) {
			Uint32 alpha = 0xff;
			alpha *= Uint32( state.StateTimeLeft() * 1000 );
			alpha /= Uint32( deathAnimTime * 1000 );
			avatar.DrawBlend( (Vei2)pos - cam.GetPos(), Uint8( alpha ) );
		}
		else {
			avatar.Draw( (Vei2)pos - cam.GetPos() );
		}

#ifdef _DEBUG
		const auto j = GetHitBox().GetDisplaced( (Vec2)-cam.GetPos() );
		SDL_Rect HitBox = { int( j.TopLeft().x ), (int)j.TopLeft().y, (int)j.GetDim().x, (int)j.GetDim().y };
		SDL_Renderer* renderer = avatar.GetRenderer();
		SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );
		SDL_RenderDrawRect( renderer, &HitBox );
#endif // DEBUG
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

void Entity::ApplyDamage( int dmg )
{
	if (!state.Is( State::Invincible ) && !state.Is( State::Damaged ) && IsAlive()) {
		hp -= dmg;
		if( dmg > 0 )
		state.ChangeState( State::Damaged, 0.5f );

		if (hp <= 0) {
			state.ChangeState( State::Dying, deathAnimTime );
			atk = 0;
		}
	}
}

void Entity::ApplyInvincibility( float dur )
{
	state.ChangeState( State::Invincible, dur );
}

RectF Entity::GetHitBox() const
{
	return RectF(hitBox).GetDisplaced(pos);
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
