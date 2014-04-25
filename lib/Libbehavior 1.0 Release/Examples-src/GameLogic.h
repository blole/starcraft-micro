#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

class CL_InputContext;
class CL_InputDevice;

namespace SL
{
	
class WorldState;
class GameObject;
class Ship;
class Renderer;

/// Coordinates the player and AI input to drive forward the worldState
class GameLogic {

private:
	Ship* playerShip;
	CL_InputDevice* keyboard;
	CL_InputDevice* mouse;
	void handleInput();
	
	

public:
		/// Standard Constructor. Automatically gets keyboard, mouse, and player ship
		GameLogic::GameLogic();
		/// Step forward in time, get player input, and get AI actions
		bool GameLogic::step();


};
}
#endif