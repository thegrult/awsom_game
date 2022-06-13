#pragma once
#include "Rect.h"
#include "Surface.h"
#include "Utilities.h"
#include "Action.h"
#include "Wrld.h"
#include <SDL_mouse.h>

struct McMode {
	enum class Modes {
		ranged,
		melee
	};
	Modes mode = Modes::ranged;
	void SetMode( Modes newMode ) { mode = newMode; }
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
	enum {
		changeMode
	};
private:
	Surface sprite;
	bool shown = false;

	RectI box;

	Action action;

	McMode& mode;
};