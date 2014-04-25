#ifndef GOTOPOINT_H_
#define GOTOPOINT_H_
#include "BehaviorTree.h"
#include "point.h"

using namespace BehaviorTree;
namespace SL
{
	namespace Behaviors
	{
		/// Simple behavior of continuously going straight
		class GotoPoint:public BehaviorTree::BehaviorTreeNode
		{
		public:
			BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
			void init(void* agent);
			GotoPoint::GotoPoint(point p, int speed);
		private:
			point target;
			int speed;
		};
	}

}
#endif