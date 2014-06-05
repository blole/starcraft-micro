#include "bot/bot.hpp"
#include "bot/reactivesquad.hpp"
#include "bot/MCTSsquad.hpp"
#include "bot/ABCDsquad.hpp"
#include "behaviors/moverelative.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

void Bot::onStart()
{
	Broodwar->enableFlag(Flag::UserInput);
	// Set the command optimization level so that common commands can be grouped and reduce the bot's APM (Actions Per Minute).
	//Broodwar->setCommandOptimizationLevel(2);
	
	if (!Broodwar->isReplay())
	{
		general = new General([]{ return new ABCDsquad(); });
		//general = new General([]{ return new MCTSsquad(); });
		general->onStart();
	}
}

void Bot::onFrame()
{
	Broodwar->drawTextScreen(200, 0,  "FPS: %d (%.2f avg)", Broodwar->getFPS(), Broodwar->getAverageFPS());
	Broodwar->drawTextScreen(200, 15, "latency frames: %d (%d max)", Broodwar->getRemainingLatencyFrames(), Broodwar->getLatencyFrames());
	Broodwar->drawTextScreen(200, 30, "I'm player: %d", Broodwar->self()->getID());
	
	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;
	
	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	//if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
	//	return;
	
	// Iterate through all the units that we own
	Unitset myUnits = Broodwar->self()->getUnits();
	for (Unitset::iterator u = myUnits.begin(); u != myUnits.end(); ++u)
	{
		if (!u->exists() || u->isLockedDown() ||
			u->isMaelstrommed() || u->isStasised() ||
			u->isLoaded() || !u->isPowered() ||
			u->isStuck() || !u->isCompleted() ||
			u->isConstructing())
			continue;
		
		// Finally make the unit do some stuff!
	}

		if(Broodwar->getFrameCount() % 6 == 0)
			general->onFrame();
}

void Bot::onEnd(bool isWinner)
{
}

void Bot::onSendText(std::string text)
{
	// Send the text to the game if it is not being processed.
	Broodwar->sendText("%s", text.c_str());
	
	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!
}

void Bot::onReceiveText(BWAPI::Player player, std::string text)
{
	// Parse the received text
	Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void Bot::onPlayerLeft(BWAPI::Player player)
{
	// Interact verbally with the other players in the game by
	// announcing that the other player has left.
	Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void Bot::onNukeDetect(BWAPI::Position target)
{
	// Check if the target is a valid position
	if (target)
	{
		// if so, print the location of the nuclear strike target
		Broodwar << "Nuclear Launch Detected at " << target << std::endl;
	}
	else 
	{
		// Otherwise, ask other players where the nuke is!
		Broodwar->sendText("Where's the nuke?");
	}
	
	// You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void Bot::onUnitDiscover(BWAPI::Unit unit)
{
}

void Bot::onUnitEvade(BWAPI::Unit unit)
{
}

void Bot::onUnitShow(BWAPI::Unit unit)
{
}

void Bot::onUnitHide(BWAPI::Unit unit)
{
}

void Bot::onUnitCreate(BWAPI::Unit unit)
{
	if (Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount()/24;
			int minutes = seconds/60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
		}
	}
}

void Bot::onUnitDestroy(BWAPI::Unit unit)
{
}

void Bot::onUnitMorph(BWAPI::Unit unit)
{
	if (Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount()/24;
			int minutes = seconds/60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
		}
	}
}

void Bot::onUnitRenegade(BWAPI::Unit unit)
{
}

void Bot::onSaveGame(std::string gameName)
{
	Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void Bot::onUnitComplete(BWAPI::Unit unit)
{
}
