#pragma once
#include <algorithm>
#include "search/selecters/selecter.hpp"

namespace Bot { namespace Search { 	namespace Selecters
{
	template <class NT>
	struct UCB : public Selecter<NT>
	{
		virtual NT* select(NT* node, const GameState& state) const override
		{
			return *std::max_element(node->children.begin(), node->children.end(), bestUCB);
		}

	private:
		static bool bestUCB(const NT* a, const NT* b)
		{
			return ucb(a) < ucb(b);
		}

		static double ucb(const NT* node)
		{
			return node->totalReward / node->visits + std::sqrt(std::log((double)node->parent->visits) / node->visits);
		}
	};
}}}
