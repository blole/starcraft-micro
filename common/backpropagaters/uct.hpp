#pragma once
#include "common/effects/effect.hpp"
#include "common/backpropagaters/backpropagater.hpp"

namespace Bot { namespace Backpropagaters
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
}}
