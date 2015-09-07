#include "common/dll.hpp"
#include "common/main.hpp"
#include "common/generals/allunitssinglesquad.hpp"
#include "common/squads/playing.hpp"
#include "common/selecters/ucb.hpp"
#include "common/stateevaluaters/simpleheuristics.hpp"
#include "common/actionlisters/branchonunit.hpp"
#include "common/players/mcts.hpp"
#include "common/backpropagaters/uct.hpp"
#include "common/terminalcheckers/framelimited.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;
	using namespace Bot::Search;

	typedef Players::MCTS<
		Nodes::MCTS,
		ActionListers::BranchOnUnit,
		Selecters::UCB,
		StateEvaluaters::Heuristics::SqrtHp_Dps,
		Backpropagaters::UCT,
		TerminalCheckers::FrameLimited<100>
	> PlayerType;
	typedef Squads::Playing<PlayerType> SquadType;
	typedef Generals::AllUnitsSingleSquad<SquadType> GeneralType;

	return new Main<GeneralType>;
}
