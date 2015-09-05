#pragma once
#include "common/generals/allunitssinglesquad.hpp"
#include "influencemap.hpp"

namespace Bot { namespace Generals
{
	template <class SquadType>
	class Kiting : public AllUnitsSingleSquad<SquadType>
	{
	public:
		Kiting(const SquadType& squad = SquadType())
			: AllUnitsSingleSquad<SquadType>(squad)
		{}

	public:
		virtual void onStart() override
		{
			InfluenceMap::init();
			AllUnitsSingleSquad<SquadType>::onStart();
		}

		virtual void onFrame() override
		{
			InfluenceMap::update();
			AllUnitsSingleSquad<SquadType>::onFrame();
		}
	};
}}
