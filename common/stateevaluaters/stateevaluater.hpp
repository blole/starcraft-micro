#pragma once

namespace Bot
{
	class GameState;

	struct StateEvaluater
	{
		virtual ~StateEvaluater() {}
		virtual double operator()(GameState& state) const = 0;
	};

	namespace StateEvaluaters
	{
		struct Heuristic : StateEvaluater {};
		struct Simulater : StateEvaluater {};
	}
}
