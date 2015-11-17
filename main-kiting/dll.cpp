#include "common/main.hpp"
#include "main-kiting/generalkiting.hpp"
#include "common/squads/playing.hpp"
#include "common/behaviors/attackclosest.hpp"
#include "main-kiting/flee.hpp"
#include "common/players/scripted.hpp"
#include "common/dll.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;

	using BehaviorTreeType =
		Behaviors::Sequence
		<
			Behaviors::Flee,
			Behaviors::AttackClosest
		>;

	using PlayerType  = Players::Scripted<BehaviorTreeType>;
	using SquadType   = Squads::Playing<PlayerType>;
	using GeneralType = Generals::Kiting<SquadType>;
	return new Main<GeneralType>;
}
