#include "botsquad.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

void botSquad::onStart()
{
	Broodwar->enableFlag(Flag::UserInput);
	Broodwar->enableFlag(Flag::CompleteMapInformation);
	Broodwar->setCommandOptimizationLevel(2);
	
	if (!Broodwar->isReplay())
		commander.init();
}

void botSquad::onFrame()
{
	static std::string botDescription = "Attack closest in squad";

	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	commander.update();
}

void botSquad::onEnd(bool isWinner)
{
}

void botSquad::onSendText(std::string text)
{
	Broodwar->sendText("%s", text.c_str());
}

void botSquad::onReceiveText(BWAPI::Player player, std::string text)
{
}

void botSquad::onPlayerLeft(BWAPI::Player player)
{
}

void botSquad::onNukeDetect(BWAPI::Position target)
{
}

void botSquad::onUnitDiscover(BWAPI::Unit unit)
{
}

void botSquad::onUnitEvade(BWAPI::Unit unit)
{
}

void botSquad::onUnitShow(BWAPI::Unit unit)
{
}

void botSquad::onUnitHide(BWAPI::Unit unit)
{
}

void botSquad::onUnitCreate(BWAPI::Unit unit)
{
}

void botSquad::onUnitDestroy(BWAPI::Unit unit)
{
	commander.destroyUnit(unit);
}

void botSquad::onUnitMorph(BWAPI::Unit unit)
{
}

void botSquad::onUnitRenegade(BWAPI::Unit unit)
{
}

void botSquad::onSaveGame(std::string gameName)
{
}

void botSquad::onUnitComplete(BWAPI::Unit unit)
{
}
