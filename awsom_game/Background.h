#pragma once
#include "Surface.h"
#include "Rect.h"
#include "Camera.h"
#include <string>
#include <fstream>
#include "bgstringconverter.h"

class Background {
public:
	Background( Surface* tiles, int tilewidth, int tileheight, Vei2 readPos, Vei2 drawStartPos, int gridw, int gridh, std::ifstream& map );

	void Draw( const Camera& cam ) const;

	bool IsColliding( RectI hitBox ) const;

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