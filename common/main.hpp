#pragma once
#include "common/common.hpp"
#include "common/generals/general.hpp"

namespace Bot
{
	template <class GeneralType>
	class Main : public BWAPI::AIModule
	{
	private:
		GeneralType general;

	public:
		Main<GeneralType>(const GeneralType& general = GeneralType())
			: general(general)
		{}

	public:
		virtual void onStart() override
		{
			Broodwar->enableFlag(BWAPI::Flag::UserInput);
			//Broodwar->enableFlag(BWAPI::Flag::CompleteMapInformation);
			Broodwar->setLatCom(true);
			//Broodwar->setCommandOptimizationLevel(2);

			if (!Broodwar->isReplay())
				general.onStart();
		}

		virtual void onFrame() override
		{
			if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
				return;

			Broodwar->drawTextScreen(200, 0, "FPS: %d (%.2f avg)", Broodwar->getFPS(), Broodwar->getAverageFPS());
			Broodwar->drawTextScreen(200, 15, "latency frames: %d (%d max)", Broodwar->getRemainingLatencyFrames(), Broodwar->getLatencyFrames());
			Broodwar->drawTextScreen(200, 30, "I'm player: %d", Broodwar->self()->getID());
			//Broodwar->drawTextScreen(100, 0, "Bot behavior is: %s", MainDescription);

			general.onFrame();
		}

		virtual void onSendText(string text) override
		{
			Broodwar->sendText("%s", text.c_str());
		}

	};
}
