#pragma once
#include "Rect.h"

class Camera {
public:
	Camera( const Vei2& startPos, const RectI& size, const RectI& limitRect );
	void CenterOnPoint( const Vei2& center );
	Vei2 GetPos() const { return focus.TopLeft(); }
private:
	RectI limitRect;
	RectI focus;
};