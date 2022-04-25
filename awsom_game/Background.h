#pragma once
#include "Surface.h"
#include "Rect.h"

class Background {
public:
	Background( Surface tiles, int width, int height, Vei2 readPos );

	void Draw();
private:
	int tilew;
	int tileh;
	Vei2 readPos;
	Surface tiles;
};