#include "Renderer.h"
#include "globals.h"
using namespace SL;

Renderer::Renderer()
{
	screenStartX = 0.0;
	screenStartY = 0.0;
	screenWidth = 640; //TODO get dynamically
	screenHeight = 480; //TODO get dynamically
	CL_FontDescription desc;
	desc.set_typeface_name("Times New Roman");
	font = new CL_Font(*gc,desc);
	camera = ws->getPlayer();
}
bool Renderer::setCamera(GameObject* obj)
{
	camera = obj;
	return true;
}

bool Renderer::Render()
{
	const GameObjectList* backgrounds = ws->getBackgroundObjects();
	const GameObjectList* objects = ws->getAllGameObjects();
	gc->clear();

	ConstGameObjectIter itr;
	for (itr = backgrounds->begin(); itr!=backgrounds->end();itr++)
	{
		GameObject* obj = *itr;
		CL_Sprite* sprite = obj->sprite;
		sprite->set_angle(obj->displayHeading);
		sprite->draw(*gc,obj->location.x,obj->location.y);
	}

	for (itr = objects->begin(); itr!=objects->end();itr++)
	{
		GameObject* obj = *itr;
		CL_Sprite* sprite = obj->sprite;
		sprite->set_angle(obj->displayHeading);
		sprite->draw(*gc,obj->location.x,obj->location.y);
		/*
		if (obj->usesPhysics)
			obj->collisionOutline->draw(0,0,CL_Colorf::azure,*gc);*/
			
	}
	
	return true;

}