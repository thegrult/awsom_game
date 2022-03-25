#include "Entity.h"

Entity::Entity( const Vec2& spawnPos, const Vei2& readPos, int width, int height, int framecount, int animcount, Surface& sprite, SDL_Renderer* renderer, RectI hitBox )
	:
	avatar( readPos, width, height, framecount, animcount, sprite, holdTime, renderer),
	pos(spawnPos),
	hitBox(hitBox)
{
}

void Entity::Update( const float dt )
{
	if (velocity.LenSq() != 0.0f) {
		avatar.Update( dt );

		pos += velocity * dt;
	}
	state.Update( dt );
}

void Entity::SetDirection( const Vec2& dir )
{
	velocity = dir.GetNormalized() * speed;
}

Vec2 Entity::GetVel() const
{
	return velocity;
}

void Entity::Draw()
{
#ifdef DEBUG
	SDL_Rect HitBox = (SDL_Rect)GetHitBox();
	SDL_Renderer* renderer = avatar.GetRenderer();
	SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );
	SDL_RenderDrawRect( renderer, &HitBox );
#endif // DEBUG

	if (state.isDamaged()) {
		const int index = avatar.CurIndex();
		avatar.SetAnim( index + avatar.NAnim() );
		avatar.Draw( (Vei2)pos );
		avatar.SetAnim( index );
	}
	else {
		avatar.Draw( (Vei2)pos );
	}
}

void Entity::SetAvatarRenderer( SDL_Renderer* newRenderer )
{
	avatar.SetRenderer( newRenderer );
}

void Entity::ClampToRect( RectF rect )
{
	if (pos.x <= rect.left) {
		pos.x = rect.left + 0.1f;
	}
	else if (pos.x >= rect.right) {
		pos.x = rect.right - 0.1f;
	}

	if (pos.y <= rect.top) {
		pos.y = rect.top;
	}
	else if (pos.y >= rect.bottom) {
		pos.y = rect.bottom - 0.1f;
	}
	else if (pos.y >= rect.bottom) {
		pos.y = rect.bottom;
	}
}

void Entity::ApplyDamage()
{
	state.Damage();
}

RectF Entity::GetHitBox() const
{
	return RectF(hitBox).GetDisplaced(pos);
}

void Entity::SetAnim( int animIndex )
{
	avatar.SetAnim( animIndex );
}
