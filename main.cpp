/*******************************************************************************
 * RMUSH - RogueLike
 * 
 * Auteur 	: Alexandre JORDAN
 * Date		: 18/08/2015
 * 
 ******************************************************************************/

#include <iostream>
#include "main.hpp"

int main(int argc, char* argv[])
{
	std::cout << "RMUSH - AJ" << std::endl;

	Engine* engine = Engine::getInstance();
	
	engine->load();
    while (!TCODConsole::isWindowClosed())
	{
		engine->update();
		engine->render();
		
		TCODConsole::root->hline(2, INFOS_POS_Y - 1, WINDOW_WIDTH - 4);
        TCODConsole::flush();
    }
	engine->save();
	
	delete engine;
	
	return 0;
}
