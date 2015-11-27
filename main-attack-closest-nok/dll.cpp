#include "common/main.hpp"
#include "common/behaviors/attackclosest.hpp"
#include "common/dll.hpp"

struct : DllInitializer
{
	virtual BWAPI::AIModule* newAIModule() override
	{
		using namespace Bot;

		return new ScriptedMain<Behaviors::AttackClosest>;
	}
} initializer;

DllInitializer& DllInitializer::instance = initializer;
