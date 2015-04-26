#include <BWAPI.h>
#include "search/actions/effect.hpp"
#include "search/actions/attack.hpp"

using namespace Bot::Search;

void lol()
{
	//GameState* state = new GameState(nullptr, nullptr);
	auto a = new ClearAttackFrame<>(0);
	auto b = new ClearAttackFrame<5, ClearAttackFrame<>>(0);
	auto c = new ClearAttackFrame<5, ClearAttackFrame<3, ClearAttackFrame<>>>(0);
	//c->applyToAndQueueNextEffect(state);
	//auto b = new SingleUnitEffect<3, SingleUnitEffect<2>>(nullptr);
	//auto d = new SimpleAttack<
	// om en effekt är
	// KillAll<4>


	//auto b = new ClearAttackFrame(nullptr);
	//a->applyTo(nullptr);
}
