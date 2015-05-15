#pragma once
#include <BWAPI.h>
#include <list>
#include "common/squad.hpp"
#include "common/reactivesquad.hpp"
#include <lib/libbehavior/BehaviorTree.h>

namespace Bot
{
	class General
	{
	protected:
		std::function<Squad*()> newSquad;
	public:
		std::list<Squad*> squads;
	
	
	public:
		General(std::function<Squad*()>& newSquad)
			: newSquad(newSquad)
		{}
		General(std::function<BehaviorTree::BehaviorTreeNode*()> unitBrain)
			: newSquad([=]{ return new ReactiveSquad(unitBrain); })
		{}
	
	
	public:
		virtual void onStart() {}
		virtual void onFrame() = 0;
	};
}
