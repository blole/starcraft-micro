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
			if (Broodwar->isReplay())
				return;

			Broodwar->enableFlag(BWAPI::Flag::UserInput);
			//Broodwar->enableFlag(BWAPI::Flag::CompleteMapInformation);
			Broodwar->setLatCom(true);
			//Broodwar->setCommandOptimizationLevel(2);

			general.onStart();
		}

		virtual void onFrame() override
		{
			try
			{
				if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
					return;

				Broodwar->drawTextScreen(200, 0, "FPS: %d (%.2f avg)", Broodwar->getFPS(), Broodwar->getAverageFPS());
				Broodwar->drawTextScreen(200, 15, "latency frames: %d (%d max)", Broodwar->getRemainingLatencyFrames(), Broodwar->getLatencyFrames());
				Broodwar->drawTextScreen(200, 30, "I'm player: %d", Broodwar->self()->getID());
				Broodwar->drawTextScreen(350, 0, "frame %d", Broodwar->getFrameCount());

				for (auto unit : Broodwar->self()->getUnits())
				{
					Broodwar->drawTextMap(unit->getPosition() + BWAPI::Position(-15, 10), "%s %d",
						unit->getLastCommand().getType().c_str(),
						Broodwar->getFrameCount() - unit->getLastCommandFrame());
				}

				for (auto& unit : Broodwar->getAllUnits())
					Unit::get(unit).onFrame();

				general.onFrame();
			}
			catch (std::runtime_error e)
			{
				Broodwar << BWAPI::Text::Red << "onFrame(): error: " << e.what() << endl;
			}
		}

		virtual void onSendText(string text) override
		{
			Broodwar->sendText("%s", text.c_str());
		}
	};
}
