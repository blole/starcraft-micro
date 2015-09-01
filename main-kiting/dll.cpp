#include "common/dll.hpp"
#include "common/main.hpp"
#include "main-kiting/generalkiting.hpp"
#include "common/reactivesquad.hpp"
#include "behaviors/attackclosest.hpp"
#include "main-kiting/flee.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;
	using namespace BehaviorTree;

	function<BehaviorTreeNode*()> unitBrain = []{
		return (new SequentialNode())
			->addChild(new Flee())
			->addChild(new Behaviors::AttackClosest());
	};

	shared_ptr<General> general = make_shared<GeneralKiting<ReactiveSquad>>(unitBrain);
	
	return new Main(general);
}
