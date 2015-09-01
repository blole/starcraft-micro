#pragma once
#include <BWAPI.h>
#include <list>
#include "common/generalallunitssinglesquad.hpp"
#include "influencemap.hpp"

namespace Bot
{
	template <class SquadType>
	class GeneralKiting : public GeneralAllUnitsSingleSquad<SquadType>
	{
	public:
		GeneralKiting(const SquadType& squad = SquadType())
			: GeneralAllUnitsSingleSquad<SquadType>(squad)
		{}

	public:
		virtual void onStart() override
		{
			InfluenceMap::init();
			GeneralAllUnitsSingleSquad<SquadType>::onStart();
		}

		virtual void onFrame() override
		{
			InfluenceMap::update();
			GeneralAllUnitsSingleSquad<SquadType>::onFrame();
		}
	};
}
