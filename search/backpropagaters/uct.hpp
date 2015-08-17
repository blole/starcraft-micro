#pragma once
#include "search/effects/effect.hpp"
#include "search/backpropagaters/backpropagater.hpp"

namespace Bot { namespace Search { 	namespace Backpropagaters
{
	template <class NT>
	struct UCT : public Backpropagater<NT>
	{
		virtual void backpropagate(NT* node, double score) const override
		{
			while (node->parent != nullptr)
			{
				node->visits++;
				if (node->effect->isPlayerEffect())
					node->totalReward += score;
				else
					node->totalReward -= score;
				node = node->parent;
			}
		}
	};
}}}