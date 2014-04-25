#ifndef WAITFORSIGNAL_H_
#define WAITFORSIGNAL_H_
#include "BehaviorTree.h"

using namespace BehaviorTree;
namespace SL
{
	namespace Behaviors
	{
	/// Destroy all projectiles within a 200 unit radius of the agent
	class WaitForSignal:public BehaviorTree::BehaviorTreeNode
	{
	public:
		BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
		void init(void* agent);
		WaitForSignal::WaitForSignal(bool& signal);
		const BehaviorTree::BehaviorTreeList* getChildren() const
		{
			return NULL;
		}

	private:
		bool& signal;
	};
	
	}

}
#endif