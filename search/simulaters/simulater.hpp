#pragma once

namespace Bot { namespace Search
{
	class GameState;

	struct Simulater
	{
		virtual ~Simulater() {}
		virtual double operator()(GameState& state) const = 0;
	};
}}
