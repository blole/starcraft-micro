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

	typedef Nodes::MCTS NT;

	shared_ptr<Searcher> searcher = make_shared<Searchers::MCTS<NT>>(
		make_shared<ActionListers::BranchOnUnit>(),
		make_shared<Selecters::UCB<NT>>(),
		make_shared<Heuristics::SqrtHp_Dps>(),
		make_shared<Backpropagaters::UCT<NT>>(),
		make_shared<TerminalCheckers::FrameLimited>(100));
	
	typedef GeneralAllUnitsSingleSquad<SearchingSquad> GeneralType;
	GeneralType general(searcher);
	return new Main<GeneralType>(general);
}
