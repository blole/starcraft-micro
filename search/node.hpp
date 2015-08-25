#pragma once
#include <vector>

namespace Bot { namespace Search
{
	class Effect;
	
	template<class NT>
	class Node
	{
	public:
		NT* parent;
		Effect* effect;
		std::vector<NT*> children;
	public:
		Node(NT* parent, Effect* effect)
			: parent(parent)
			, effect(effect)
		{}

		virtual ~Node()
		{
			delete effect;
			for (NT* node : children)
				delete node;
			children.clear();
		}
	};
}}
