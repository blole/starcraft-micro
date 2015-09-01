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

	typedef UCT::NodeUCT NT;

	function<Squad*()> newSquad = []{
		static shared_ptr<Searcher> searcher = make_shared<SearcherMCTS<NT>>(
			make_shared<ActionListers::BranchOnUnit>(),
			make_shared<Selecters::UCB<NT>>(),
			make_shared<Heuristics::SqrtHp_Dps>(),
			make_shared<Backpropagaters::UCT<NT>>(),
			make_shared<TerminalCheckers::FrameLimited>(100));
		return new SearchingSquad(searcher);
	};
	
	shared_ptr<GeneralAllUnitsSingleSquad> general = make_shared<GeneralAllUnitsSingleSquad>(newSquad);

	return new Main(general);
}
