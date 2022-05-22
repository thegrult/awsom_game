#include "Action.h"

bool Action::Do( int act, float actDur, float cooldown )
{
	bool valid = true;
	for (auto a : cooldowns) {
		valid = valid && a.first != act;
	}
	if (valid) {
		active.push_back( { act, actDur } );
		cooldowns.push_back( { act, cooldown } );
	}
	return valid;
}

bool Action::IsDoing( int action )
{
	return std::any_of( active.begin(), active.end(), [&action]( std::pair<int, float> p )
		{
			return p.first == action;
		}
	);
}

void Action::Update( float dt )
{
	for (int i = 0; i < cooldowns.size(); i++) {
		cooldowns[i].second -= dt;
		if (cooldowns[i].second <= 0.0f) {
			cooldowns.erase( cooldowns.begin() + i );
		}
	}
	for (int i = 0; i < active.size(); i++) {
		active[i].second -= dt;
		if (active[i].second <= 0.0f) {
			active.erase( active.begin() + i );
		}
	}
}