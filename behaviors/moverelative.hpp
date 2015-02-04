#pragma once
#include "common/BehaviorTreeNodeFirstSubsequent.hpp"
#include "common/punit.hpp"
#include <BWAPI.h>

using namespace BehaviorTree;

class MoveRelative : public BehaviorTreeNodeFirstSubsequent<PUnit>
{
public:
	MoveRelative(int x, int y);

	BEHAVIOR_STATUS firstExecute(PUnit* unit);
	BEHAVIOR_STATUS subsequentExecute(PUnit* unit);

private:
	BWAPI::Position origin;
	BWAPI::Position offset;
};
