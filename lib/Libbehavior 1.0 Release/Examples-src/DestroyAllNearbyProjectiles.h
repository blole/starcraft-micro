#ifndef DESTROYPROJECTILES_H_
#define DESTROYPROJECTILES_H_
#include "BehaviorTree.h"

using namespace BehaviorTree;
namespace SL
{
	namespace Behaviors
	{
	/// Destroy all projectiles within a 200 unit radius of the agent
	class DestroyAllNearbyProjectiles:public BehaviorTree::BehaviorTreeNode
	{
	public:
		BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
		void init(void* agent);
		DestroyAllNearbyProjectiles::DestroyAllNearbyProjectiles();
		const BehaviorTree::BehaviorTreeList* getChildren() const
		{
			return NULL;
		}
	};
	}

}
#endif