#include "common/dll.hpp"
#include "common/main.hpp"
#include "behaviors/attackclosest.hpp"
#include "generalkiting.hpp"
#include "flee.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;

	function<BehaviorTreeNode*()> unitBrain = []{
		return (new SequentialNode())
			->addChild(new Flee())
			->addChild(new Behaviors::AttackClosest());
	};
	shared_ptr<General> general = make_shared<GeneralAllUnitsSingleSquad>(unitBrain);
	
	return new Main(general);
}
