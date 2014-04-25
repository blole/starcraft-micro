#ifndef WAITFORNEXTWAVE_H_
#define WAITFORNEXTWAVE_H_
#include "BehaviorTree.h"

using namespace BehaviorTree;
namespace SL
{
	namespace Behaviors
	{
	/// Destroy all projectiles within a 200 unit radius of the agent
	class WaitForNextWave:public BehaviorTree::BehaviorTreeNode
	{
	public:
		BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
		void init(void* agent);
		WaitForNextWave::WaitForNextWave();
		const BehaviorTree::BehaviorTreeList* getChildren() const
		{
			return NULL;
		}
	};
	}

}
#endif