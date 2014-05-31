#pragma once
#include "common/squad.hpp"
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>
#include <list>

using namespace BehaviorTree;

class MCTSsquad : public Squad
{
public:
	static const int radius = 400;
	
	void onFrame();
};
