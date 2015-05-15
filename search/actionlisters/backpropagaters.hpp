#pragma once
#include "search/actionlisters/actionlister.hpp"

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
				node->totalReward += score;
				node = node->parent;
			}
		}
	};
}}}
