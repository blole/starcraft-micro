#pragma once
#include "common/common.hpp"
#include "common/selecters/selecter.hpp"

namespace Bot { namespace Selecters
{
	template <class NT>
	struct UCB : Selecter<NT>
	{
		virtual NT& operator()(const GameState& state, NT& node) const override
		{
			return *std::max_element(node.children.begin(), node.children.end(), function<double(const NT&)>(ucb));
		}

	private:
		static double ucb(const NT& node)
		{
			return node.totalReward / node.visits + std::sqrt(std::log((double)node.parent->visits) / node.visits);
		}
	};
}}
