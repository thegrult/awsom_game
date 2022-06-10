#pragma once
#include "Rect.h"
#include "Surface.h"

class Inventory {
public:
	Inventory( SDL_Renderer* renderer );
	void ToggleShown() { shown = !shown; }
	void Draw( RectI screen );
private:
	Surface sprite;
	bool shown = false;
};