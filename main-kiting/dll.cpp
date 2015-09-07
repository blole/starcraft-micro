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
	using namespace Bot::Search;

	Behaviors::Sequence behavior;
	behavior.addChild(make_unique<Behaviors::Flee>());
	behavior.addChild(make_unique<Behaviors::AttackClosest>());

	typedef Players::Scripted<Behaviors::Sequence> PlayerType;
	typedef Squads::Playing<PlayerType> SquadType;
	typedef Generals::Kiting<SquadType> GeneralType;
	return new Main<GeneralType>(GeneralType(SquadType(PlayerType(behavior))));
}
