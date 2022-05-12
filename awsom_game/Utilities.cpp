#include "Utilities.h"

bool util::PointVsRect( const Vec2& p, const RectF* r )
{
	return (p.x >= r->TopLeft().x && p.y >= r->TopLeft().y && p.x < r->TopLeft().x + r->GetDim().x && p.y < r->TopLeft().y + r->GetDim().y);
}

bool util::RectVsRect( const RectF* r1, const RectF* r2 )
{
	return (r1->TopLeft().x < r2->TopLeft().x + r2->GetDim().x && r1->TopLeft().x + r1->GetDim().x > r2->TopLeft().x && r1->TopLeft().y < r2->TopLeft().y + r2->GetDim().y && r1->TopLeft().y + r1->GetDim().y > r2->TopLeft().y);
}

bool util::RayVsRect( const Vec2& ray_origin, const Vec2& ray_dir, const RectF* target, Vec2& contact_point, Vec2& contact_normal, float& t_hit_near )
{
	contact_normal = { 0,0 };
	contact_point = { 0,0 };

	// Cache division
	Vec2 invdir = { 1.0f / ray_dir.x, 1.0f / ray_dir.y };

	// Calculate intersections with rectangle bounding axes
	Vec2 t_near;
	t_near.x = (target->TopLeft().x - ray_origin.x) * invdir.x;
	t_near.y = (target->TopLeft().y - ray_origin.y) * invdir.y;
	Vec2 t_far;
	t_far.x = (target->TopLeft().x + target->GetDim().x - ray_origin.x) * invdir.y;
	t_far.y = (target->TopLeft().y + target->GetDim().y - ray_origin.y) * invdir.y;

	if (std::isnan( t_far.y ) || std::isnan( t_far.x )) return false;
	if (std::isnan( t_near.y ) || std::isnan( t_near.x )) return false;

	// Sort distances
	if (t_near.x > t_far.x) std::swap( t_near.x, t_far.x );
	if (t_near.y > t_far.y) std::swap( t_near.y, t_far.y );

	// Early rejection		
	if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

	// Closest 'time' will be the first contact
	t_hit_near = std::max( t_near.x, t_near.y );

	// Furthest 'time' is contact on opposite side of target
	float t_hit_far = std::min( t_far.x, t_far.y );

	// Reject if ray direction is pointing away from object
	if (t_hit_far < 0)
		return false;

	// Contact point of collision from parametric line equation
	contact_point = ray_origin + ray_dir * t_hit_near;

	if (t_near.x > t_near.y)
		if (invdir.x < 0)
			contact_normal = { 1, 0 };
		else
			contact_normal = { -1, 0 };
	else if (t_near.x < t_near.y)
		if (invdir.y < 0)
			contact_normal = { 0, 1 };
		else
			contact_normal = { 0, -1 };

	// Note if t_near == t_far, collision is principly in a diagonal
	// so pointless to resolve. By returning a CN={0,0} even though its
	// considered a hit, the resolver wont change anything.
	return true;
}


bool util::DynamicRectVsRect( const RectF* r_dynamic, Vec2& vel, const float fTimeStep, const RectF& r_static,
	Vec2& contact_point, Vec2& contact_normal, float& contact_time )
{
	// Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
	if (vel.x == 0 && vel.y == 0)
		return false;

	// Expand target rectangle by source dimensions
	RectF expanded_target = RectF( r_static.TopLeft() - r_dynamic->GetDim() / 2, (r_static.GetDim() + r_dynamic->GetDim()/2).x, (r_static.GetDim() + r_dynamic->GetDim()/2).y );

	if (RayVsRect( r_dynamic->TopLeft() + r_dynamic->GetDim() / 2, vel * fTimeStep, &expanded_target, contact_point, contact_normal, contact_time ))
		return (contact_time >= 0.0f && contact_time < 1.0f);
	else
		return false;
}

bool util::ResolveDynamicRectVsRect( RectF* r_dynamic, Vec2& vel, const float fTimeStep, RectF* r_static )
{
	Vec2 contact_point, contact_normal;
	float contact_time = 0.0f;
	if (DynamicRectVsRect( r_dynamic, vel, fTimeStep, *r_static, contact_point, contact_normal, contact_time ))
	{
		vel.x += contact_normal.x * std::abs( vel.x ) * (1 - contact_time);
		vel.y += contact_normal.y * std::abs( vel.y ) * (1 - contact_time);
		return true;
	}

	return false;
}