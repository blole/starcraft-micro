#include "common/dll.hpp"
#include "common/main.hpp"
#include "common/selecters/ucb.hpp"
#include "common/stateevaluaters/simpleheuristics.hpp"
#include "common/actionlisters/branchonunit.hpp"
#include "common/players/mcts.hpp"
#include "common/backpropagaters/uct.hpp"
#include "common/terminalcheckers/framelimited.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;

	using PlayerType = Players::MCTS<
		Nodes::MCTS,
		ActionListers::BranchOnUnit,
		Selecters::UCB,
		StateEvaluaters::Heuristics::SqrtHp_Dps,
		Backpropagaters::UCT,
		TerminalCheckers::FrameLimited<100>
	>;

	return new PlayerMain<PlayerType>;
}
