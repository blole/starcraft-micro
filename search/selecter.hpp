#pragma once

namespace Bot { namespace Search
{
	class GameState;
	template<class NT> struct EffectNodePair;

	template<class NT>
	struct Selecter
	{
		virtual EffectNodePair<NT>& select(NT& node, const GameState& state) const = 0;
	};
}}
