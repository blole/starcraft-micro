#include "globals.h"
#include <string>
#include <iostream>
#include "point.h"
#include <ctime>
#include "GFrameHandler.h"
#include "globals.h"
#include "Definitions.h"

#include "Scenario1.h"
#include "Scenario2.h"
#include "Scenario3.h"
#include "Scenario4.h"
#include "Scenario5.h"
#include "Scenario6.h"
#include "Scenario7.h"
#include "Scenario8.h"
#include "Scenario9.h"
#include "Scenario10.h"

using namespace SL;
using namespace SL::Behaviors;
using namespace std;

// Instantiate the globals that were declared as "extern" in globals.h
CL_GraphicContext* SL::gc;
WorldState* SL::ws;
CL_InputContext* SL::ic;
Renderer* SL::ren;
GameLogic* SL::gl;
SoundManager* SL::sm;


class DisplayProgram : public CL_ClanApplication
{

public:
	static int main(const std::vector<CL_String> &args);
};

// Create global application object:
// You MUST include this line or the application start-up will fail to
// locate your application object.
CL_ClanApplication app(&DisplayProgram::main);


int DisplayProgram::main(const std::vector<CL_String> &args)
{
	try
	{
	if(AllocConsole()) {
		freopen("CONOUT$", "w", stdout); //redirect std io to it
		SetConsoleTitle("Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);  
	}


	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;
	CL_SetupSound setup_sound;

	CL_DisplayWindow window("Snow Leopard", 800, 600);
	CL_GraphicContext graphics_context = window.get_gc();
	SL::gc = &graphics_context;
	CL_InputContext input_context = window.get_ic();
	SL::ic = &input_context;

	CL_InputDevice keyboard = ic->get_keyboard();
	CL_InputDevice mouse = ic->get_mouse();

	
	
	cout << "Now Debugging..." <<endl;

	int scenario;
	if (args.size() > 1)
		scenario = atoi(args[1].c_str());
	else
		scenario = 10;


	 switch (scenario)
	 {
		case 1: SL::ws = loadScenario1(); break;
		case 2: SL::ws = loadScenario2(); break;
		case 3: SL::ws = loadScenario3(); break;
		case 4: SL::ws = loadScenario4(); break;
		case 5: SL::ws = loadScenario5(); break;
		case 6: SL::ws = loadScenario6(); break;
		case 7: SL::ws = loadScenario7(); break;
		case 8: SL::ws = loadScenario8(); break;
		case 9: SL::ws = loadScenario9(); break;
		case 10: SL::ws = loadScenario10(); break;
	 }

	Renderer* renderer = new Renderer();
	SL::ren = renderer;
	GameLogic* logic = new GameLogic();
	SL::gl = logic;

	GFrameHandler frame_handler;
	frame_handler.set_fps_limit(60);

	while (true)
	{
	renderer->Render();
	window.flip();
	frame_handler.limit_frame();
	frame_handler.calculate();
	//cout << "fps" << frame_handler.get_fps() << endl;
	if (CL_DisplayMessageQueue::has_messages())
		CL_DisplayMessageQueue::process();
 	logic->step();
	}
	
	return 0;
	}
	catch (CL_Exception err) {
		std::cout << "Exception caught: " << err.message.c_str() << std::endl;
		getchar();
		return -1;
	}
}