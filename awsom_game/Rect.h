#pragma once
#include "SDL.h"

#include "Vec2.h"

template<typename T>
class Rect
{
public:
	template <typename N>
	operator Rect<N>() const {
		return Rect<N>( (N)left, (N)right, (N)top, (N)bottom );
	}
	Rect( T left_in, T right_in, T top_in, T bottom_in )
		:
		left( left_in ),
		right( right_in ),
		top( top_in ),
		bottom( bottom_in )
	{
	}
	Rect( const _Vec2<T>& topLeft, const _Vec2<T>& bottomRight )
		:
		Rect( topLeft.x, bottomRight.x, topLeft.y, bottomRight.y )
	{
	}
	Rect( const _Vec2<T>& topLeft, T width, T height )
		:
		Rect( topLeft, topLeft + _Vec2<T>( width, height ) )
	{
	}
	Rect( const SDL_Rect& rect )
		:
		Rect( {rect.x, rect.y}, rect.w, rect.h )
	{}
	Rect() = default;

	bool IsOverlappingWith( const Rect& other ) const
	{
		return right > other.left && left < other.right
			  && bottom > other.top && top < other.bottom;
	}

	bool IsContainedBy( const Rect& other ) const
	{
		return left >= other.left && right <= other.right &&
				top >= other.top && bottom <= other.bottom;
	}

	bool Contains( const _Vec2<T> point ) const {
		return point.x >= left && point.x <= right &&
			point.y >= top && point.y <= bottom;
	}

	static Rect<T> FromCenter( const _Vec2<T>& center, T halfWidth, T halfHeight )
	{
		  const _Vec2<T> half( halfWidth, halfHeight );
		  return Rect<T>( center - half, center + half );
	}

	Rect<T> GetExpanded( T offset ) const
	{
		  return Rect<T>( left - offset, right + offset, top - offset, bottom + offset );
	}

	Rect<T> GetDisplaced(_Vec2<T> d) const
	{
		return Rect<T>( left + d.x, right + d.x, top + d.y, bottom + d.y );
	}

	_Vec2<T> GetCenter() const
	{
		  return _Vec2<T>( (left + right) / (T)2, (top + bottom) / (T)2 );
	}

	T GetHalfWidth() const
	{
		return (right - left) / 2;
	}

	T GetHalfHeight() const
	{
		return (bottom - top) / 2;
	}

	_Vec2<T> GetDim() const
	{
		return { right - left, bottom - top };
	}

	_Vec2<T> TopLeft() const
	{
		return { left, top };
	}

	bool IsDegenerate() const
	{
		return left > right || top > bottom;
	}

	Rect<T> GetSquare() const
	{
		const auto shortside = GetHalfWidth() > GetHalfHeight() ? GetHalfHeight() : GetHalfWidth();
		return FromCenter( GetCenter(), shortside, shortside );
	}

	explicit operator SDL_Rect() const{
		return SDL_Rect{ int( left ), int( top ), int( right - left ), int( bottom - top ) };
	}
public:
	T left;
	T right;
	T top;
	T bottom;
};

using RectF = Rect<float>;
using RectI = Rect<int>;