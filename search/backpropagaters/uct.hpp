#pragma once
#include "search/effects/effect.hpp"
#include "search/backpropagaters/backpropagater.hpp"

namespace Bot { namespace Search { 	namespace Backpropagaters
{
	template <class NT>
	struct UCT : Backpropagater<NT>
	{
		virtual void operator()(const GameState& state, NT& node_, double score) const override
		{
			NT* node = &node_;
			do
			{
				++node->visits;
				if (node->effect->isPlayerEffect(state))
					node->totalReward += score;
				else
					node->totalReward -= score;
				node = node->parent;
			}
			while (node);
		}
	};
}}}
