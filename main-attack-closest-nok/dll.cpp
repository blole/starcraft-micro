#include "common/dll.hpp"
#include "common/main.hpp"
#include "common/generalallunitssinglesquad.hpp"
#include "behaviors/attackclosestnok.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;
	using namespace BehaviorTree;

	function<BehaviorTreeNode*()> unitBrain = []{
		return (new SequentialNode())
			->addChild(new Behaviors::AttackClosestNOK());
	};
	shared_ptr<General> general = make_shared<GeneralAllUnitsSingleSquad>(unitBrain);

	return new Main(general);
}
