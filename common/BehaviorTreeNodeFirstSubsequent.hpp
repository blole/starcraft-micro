#pragma once
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>

namespace BehaviorTree
{
	template <class T>
	class BehaviorTreeNodeFirstSubsequent : public BehaviorTreeNode
	{
	private:
		bool first;
		
	public:
		BehaviorTreeNodeFirstSubsequent()
		{
			first = true;
		}


	public: //sealed
		void init(void* agent) sealed {}

		BEHAVIOR_STATUS execute(void* agent) sealed
		{
			T* t = static_cast<T*>(agent);
			BEHAVIOR_STATUS status;

			if (first)
			{
				first = false;
				status = firstExecute(t);
			}
			else
				status = subsequentExecute(t);

			if (status != BT_RUNNING)
				first = true;

			return status;
		}

	protected:
		virtual BEHAVIOR_STATUS firstExecute(T* agent) = 0;
		virtual BEHAVIOR_STATUS subsequentExecute(T* agent) = 0;
	};
}
