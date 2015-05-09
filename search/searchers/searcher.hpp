#pragma once
#include <list>
#include "search/gamestate.hpp"
#include "search/actionlisters/actionlister.hpp"

namespace Bot { namespace Search
{
	class Effect;

	template<class NT>
	class EffectNodePair
	{
	public:
		Effect* effect;
		NT* node;
	public:
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
		typedef EffectNodePair<NT> Child;
	public:
		NT* parent;
		std::vector<Child> children;
	public:
		Node(NT* parent)
			: parent(parent)
		{}

		virtual ~Node()
		{}
	};

	class Searcher
	{
	public:
		virtual std::vector<Effect*> search(GameState* gamestate, ActionLister* possibleActions) = 0;
	};
}}
