#ifndef COOLDOWN_H_
#define COOLDOWN_H_
#include "BehaviorTree.h"

using namespace BehaviorTree;
namespace SL
{
	namespace Behaviors
	{
	/// Do nothing for the specified amount of time, then return success
	class Cooldown:public BehaviorTree::BehaviorTreeNode
	{
	public:
		int limit, current;
		BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
		void init(void* agent);
		/// Standard constructor
		/** \param timeout in milliseconds
		*/
		Cooldown::Cooldown(int timeout);
	};
	}

}
#endif