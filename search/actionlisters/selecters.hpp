#pragma once
#include "search/actionlisters/actionlister.hpp"

namespace Bot { namespace Search { 	namespace Selecters
{
	template <class NT>
	struct UCB : public Selecter<NT>
	{
		virtual EffectNodePair<NT>& select(NT& node, const GameState& state) const override
		{
			if (!node.fullyExpanded)
			{
				//try to find unexpanded child
				for (unsigned int i = 0; i<node.children.size(); i++)
				{
					EffectNodePair<NT>& next = node.children[i];
					if (next.node == nullptr)
					{
						next.node = new NT(node);
						return next;
					}
				}

				node.fullyExpanded = true;
			}

			EffectNodePair<NT>* best = &node.children[0];
			double bestUCB = getUCB(best->node);

			for (unsigned int i = 1; i<node.children.size(); i++)
			{
				EffectNodePair<NT>& next = node.children[i];
				double ucb = getUCB(next.node);
				if (bestUCB < ucb)
				{
					bestUCB = ucb;
					best = &next;
				}
			}

			return *best;
		}

	private:
		static double getUCB(NT* node)
		{
			return node->totalReward / node->visits + std::sqrt(std::log((double)node->parent->visits) / node->visits);
		}
	};
}}}
