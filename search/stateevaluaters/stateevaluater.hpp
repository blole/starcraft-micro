#pragma once

namespace Bot { namespace Search
{
	class GameState;

	struct StateEvaluater
	{
		virtual ~StateEvaluater() {}
		virtual double operator()(GameState& state) const = 0;
	};
}}
