#pragma once
#include <list>
#include "search/gamestate.hpp"

namespace Bot { namespace Search
{
	class Effect;

	template<class NT>
	struct EffectNodePair
	{
		Effect* effect;
		NT* node;
		EffectNodePair(Effect* effect, NT* node = nullptr)
			: effect(effect)
			, node(node)
		{}
		~EffectNodePair()
		{
			delete effect;
			delete node;
		}
	};

	template<class NT>
	class Node
	{
	public:
		NT* parent;
		Effect* effect;
		std::vector<EffectNodePair<NT>> children;
	public:
		Node(NT* parent, Effect* effect)
			: parent(parent)
			, effect(effect)
		{}

		virtual ~Node()
		{}
	};

	class Searcher
	{
	public:
		virtual std::vector<Effect*> search(GameState* gamestate) = 0;
	};
}}
