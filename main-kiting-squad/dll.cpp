#include "common/dll.hpp"
#include "common/main.hpp"
#include "main-kiting-squad/commander.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;

	return new Main(make_shared<Commander>());
}
