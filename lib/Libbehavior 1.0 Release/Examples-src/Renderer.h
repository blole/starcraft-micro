#ifndef RENDERER_H_
#define RENDERER_H_

#include <map>
#include "boost/lexical_cast.hpp"
#include <stdlib.h>

class CL_DisplayWindow;
class CL_GraphicContext;
class CL_ResourceManager;
class CL_Font;

namespace SL
{
class WorldState;
class GameObject;

/// Render sprites from the worldState to the graphics context
class Renderer{
public:
	/// Constructor. Initialize resources and get the camera from the worldState
	Renderer::Renderer();
	/// Render to the global graphics context
	bool Renderer::Render();
	/// The width of the screen, in pixels
	int screenWidth;
	/// The height of the screen, in pixels
	int screenHeight;
	/// Set which object to center the display on
	bool Renderer::setCamera(GameObject* obj);

private:
	GameObject* camera;
	CL_ResourceManager* resources;
	double screenStartX;
	double screenStartY;
	CL_Font* font;

};
}
#endif