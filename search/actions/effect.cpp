#include "search/actions/effect.hpp"
#include <BWAPI.h>

using namespace Bot::Search;

void lol()
{
	GameState* state = new GameState(nullptr, nullptr);
	auto a = new ClearAttackFrame<>(0);
	auto b = new ClearAttackFrame<5, ClearAttackFrame<>>(0);
	auto c = new ClearAttackFrame<5, ClearAttackFrame<3, ClearAttackFrame<>>>(0);
	c->applyToAndQueueNextEffect(state);
	//auto b = new SingleUnitEffect<3, SingleUnitEffect<2>>(nullptr);

	// om en effekt är
	// KillAll<4>


	//auto b = new ClearAttackFrame(nullptr);
	//a->applyTo(nullptr);
}
