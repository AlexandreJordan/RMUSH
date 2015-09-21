#ifndef Gui_HPP
#define Gui_HPP

#include "libtcod.hpp"

struct GuiMessage
{
	char* text;
	TCODColor color;
	
	GuiMessage(const char* ptext, const TCODColor& pcolor);
	~GuiMessage();
};

class Gui
{
public:
    Gui();
    ~Gui();

	void render();
	void message(const TCODColor& pcolor, const char* ptext, ...);
	void clear();

private:
	TCODConsole* console;
	
	void renderBar(const int& px, const int& py, const int& pwidth, const char* ptext, 
				   const float& pvalue, const float& pmaxValue, const TCODColor& pbarColor, 
				   const TCODColor& pbackColor);
					
	TCODList<GuiMessage*> messages;
};

#endif
