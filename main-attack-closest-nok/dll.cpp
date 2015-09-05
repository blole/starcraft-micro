#include "common/dll.hpp"
#include "common/main.hpp"
#include "common/generalallunitssinglesquad.hpp"
//#include "search/behaviors/attackclosestnok.hpp"
#include "common/reactivesquad.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;
	using namespace BehaviorTree;

	function<BehaviorTreeNode*()> unitBrain = []{
		return nullptr; //TODO:return new Behaviors::AttackClosestNOK();
	};

	typedef GeneralAllUnitsSingleSquad<ReactiveSquad> GeneralType;
	GeneralType general(unitBrain);
	return new Main<GeneralType>(general);
}
