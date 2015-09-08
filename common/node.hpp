#pragma once
#include "common/common.hpp"

namespace Bot
{
	class Effect;
	
	template<class NT>
	struct Node
	{
		NT* const parent;
		shared_ptr<Effect> effect;
		vector<NT> children;
		bool terminal;

		Node(NT* parent, shared_ptr<Effect> effect)
			: parent(parent)
			, effect(effect)
			, terminal(false)
		{}

		virtual ~Node() {}
	};
}
