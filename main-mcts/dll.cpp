#include "common/dll.hpp"
#include "common/main.hpp"
#include "common/generalallunitssinglesquad.hpp"
#include "search/searchingsquad.hpp"
#include "search/selecters/ucb.hpp"
#include "search/stateevaluaters/simpleheuristics.hpp"
#include "search/actionlisters/branchonunit.hpp"
#include "search/searchers/searchermcts.hpp"
#include "search/backpropagaters/uct.hpp"
#include "search/terminalcheckers/framelimited.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;
	using namespace Bot::Search;

	typedef Searchers::MCTS<
		Nodes::MCTS,
		ActionListers::BranchOnUnit,
		Selecters::UCB,
		Heuristics::SqrtHp_Dps,
		Backpropagaters::UCT,
		TerminalCheckers::FrameLimited<100>
	> SearcherType;
	typedef SearchingSquad<SearcherType> SquadType;
	typedef GeneralAllUnitsSingleSquad<SquadType> GeneralType;

	return new Main<GeneralType>;
}
