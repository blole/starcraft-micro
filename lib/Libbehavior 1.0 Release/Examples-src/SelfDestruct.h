#ifndef SELFDESTRUCT_H_
#define SELFDESTRUCT_H_
#include "BehaviorTree.h"

using namespace BehaviorTree;
namespace SL
{
	namespace Behaviors
	{
		/// Fire your weapon
		class SelfDestruct:public BehaviorTree::BehaviorTreeNode
		{
		public:
			BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
			void init(void* agent);
			SelfDestruct::SelfDestruct();
			const BehaviorTree::BehaviorTreeList* getChildren() const
			{
				return NULL;
			}
		};
	}

}
#endif