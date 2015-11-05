#include "common/main.hpp"
#include "common/generals/allunitssinglesquad.hpp"
#include "common/players/scripted.hpp"
#include "common/squads/playing.hpp"
#include "common/dll.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;

	typedef Players::Scripted<Behaviors::Always> PlayerType;
	typedef Squads::Playing<PlayerType> SquadType;
	typedef Generals::AllUnitsSingleSquad<SquadType> GeneralType;

	return new Main<GeneralType>(GeneralType(SquadType(PlayerType(Behaviors::Always(Behaviors::success)))));
}