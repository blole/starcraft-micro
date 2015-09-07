#include "common/dll.hpp"
#include "common/main.hpp"
#include "common/generals/allunitssinglesquad.hpp"
#include "search/searchingsquad.hpp"
#include "search/selecters/ucb.hpp"
#include "search/stateevaluaters/simpleheuristics.hpp"
#include "search/actionlisters/branchonunit.hpp"
#include "search/players/mcts.hpp"
#include "search/backpropagaters/uct.hpp"
#include "search/terminalcheckers/framelimited.hpp"

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
