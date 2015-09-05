#pragma once
#include "common/common.hpp"
#include "common/squads/squad.hpp"

namespace Bot
{
	struct General
	{
		virtual ~General() {}

		virtual void onStart() {}
		virtual void onFrame() = 0;
	};

	namespace Generals
	{
		template <class SquadType>
		struct GeneralWithSquads : General
		{
			vector<SquadType> squads;
		};
	}
}
