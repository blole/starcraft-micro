#include "behaviors/AttackClosest.hpp"
#include "common/GameUnit.hpp"
#include "common/PUnit.hpp"

using namespace BWAPI;

MoveSquadToPosition::MoveSquadToPosition()
{
}

void MoveSquadToPosition::init(void* agent)
{
}

BEHAVIOR_STATUS AttackClosest::execute(void* agent)
{
 return BT_RUNNING;
}
