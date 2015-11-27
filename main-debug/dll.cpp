#include "common/main.hpp"
#include "common/players/scripted.hpp"
#include "common/dll.hpp"

struct : DllInitializer
{
	virtual BWAPI::AIModule* newAIModule() override
	{
		using namespace Bot;

		return new ScriptedMain<Behaviors::AlwaysSuccess>;
	}
} initializer;

DllInitializer& DllInitializer::instance = initializer;
