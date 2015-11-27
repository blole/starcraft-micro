#pragma once
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <BWAPI.h>
#include "common/generals/allunitssinglesquad.hpp"
#include "common/players/scripted.hpp"
#include "common/squads/playing.hpp"
#include "common/main.hpp"


struct DllInitializer
{
	void* hModule = nullptr;
	static DllInitializer& instance;

	virtual ~DllInitializer() {}

	virtual BWAPI::AIModule* newAIModule() = 0;
	virtual void gameInit(BWAPI::Game* game)
	{
		BWAPI::BroodwarPtr = game;
	}

	virtual bool dllMain(HANDLE hModule, unsigned long ul_reason_for_call)
	{
		switch (ul_reason_for_call)
		{
		case DLL_PROCESS_ATTACH:
			this->hModule = hModule;
			return onAttach();
		case DLL_PROCESS_DETACH:
			return onDetach();
		default:
			return true;
		}
	}

	virtual bool onAttach() { return true; }
	virtual bool onDetach() { return true; }
};

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	return DllInitializer::instance.dllMain(hModule, ul_reason_for_call);
}

extern "C" __declspec(dllexport) void gameInit(BWAPI::Game* game)
{
	DllInitializer::instance.gameInit(game);
}

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	return DllInitializer::instance.newAIModule();
}

namespace Bot
{
	//template <typename PlayerType>
	//using PlayerMain = Bot::Main<Bot::Generals::AllUnitsSingleSquad<Bot::Squads::Playing<PlayerType>>>;
	template <typename PlayerType>
	struct PlayerMain : Main<Generals::AllUnitsSingleSquad<Squads::Playing<PlayerType>>>
	{
		PlayerMain(PlayerType& player = PlayerType())
			: Main<Generals::AllUnitsSingleSquad<Squads::Playing<PlayerType>>>(
				Generals::AllUnitsSingleSquad<Squads::Playing<PlayerType>>(
					Squads::Playing<PlayerType>(player)))
		{}
	};

	//template <typename BehaviorTreeType>
	//using ScriptedMain = Main<Generals::AllUnitsSingleSquad<Squads::Playing<Players::Scripted<BehaviorTreeType>>>>;
	template <typename BehaviorTreeType>
	struct ScriptedMain : PlayerMain<Players::Scripted<BehaviorTreeType>>
	{
		ScriptedMain(BehaviorTreeType& behaviorTree = BehaviorTreeType())
			: PlayerMain<Players::Scripted<BehaviorTreeType>>(
				Players::Scripted<BehaviorTreeType>(behaviorTree))
		{}
	};
}
