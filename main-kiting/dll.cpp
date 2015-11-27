#include "common/main.hpp"
#include "main-kiting/generalkiting.hpp"
#include "common/squads/playing.hpp"
#include "common/behaviors/attackclosest.hpp"
#include "main-kiting/flee.hpp"
#include "common/players/scripted.hpp"
#include "common/dll.hpp"

struct : DllInitializer
{
	virtual BWAPI::AIModule* newAIModule() override
	{
		using namespace Bot;

		using BehaviorTreeType =
			Behaviors::Sequence
			<
			Behaviors::Flee,
			Behaviors::AttackClosest
			>;

		using PlayerType = Players::Scripted<BehaviorTreeType>;
		using SquadType = Squads::Playing<PlayerType>;
		using GeneralType = Generals::Kiting<SquadType>;
		return new Main<GeneralType>;
	}
} initializer;

DllInitializer& DllInitializer::instance = initializer;
