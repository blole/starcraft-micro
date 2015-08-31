#include "botsquad.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;
using namespace Bot;

void BotSquad::onStart()
{
	Broodwar->enableFlag(Flag::UserInput);
	Broodwar->enableFlag(Flag::CompleteMapInformation);
	Broodwar->setCommandOptimizationLevel(2);
	
	if (!Broodwar->isReplay())
		commander.init();
}

void BotSquad::onFrame()
{
	static std::string botDescription = "Attack closest in squad";

	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	commander.update();
}

void BotSquad::onEnd(bool isWinner)
{
}

void BotSquad::onSendText(std::string text)
{
	Broodwar->sendText("%s", text.c_str());
}

void BotSquad::onReceiveText(BWAPI::Player player, std::string text)
{
}

void BotSquad::onPlayerLeft(BWAPI::Player player)
{
}

void BotSquad::onNukeDetect(BWAPI::Position target)
{
}

void BotSquad::onUnitDiscover(BWAPI::Unit unit)
{
}

void BotSquad::onUnitEvade(BWAPI::Unit unit)
{
}

void BotSquad::onUnitShow(BWAPI::Unit unit)
{
}

void BotSquad::onUnitHide(BWAPI::Unit unit)
{
}

void BotSquad::onUnitCreate(BWAPI::Unit unit)
{
}

void BotSquad::onUnitDestroy(BWAPI::Unit unit)
{
	commander.destroyUnit(unit);
}

void BotSquad::onUnitMorph(BWAPI::Unit unit)
{
}

void BotSquad::onUnitRenegade(BWAPI::Unit unit)
{
}

void BotSquad::onSaveGame(std::string gameName)
{
}

void BotSquad::onUnitComplete(BWAPI::Unit unit)
{
}
