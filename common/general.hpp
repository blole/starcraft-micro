#pragma once
#include "common/common.hpp"
#include "common/squad.hpp"

namespace Bot
{
	class General
	{
	public:
		virtual ~General() {}
	
	public:
		virtual void onStart() {}
		virtual void onFrame() = 0;
	};

	template <class SquadType>
	class GeneralWithSquads : public General
	{
	public:
		vector<SquadType> squads;
	};
}
