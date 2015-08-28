#pragma once

namespace Bot { namespace Search
{
	class GameState;

	template<class NT>
	struct Backpropagater
	{
		virtual ~Backpropagater() {}
		virtual void operator()(NT* node, double score) const = 0;
	};
}}
