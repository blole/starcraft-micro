#pragma once
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <BWAPI.h>

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
extern "C" __declspec(dllexport) void gameInit(BWAPI::Game* game);
extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule();
