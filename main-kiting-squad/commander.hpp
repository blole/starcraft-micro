#pragma once
#include "common/general.hpp"
#include "main-kiting-squad/squadmanager.hpp"

namespace Bot
{
	class Commander : public GeneralWithSquads<SquadManager>
	{
	private:
		const static int nbUnitPerSquad = 5;

	public:
		virtual void onStart() override;
		virtual void onFrame() override;
	};
}
