#pragma once
#include "common/common.hpp"
#include "search/selecters/selecter.hpp"

namespace Bot { namespace Search { 	namespace Selecters
{
	template <class NT>
	struct UCB : public Selecter<NT>
	{
		virtual NT* operator()(const GameState& state, NT* node) const override
		{
			return std::max_element(node->children.begin(), node->children.end(), function<double(const unique_ptr<NT>&)>(ucb))->get();
		}

	private:
		static double ucb(const unique_ptr<NT>& node)
		{
			return node->totalReward / node->visits + std::sqrt(std::log((double)node->parent->visits) / node->visits);
		}
	};
}}}
