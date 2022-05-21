#include "Camera.h"

Camera::Camera( const Vei2& startPos, const RectI& size, const RectI& limitRect )
	:
	limitRect(limitRect),
	focus(size)
{}

void Camera::CenterOnPoint( const Vei2& center )
{
	focus = RectI::FromCenter( center, focus.GetHalfWidth(), focus.GetHalfHeight() );

	if (focus.left < limitRect.left)
		focus = focus.GetDisplaced( { limitRect.left - focus.left, 0 } );
	else if (focus.right > limitRect.right)
		focus = focus.GetDisplaced( { limitRect.right - focus.right, 0 } );

	if (focus.top < limitRect.top)
		focus = focus.GetDisplaced( { 0, limitRect.top - focus.top } );
	else if (focus.bottom > limitRect.bottom)
		focus = focus.GetDisplaced( { 0, limitRect.bottom - focus.bottom } );
}