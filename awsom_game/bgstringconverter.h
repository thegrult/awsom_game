#pragma once
#include "Rect.h"
#include <algorithm>

class BgStringConverter {
private:
	static constexpr int dim = 32;
public:
	static std::pair<RectI,RectI> Convert( const char c ) {
		int i = c - 'A';
		RectI tile = { Vei2( i * dim, 0 ), dim, dim };
		RectI obHBox = { 1, -1, 1, -1 };
		switch (i) {
		case 18:
			obHBox = RectI( 7, 25, 9, 17 );
			break;
		case 19:
			obHBox = RectI( 9, 24, 9, 17 );
			break;
		case 20:
			obHBox = RectI( 10, 21, 12, 17 );
			break;
		case 21:
			obHBox = RectI( 9, 16, 14, 17 );
			break;
		case 24:
			obHBox = RectI( 5, 27, 7, 23 );
			break;
		case 25:
			obHBox = RectI( 0, 32, 0, 32 );
			break;
		}

		return { tile, obHBox };
	}
};