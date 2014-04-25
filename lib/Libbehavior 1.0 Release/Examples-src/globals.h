#ifndef GLOBALS_H
#define GLOBALS_H
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/sound.h>
#include "GameObject.h"
#include "WorldState.h"
#include "Renderer.h"
#include "GameLogic.h"
#include "SoundManager.h"

using namespace SL;
using namespace std;

namespace SL
{
	//these represent the overall context

	/// The screen being rendered to
	extern CL_GraphicContext* gc;
	/// The current state of the level
	extern WorldState* ws;
	/// Grabs input events (mouse and keyboard)
	extern CL_InputContext* ic;
	/// Renders to the GraphicContext
	extern Renderer* ren;
	/// Accepts player input and drives the event loop
	extern GameLogic* gl;
	extern SoundManager* sm;
}
#endif