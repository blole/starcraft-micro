#include "common/dll.hpp"
#include "botsquad.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	return new Bot::BotSquad();
}
