#pragma once
#include "Rect.h"
#include <algorithm>
#include <cmath>

#ifndef UTILITIES_H
#define UTILITIES_H

namespace util {
	bool PointVsRect( const Vec2& p, const RectF* r );

	bool RectVsRect( const RectF* r1, const RectF* r2 );

	bool RayVsRect( const Vec2& ray_origin, const Vec2& ray_dir, const RectF* target, Vec2& contact_point, Vec2& contact_normal, float& t_hit_near );

	bool DynamicRectVsRect( const RectF* r_dynamic, Vec2& vel, const float fTimeStep, const RectF& r_static,
		Vec2& contact_point, Vec2& contact_normal, float& contact_time );

	bool ResolveDynamicRectVsRect( RectF* r_dynamic, Vec2& vel, const float fTimeStep, RectF* r_static );

	void DumbWay( RectF* r_dynamic, Vec2& vel, RectF* r_static );

	template <class Container, class Pred>
	void remove_erase_if( Container& c, Pred p )
	{
		const auto nEnd = std::remove_if( c.begin(), c.end(), p );
		c.erase( nEnd, c.end() );
	}
}

#endif