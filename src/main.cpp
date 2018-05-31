#include <iostream>
#include <string>
#include <fstream>
#include "main.hpp"
#include "constants.hpp"

////////////////////////////////////////////////////////////////////////////////
//             __________    _____    ____ ___  _________  ___ ___            //
//             \______   \  /     \  |    |   \/   _____/ /   |   \           //
//              |       _/ /  \ /  \ |    |   /\_____  \ /    ~    \          //
//              |    |   \/    Y    \|    |  / /        \\    Y    /          //
//              |____|_  /\____|__  /|______/ /_______  / \___|_  /           //
//                     \/         \/                  \/        \/            //
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    //
    // Initialisation de libtcod
    //
    //TCODConsole::setCustomFont(CONSOLE_FONT.c_str(), (TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW), 0, 0);
    TCODConsole::setCustomFont(CONSOLE_FONT.c_str(), (TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW), 16, 48);
    TCODConsole::initRoot(WINDOW_WIDTH, WINDOW_HEIGHT, "RMUSH", false);
    TCODConsole::root->setKeyboardRepeat(175, 30);
    TCODSystem::setFps(60);

    //
    // Fichier de sauvegarde
    //  - Si existant : Chargement de la partie
    //  - Sinon création d'une nouvelle partie
    //
    GameStart gs;
    std::ifstream file(SAVE_FILEPATH, std::ios::in);

    if (!file)
        gs = GameStart::NEW_GAME;
    else
        gs = GameStart::LOAD_GAME;

    //
    // Menu principal
    //
    int posMenuY(28);
    char selector('-');
    int cursor(posMenuY);
    TCOD_key_t key;

    while (!TCODConsole::isWindowClosed())
    {
        std::string w = R"(
             __________    _____    ____ ___  _________  ___ ___
             \______   \  /     \  |    |   \/   _____/ /   |   \
              |       _/ /  \ /  \ |    |   /\_____  \ /    ~    \
              |    |   \/    Y    \|    |  / /        \\    Y    /
              |____|_  /\____|__  /|______/ /_______  / \___|_  /
                     \/         \/                  \/        \/

                                                         kz - v0.1
        )";

        TCODConsole::root->print(1, 5, w.c_str());
        TCODConsole::root->printEx(14, posMenuY, TCOD_BKGND_DEFAULT, TCOD_LEFT, "%c %s", (posMenuY == cursor) ? '>' : '-', (gs == GameStart::NEW_GAME) ? "Nouveau" : "Continuer");
        TCODConsole::root->printEx(14, posMenuY + 1, TCOD_BKGND_DEFAULT, TCOD_LEFT, "%c Quitter", (posMenuY + 1 == cursor) ? '>' : '-');

        TCODConsole::flush();

        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr, true);

        switch (key.vk)
        {
            case TCODK_UP:                
                if (cursor > posMenuY) cursor--; break;

            case TCODK_DOWN:
                if (cursor < posMenuY + 1) cursor++; break;

            case TCODK_ENTER:
            {
                if (cursor == posMenuY)
                {
                    //
                    // Nouveau / Continuer
                    //
                    Engine* engine = Engine::getInstance();

                    if (gs == GameStart::NEW_GAME)
                        engine->newGame();
                    else if (gs == GameStart::LOAD_GAME)
                        engine->loadGame();

                    do
                    {
                        engine->update();
                        engine->render();

                        TCODConsole::flush();
                    } while (engine->IsRunning);

                    delete engine;

                    exit(0);
                }
                else if (cursor == posMenuY + 1)
                {
                    //
                    // Quitter
                    //
                    exit(0);
                }
                break;
            }

            default:
                break;
            }
    }

    return 0;
}
