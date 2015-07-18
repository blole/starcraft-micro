#pragma once
#include <algorithm>
#include "search/selecter.hpp"

namespace Bot { namespace Search { 	namespace Selecters
{
	template <class NT>
	struct UCB : public Selecter<NT>
	{
		virtual EffectNodePair<NT> select(NT* node, const GameState& state) const override
		{
			return *std::max_element(node->children.begin(), node->children.end(), bestUCB);
		}

	private:
		static bool bestUCB(EffectNodePair<NT> a, EffectNodePair<NT> b)
		{
			return ucb(a.node) < ucb(b.node);
		}

		static double ucb(NT* node)
		{
			return node->totalReward / node->visits + std::sqrt(std::log((double)node->parent->visits) / node->visits);
		}
	};
}}}
