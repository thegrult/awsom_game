#pragma once
#include "Rect.h"
#include "Surface.h"
#include "Utilities.h"
#include "Action.h"
#include "Wrld.h"
#include <SDL_mouse.h>

struct McMode {
	enum Modes {
		ranged,
		melee
	};
	int mode = Modes::ranged;
	void SetMode( Modes newMode ) { mode = newMode; }
	void SetMode( int newMode ) { mode = newMode; }
	template <typename T>
	T DiscernMode( Modes m, T yes, T no ) const{
		return m == mode ? yes : no;
	}
};

class Inventory {
public:
	Inventory( SDL_Renderer* renderer, McMode& mode );
	void ToggleShown() { shown = !shown; }
	void Draw( RectI screen );

	void HandleInput( Wrld* wrld );
	void Update( const float dt ) { action.Update( dt ); }
private:
	void SetMode( McMode::Modes newMode );
	void SetMode( int newMode );
private:
	Surface sprite;
	bool shown = false;

	std::vector<std::pair<int, std::pair<RectI, Surface*>>> imgs;

	RectI box;

	Action action;

	McMode& mode;

	friend class Protagonist;
};