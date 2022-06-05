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
	util::remove_erase_if( cooldowns, [dt]( auto& c ) {
		c.second -= dt;
		if (c.second <= 0.0f) return true;
		else return false;
		} );
	util::remove_erase_if( active, [dt]( auto& c ) {
		c.second -= dt;
		if (c.second <= 0.0f) return true;
		else return false;
		} );
}