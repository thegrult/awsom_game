#pragma once
#include <vector>
#include <algorithm>
#include "Utilities.h"

class Action {
public:
	//cooldown starts counting as soon as the action starts
	bool Do( int action, float actDur, float coolDown = 0.0f );
	bool IsDoing( int action );
	void Update( float dt );
private:
	std::vector<std::pair<int, float>> active;
	std::vector<std::pair<int, float>> cooldowns;
};