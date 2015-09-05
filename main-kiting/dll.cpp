#include "common/dll.hpp"
#include "common/main.hpp"
#include "main-kiting/generalkiting.hpp"
#include "common/reactivesquad.hpp"
#include "search/behaviors/attackclosest.hpp"
#include "main-kiting/flee.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;
	using namespace BehaviorTree;

	function<BehaviorTreeNode*()> unitBrain = []{
		return nullptr;
		//TODO: return (new SequentialNode())
		//	->addChild(new Flee())
		//	->addChild(new Behaviors::AttackClosest());
	};

	typedef GeneralKiting<ReactiveSquad> GeneralType;
	GeneralType general(unitBrain);
	return new Main<GeneralType>(general);
}
