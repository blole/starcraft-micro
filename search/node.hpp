#pragma once
#include "common/common.hpp"

namespace Bot { namespace Search
{
	class Effect;
	
	template<class NT>
	class Node
	{
	public:
		NT* const parent;
		shared_ptr<Effect> effect;
		vector<unique_ptr<NT>> children;
		bool terminal;
	public:
		Node(NT* parent, shared_ptr<Effect> effect)
			: parent(parent)
			, effect(effect)
			, terminal(false)
		{}

		virtual ~Node() {}
	};
}}
