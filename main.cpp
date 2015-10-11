/*******************************************************************************
 * RMUSH - RogueLike
 * 
 * Auteur 	: Alexandre JORDAN
 * Date		: 18/08/2015
 * 
 ******************************************************************************/

#include <iostream>
#include "libtcod.hpp"
#include "engine.hpp"
#include "constants.hpp"

int main(int argc, char* argv[])
{
	Engine* engine = Engine::getInstance();
	engine->initialize();
	
    while (!TCODConsole::isWindowClosed())
	{
		engine->update();
		engine->render();
		
		TCODConsole::root->hline(2, INFOS_POS_Y - 1, WINDOW_WIDTH - 4);
        TCODConsole::flush();
    }
	
	delete engine;
	
	return 0;
}
