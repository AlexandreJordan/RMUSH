#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "libtcod.hpp"
#include <string>

//window
static const int WINDOW_WIDTH               = 80;
static const int WINDOW_HEIGHT              = 45;

//font
static const std::string CONSOLE_FONT       = "./data/fonts/tileset_12x12.png";

//data
static const std::string SAVE_FILEPATH      = "./data/save/save.rl";

//carte
static const int MAP_WIDTH                  = WINDOW_WIDTH;
static const int MAP_HEIGHT                 = 34;
static const int LEVELS_COUNT               = 2;

//population
static const int MIN_POPULATE_ITEM          = 1;
static const int MAX_POPULATE_ITEM          = 6;
static const int MIN_POPULATE_PNJ           = 6;
static const int MAX_POPULATE_PNJ           = 12;
static const int MIN_POPULATE_ITEMFIXED     = 1;
static const int MAX_POPULATE_ITEMFIXED     = 6;
static const int PNJ_TYPE_COUNT             = 1;
static const int ITEM_TYPE_COUNT            = 3;
static const int ITEMFIXED_TYPE_COUNT       = 1;

//infos
static const int INFOS_POS_X                = 1;
static const int INFOS_POS_Y                = MAP_HEIGHT;
static const int INFOS_WIDTH                = WINDOW_WIDTH;
static const int INFOS_HEIGHT               = WINDOW_HEIGHT - MAP_HEIGHT;
static const int MESSAGES_LINES             = 9;
static const int MESSAGES_POS_X             = 30;

//pièces
static const int ROOM_MIN_SIZE              = 6;
static const int ROOM_MAX_SIZE              = 14;
static const int ROOM_COUNT_LEVEL           = 6;
static const TCODColor C_DARKWALL           = TCODColor(  0,   0, 100);
static const TCODColor C_DARKGROUND         = TCODColor( 50,  50, 150);
static const TCODColor C_LIGHTWALL          = TCODColor(130, 110,  50);
static const TCODColor C_LIGHTGROUND        = TCODColor(200, 180,  50);
static const TCODColor C_LIGHTFALL          = TCODColor::lightRed;
static const TCODColor C_DARKFALL           = TCODColor::darkRed;

//gameplay
static const int TRACKING_TURNS             = 3;
static const int MAX_ROOM_MONSTERS          = 3;
static const int MAX_ROOM_ITEMS             = 5;

//inventaire
static const int INVENTORY_WIDTH            = 74;
static const int INVENTORY_HEIGHT           = 34;
static const int INVENTORY_POS_Y            = 3;
static const int INVENTORY_SIZE             = 10;

//fenêtre de sélection
static const int SELECTION_WIDTH			= 36;
static const int SELECTION_HEIGHT			= 24;

//télécommande
static const int REMOTE_WIDTH               = 50;
static const int REMOTE_HEIGHT              = 30;
static const int REMOTE_POS_Y               = 3;

//console de dev
static const int DEV_CONSOLE_WIDTH          = 50;
static const int DEV_CONSOLE_HEIGHT         = 25;

//console d'aide
static const int HELP_CONSOLE_WIDTH         = 60;
static const int HELP_CONSOLE_HEIGHT        = 32;

//menu ingame
static const int MENU_INGAME_WIDTH			= 45;
static const int MENU_INGAME_HEIGHT			= 9;
static const int MENU_INGAME_POS_Y			= 10;

//couleurs des messages
static const TCODColor C_MESS_INFO          = TCODColor::white;
static const TCODColor C_MESS_HIT           = TCODColor::green;
static const TCODColor C_MESS_DAMAGE        = TCODColor::red;

//couleurs des entity
static const TCODColor C_PLAYER             = TCODColor::white;
static const TCODColor C_HUMAN              = TCODColor::orange;
static const TCODColor C_DOG                = TCODColor::orange;
static const TCODColor C_ITEM_BANDAGE       = TCODColor::darkGreen;
static const TCODColor C_ITEM_STAIR         = TCODColor::white;
static const TCODColor C_ITEM_GAZTRAP       = TCODColor::white;
static const TCODColor C_ITEM_MINE          = TCODColor::red;
static const TCODColor C_ITEM_REMOTECONTROL = TCODColor::blue;
static const TCODColor C_ITEM_LEVER         = TCODColor::blue;
static const TCODColor C_ITEM_DAGUE         = TCODColor::yellow;
static const TCODColor C_ITEM_ARMOR_HEAD    = TCODColor::brass;
static const TCODColor C_ITEM_DOOR          = TCODColor::red;
static const TCODColor C_ITEM_PITFALL       = TCODColor::magenta;
static const TCODColor C_TEMP_BASE_GAZ      = TCODColor::lightPurple;

//paramètres items
static const int T_DURATION_GAZTRAP         = 5;

//caractères des entités
static const char CH_FLOOR                  = '.';
static const char CH_WALL                   = '#';
static const char CH_HUMAN                  = 'h';
static const char CH_BANDAGE                = '~';
static const char CH_PNJ_DEAD               = '%';


#endif
