#pragma once
#include "Surface.h"
#include "Rect.h"
#include <string>
#include "bgstringconverter.h"

class Background {
public:
	Background( Surface* tiles, int tilewidth, int tileheight, Vei2 readPos, Vei2 drawStartPos, int gridw, int gridh, std::string map );

	void Draw();

	bool IsColliding( RectI hitBox );

	const std::vector<RectI>& GetObstacles() const {
		return obstacles;
	}
private:
	const int tilew;
	const int tileh;
	int gridw;
	int gridh;
	Vei2 drawStartPos;
	Surface* sprite;
	std::vector<RectI> tiles;
	std::vector<RectI> obstacles;
};