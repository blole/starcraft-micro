#pragma once

namespace Bot { namespace Search
{
	class GameState;

	struct Simulater
	{
		virtual ~Simulater() {}
		virtual double simulate(GameState* state) const = 0;
	};
}}
