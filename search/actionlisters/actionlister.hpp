#pragma once
#include <BWAPI.h>
#include <vector>

namespace Bot { namespace Search
{
	class Effect;
	class GameState;
	template<class NT> struct EffectNodePair;

	struct ActionLister
	{
		virtual std::vector<Effect*> actions(const GameState* gamestate) const = 0;
	};

	template<class NT>
	struct Selecter
	{
		virtual EffectNodePair<NT>& select(NT& node, const GameState& state) const = 0;
	};

	template<class NT>
	struct Backpropagater
	{
		virtual void backpropagate(NT* node) const = 0;
	};

	struct Simulater
	{
		virtual double simulate(GameState* state) const = 0;
	};

	struct HeuristicFunction
	{
		virtual double heuristicValue(const GameState* state) const = 0;
	};
}}
