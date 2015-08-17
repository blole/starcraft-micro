#include "common/main.hpp"
#include <iostream>
#include "common/reactivesquad.hpp"

using namespace BWAPI;
using namespace Filter;
using namespace Bot;

Main::Main(std::shared_ptr<General> general)
	: general(general)
{
}

void Main::onStart()
{
	Broodwar->enableFlag(Flag::UserInput);
	Broodwar->enableFlag(Flag::CompleteMapInformation);
	Broodwar->setLatCom(true);
	//Broodwar->setCommandOptimizationLevel(2);

	if (!Broodwar->isReplay())
		general->onStart();
}

void Main::onFrame()
{
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	Broodwar->drawTextScreen(200, 0, "FPS: %d (%.2f avg)", Broodwar->getFPS(), Broodwar->getAverageFPS());
	Broodwar->drawTextScreen(200, 15, "latency frames: %d (%d max)", Broodwar->getRemainingLatencyFrames(), Broodwar->getLatencyFrames());
	Broodwar->drawTextScreen(200, 30, "I'm player: %d", Broodwar->self()->getID());
	//Broodwar->drawTextScreen(100, 0, "Bot behavior is: %s", MainDescription);

	general->onFrame();
}

void Main::onEnd(bool isWinner)
{
}

void Main::onSendText(std::string text)
{
	Broodwar->sendText("%s", text.c_str());
}

void Main::onReceiveText(BWAPI::Player player, std::string text)
{
}

void Main::onPlayerLeft(BWAPI::Player player)
{
}

void Main::onNukeDetect(BWAPI::Position target)
{
}

void Main::onUnitDiscover(BWAPI::Unit unit)
{
}

void Main::onUnitEvade(BWAPI::Unit unit)
{
}

void Main::onUnitShow(BWAPI::Unit unit)
{
}

void Main::onUnitHide(BWAPI::Unit unit)
{
}

void Main::onUnitCreate(BWAPI::Unit unit)
{
}

void Main::onUnitDestroy(BWAPI::Unit unit)
{
}

void Main::onUnitMorph(BWAPI::Unit unit)
{
}

void Main::onUnitRenegade(BWAPI::Unit unit)
{
}

void Main::onSaveGame(std::string gameName)
{
}

void Main::onUnitComplete(BWAPI::Unit unit)
{
}
